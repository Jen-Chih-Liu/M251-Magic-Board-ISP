#define  _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "ISP_COMMAND.h"
#include <time.h>
#include "cmdline.h"
#include "stdio.h"
#include "malloc.h"
//HOW TO RUN 
//NUVOTON_ISP_COMMAND.exe -? =>show supports all command
//NUVOTON_ISP_COMMAND.EXE -u 1 -b 1  => usb port, DUMP BOOT state
//NUVOTON_ISP_COMMAND.EXE -u 1 -l 1  => usb port, jumper to ldrom
//NUVOTON_ISP_COMMAND.EXE -u 1 -a 1  => usb port, jumper to arpom
//NUVOTON_ISP_COMMAND.EXE -u 1 -v 1  => usb port, dump fw version
//NUVOTON_ISP_COMMAND.EXE -u 1 -g 1 -f c:\APROM_DEMO.bin => usb port, open bin file and do erase, program, verify
//NUVOTON_ISP_COMMAND.EXE -u 1 -e 1  =>erase user data string area
//NUVOTON_ISP_COMMAND.EXE -u 1 -w 55-66-88  =>write user data
//NUVOTON_ISP_COMMAND.EXE -u 1 -p 1  =>erase user image data area
//NUVOTON_ISP_COMMAND.EXE -u 1 -i 1 -f c:\img.bmp
//NUVOTON_ISP_COMMAND.EXE -u 1 -r 1  =>updated target rtc time
//#define bin_inside 
#define checksum_inside
using namespace std;




int _tmain(int argc, _TCHAR* argv[])
{
#if 0
		FILE *file = fopen("c:\\img.bmp", "rb"); // 以二进制只读方式打开文件
		if (file == NULL) {
			perror("Error opening file");
			return 1;
		}

		BMPHeader header;
		fread(&header, sizeof(header), 1, file); // 读取文件头信息

		// 验证是否为 BMP 文件
		if (header.type != 0x4D42) {
			fprintf(stderr, "Not a BMP file\n");
			fclose(file);
			return 1;
		}

		// 打印图像信息
		printf("Width: %d\n", header.width);
		printf("Height: %d\n", header.height);
		printf("Bits per pixel: %d\n", header.bpp);

		// 计算每行像素字节数
		int bytes_per_pixel = header.bpp / 8;
		int row_size = header.width / 8; // 单色图像中每个像素只占用一个位
		if (header.width % 8 != 0)
			row_size++; // 如果图像宽度不能被 8 整除，则需要向上取整

		// 分配内存来存储像素数据
		unsigned char *data = (unsigned char*)malloc(row_size * header.height);
		if (data == NULL) {
			perror("Memory allocation failed");
			fclose(file);
			return 1;
		}

		// 读取像素数据
		fseek(file, header.offset, SEEK_SET);
		fread(data, 4 * header.height, 1, file);
		for (int i = 0; i < 32; i++)
			printf("%d=0x%x\n\r",i, data[i]);

		// 在这里可以使用像素数据进行进一步处理

		// 释放内存并关闭文件
		free(data);
		fclose(file);
		return 0;

#endif
	std::string WAPD_value;
	
	clock_t start_time, end_time;
	float total_time = 0;
	cmdline::parser theArgs;
#ifdef bin_inside	

#else
	theArgs.add<string>("AFN", 'f', "AP file name", false, "");
#endif	
	theArgs.add<string>("COM", 'c', "COM port number", false, ""); 
	theArgs.add<string>("WAPD", 'w', "write APROM user data area, ascii string", false, "");
    theArgs.add<bool>("WAPI", 'i', "write APROM user image area, ascii string", false, false);
	theArgs.add<bool>("USB", 'u', "USB port", false, false);
	theArgs.add<bool>("All", 'g', "Ldrom boot chek, check version, erase, verify, boot to aprom ", false, false);
	theArgs.add<bool>("RUNAP", 'a', "Run aprom run", false, false);
	theArgs.add<bool>("RUNLD", 'l', "Run ldrom run", false, false);
	theArgs.add<bool>("FWVER", 'v', "Dump mcu FW version", false, false);
	theArgs.add<bool>("BC", 'b', "Boot Check", false, false);
    theArgs.add<bool>("RTC", 'r', "Update RTC Time", false, false);
	theArgs.add<bool>("Time", 't', "Show progamming time", false, false);
	theArgs.add<bool>("EAPD", 'e', "Erase APROM user data area", false, false);
	theArgs.add<bool>("EAPI", 'p', "Erase APROM user image area", false, false);
	theArgs.set_program_name(argv[0]);
	theArgs.parse_check(argc, &argv[0]);


	if (theArgs.get<bool>("Time") == 1)
	{

		start_time = clock(); /* mircosecond */
	}

	if ((theArgs.get<bool>("USB") == 0) && (theArgs.get<string>("COM") == ""))
	{
		printf("please input -? \n\r");
		return 0;
	}
	if ((theArgs.get<bool>("USB") == 1) && (theArgs.get<string>("COM") != ""))
	{
		printf("please donn't input -u 1 and -c com[n] in same time\n\r");
		return 0;
	}
	 
	ISP_COMMAND *ISP = new ISP_COMMAND();
//uart interface
	if (theArgs.get<string>("COM") != "")
	{
		std::string comPortStr = theArgs.get<std::string>("COM");
		_TCHAR comPort[256]; // set buffer

		_tcscpy(comPort, _T(comPortStr.c_str()));
		if (ISP->OPEN_COMPORT(comPort) != RES_CONNECT)
		{
			printf("COMPORT NO FOUND, plase check your comport\n\r");
			goto UART_EXIT;
		}
		printf("Polling MCU!!\n\r");
		if (ISP->CHECK_UART_LINK() == RES_DETECT_MCU)
			printf("Detect MCU!!\n\r");
		else
		{
			printf("No detect MCU!!\n\r");
			goto UART_EXIT;
		}
		ISP->SN_PACKAGE_UART();
		ISP->SN_PACKAGE_UART();
		if (theArgs.get<bool>("All") == 1)
		{

#ifdef bin_inside	
			ISP->Copy_BIN_Array();
#else
			std::string AFN_value = theArgs.get<std::string>("AFN");

			if (AFN_value.empty()) {
				printf("Please input -AFN [FILE NAME]\n\r");
				delete ISP;
				return 0;
			}
			else
			{
				std::string AFNStr = theArgs.get<std::string>("AFN");
				_TCHAR fileanme[256]; // 設定一個足夠大的緩衝區
				_tcscpy(fileanme, _T(AFNStr.c_str()));
				if (ISP->File_Open_APROM(fileanme) == RES_FILE_NO_FOUND)
				{
					printf("FILE NO FOUND\n\r");
					delete ISP;
					return 0;
				}
				printf("File name: %s\n\r", fileanme);
				printf("File size: %d\n\r", ISP->file_size);
				printf("File checksum: %d\n\r", ISP->file_checksum);


				if (ISP->file_size > baprom_size)
				{
					printf("bin file over flash size\n\r");
					delete ISP;
					return 0;
				}
				printf("File size: 0x%x\n\r", ISP->file_size);
				printf("File checksum: 0x%x\n\r", ISP->file_checksum);
			}

#endif

#ifdef checksum_inside
			ISP->APROM_AND_CHECKSUM();
			printf("aprom with check sum inside File size: %d\n\r", ISP->file_size);
			printf("aprom with check sun inside File checksum: %d\n\r", ISP->file_checksum);
#endif

			unsigned int temp = ISP->READFW_VERSION_UART();

			printf("FW version: 0x%x \n\r", temp);

			if (ISP->READ_PID_UART() == RES_FALSE)
			{
				printf("CHIP NO FOUND\n\r");
				goto UART_EXIT;
			}
			if (ISP->CHECK_BOOT_UART() != 2)
			{
				printf("it is boot in aprom\n\r");
				goto UART_EXIT;
			}
			//READ CONFIG
			printf("config \n\r");
			ISP->READ_CONFIG_UART();

			//test updata aprom
			ISP->UPDATE_APROM_UART();

			//reboot mcu to aprom
			ISP->RUN_TO_APROM_UART();
		}
		if (theArgs.get<bool>("BC") == 1)
		{
			unsigned int temp = 0;
			temp = ISP->CHECK_BOOT_UART();
			printf("Mode = 1, Run in APROM\n\r");
			printf("Mode = 2, Run in LDROM\n\r");
			printf("BOOT:0x%x\n\r", temp);
		}
		if (theArgs.get<bool>("FWVER") == 1)
		{
			unsigned int temp = ISP->READFW_VERSION_UART();
			printf("FW version: 0x%x \n\r", temp);
		}
		if (theArgs.get<bool>("RUNAP") == 1)
		{
			ISP->RUN_TO_APROM_UART();
		}
		if (theArgs.get<bool>("RUNLD") == 1)
		{
			ISP->RUN_TO_LDROM_UART();
		}

	UART_EXIT:
		ISP->CLOSE_UART_PORT();
		delete ISP;
	}

	//usb interface
	if (theArgs.get<bool>("USB") == 1)
	{

		if (ISP->OPEN_USBPORT() != RES_CONNECT)
		{
			printf("USB NO FOUND\n\r");
			goto USB_EXIT;
		}

		//START ;
		ISP->SN_PACKAGE_USB();
		ISP->SN_PACKAGE_USB();


		WAPD_value = theArgs.get<std::string>("WAPD");
		if (WAPD_value.empty())
		{
			//printf("Please input -WAPD [STRING]\n\r");
		}
		else
		{
					std::string WAPDStr = theArgs.get<std::string>("WAPD");
					_TCHAR user_String[256]; // 設定一個足夠大的緩衝區
					_tcscpy(user_String, _T(WAPDStr.c_str()));
					unsigned char wlength = _tcslen(user_String);
					//WRITE DATA TO BUFFER
					printf("user string:%s\n\r", user_String);
					printf("user string length:%d\n\r", wlength);
					ISP->WRITE_APROM_DATA_USB(user_String, wlength);
		}
		

		if (theArgs.get<bool>("WAPI") == 1)
		{
			std::string AFN_value = theArgs.get<std::string>("AFN");

			if (AFN_value.empty()) {
				printf("Please input -AFN [FILE NAME]\n\r");
				delete ISP;
				return 0;
			}
			else
			{
				std::string AFNStr = theArgs.get<std::string>("AFN");
				_TCHAR filename[256]; // 設定一個足夠大的緩衝區
				_tcscpy(filename, _T(AFNStr.c_str()));

				if (ISP->WRITE_APROM_IMAGE_USB(filename) == RES_FILE_NO_FOUND)
				{
					printf("FILE NO FOUND\n\r");
					delete ISP;
					return 0;
				}
		    }
		}

		if (theArgs.get<bool>("All") == 1)
		{
#ifdef bin_inside	
			ISP->Copy_BIN_Array();
#else
			std::string AFN_value = theArgs.get<std::string>("AFN");

			if (AFN_value.empty()) {
				printf("Please input -AFN [FILE NAME]\n\r");
				delete ISP;
				return 0;
			}
			else
			{
				std::string AFNStr = theArgs.get<std::string>("AFN");
				_TCHAR filename[256]; // 設定一個足夠大的緩衝區
				_tcscpy(filename, _T(AFNStr.c_str()));

				if (ISP->File_Open_APROM(filename) == RES_FILE_NO_FOUND)
				{
					printf("FILE NO FOUND\n\r");
					delete ISP;
					return 0;
				}
				printf("File name: %s\n\r", filename);
				printf("File size: %d\n\r", ISP->file_size);
				printf("File checksum: %d\n\r", ISP->file_checksum);

				if (ISP->file_size > baprom_size)
				{
					printf("bin file over flash size\n\r");
					delete ISP;
					return 0;
				}

				printf("File size: 0x%x\n\r", ISP->file_size);
				printf("File checksum: 0x%x\n\r", ISP->file_checksum);
			}

#endif

#ifdef checksum_inside
			ISP->APROM_AND_CHECKSUM();
			printf("aprom with check sum inside File size: %d\n\r", ISP->file_size);
			printf("aprom with check sun inside File checksum: %d\n\r", ISP->file_checksum);
#endif
			//CHECK FW
			printf("FW version: 0x%x \n\r", ISP->READFW_VERSION_USB());
			//CHECK PID
			if (ISP->READ_PID_USB() == RES_FALSE)
			{
				printf("CHIP NO FOUND\n\r");
				goto USB_EXIT;
			}
			if (ISP->CHECK_BOOT_USB() != 2)
			{
				printf("it is boot in aprom\n\r");
				goto USB_EXIT;
			}
			//READ CONFIG
			printf("config \n\r");
			ISP->READ_CONFIG_USB();

			//test updata aprom
			ISP->UPDATE_APROM_USB();

			//reboot mcu to aprom
			ISP->RUN_TO_APROM_USB();
		}
		if (theArgs.get<bool>("BC") == 1)
		{
			unsigned int temp = 0;
			temp=ISP->CHECK_BOOT_USB();
			printf("Mode = 1, Run in APROM\n\r");
			printf("Mode = 2, Run in LDROM\n\r");
			printf("BOOT:0x%x\n\r", temp);
		}
		if (theArgs.get<bool>("FWVER") == 1)
		{
			unsigned int temp = ISP->READFW_VERSION_USB();
			printf("FW version: 0x%x \n\r", temp);
		}
		if (theArgs.get<bool>("RUNAP") == 1)
		{
			ISP->RUN_TO_APROM_USB();
		}
		if (theArgs.get<bool>("RUNLD") == 1)
		{
			ISP->RUN_TO_LDROM_USB();
		}
		if (theArgs.get<bool>("EAPD") == 1)
		{
			printf("erase user data in aprom\n\r");
			ISP->Erase_APROM_DATA_USB();
		}
		if (theArgs.get<bool>("EAPI") == 1)
		{
			printf("erase user image in aprom\n\r");
			ISP->Erase_APROM_IMAGE_USB();
		}
		if (theArgs.get<bool>("RTC") == 1)
		{
			ISP->UPDATE_RTC_USB();
		}
		
	USB_EXIT:
		//close usb port
		ISP->CLOSE_USB_PORT();

		delete ISP;
	}

	if (theArgs.get<bool>("Time") == 1)
	{
		end_time = clock();
		/* CLOCKS_PER_SEC is defined at time.h */
		total_time = (float)(end_time - start_time) / CLOCKS_PER_SEC;

		printf("Time : %f sec \n", total_time);
	}
	return 0;

}

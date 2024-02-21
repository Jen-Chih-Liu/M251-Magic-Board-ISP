#include "StdAfx.h"
#include "ISP_COMMAND.h"
#include "HID.h"
#include <time.h>
#include "aprom.h"
#include "stdint.h"
//#define dbg_printf printf
#define dbg_printf(...) 
#define USB_VID 0x0416
#define USB_PID 0x3f00
#define Time_Out_Value 1000
#define Package_Size 64
CHidCmd pUSB;
DWORD Length;
unsigned char buffer[Package_Size]={0};
unsigned int PacketNumber;
unsigned int Address,Size;
unsigned char send_buf[64];
#define CMD_UPDATE_APROM	0x000000A0
void WordsCpy(void *dest, void *src, unsigned int size)
{
	unsigned char *pu8Src, *pu8Dest;
	unsigned int i;

	pu8Dest = (unsigned char *)dest;
	pu8Src = (unsigned char *)src;

	for (i = 0; i < size; i++)
		pu8Dest[i] = pu8Src[i];
}
ISP_STATE ISP_COMMAND::OPEN_USBPORT(void)
{
	if(!pUSB.OpenDevice(USB_VID, USB_PID))
	{
		printf("connect false\n\r");
		return RES_CONNECT_FALSE;
	}
	else 
		USB_OPEN_FLAG=1;
		return RES_CONNECT;
}



ISP_STATE ISP_COMMAND::OPEN_COMPORT(_TCHAR* temp)
{
		COMMTIMEOUTS CommTimeOuts ; //¶¨Òå³¬Ê±½á¹¹£¬²¢ÌîÐ´¸Ã½á¹¹
		DCB dcb;                    //¶¨ÒåÊý¾Ý¿ØÖÆ¿é½á¹¹ 
		
		memset(&CommTimeOuts, 0, sizeof(CommTimeOuts));
		memset(&dcb, 0, sizeof(dcb));
		CommTimeOuts.ReadIntervalTimeout = 20;//ms
		CommTimeOuts.ReadTotalTimeoutMultiplier=1;
		CommTimeOuts.ReadTotalTimeoutConstant=1;
//m_bPortReady = false;
//m_sComPort = "//";
		char comport_path[100] = "\\\\.\\";
		strcat(comport_path,temp);
m_hCom = CreateFile(comport_path, 
		GENERIC_READ | GENERIC_WRITE,
		0, // exclusive access
		NULL, // no security
		OPEN_EXISTING,
		0, // no overlapped I/O
		NULL); // null template

//PurgeComm( m_hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ); 
if(SetupComm(m_hCom, 128, 128)==true&&GetCommState(m_hCom, &m_dcb)==true)
{
m_dcb.BaudRate = 115200;
m_dcb.ByteSize = 8;
m_dcb.fBinary = TRUE ;
m_dcb.Parity = NOPARITY;
m_dcb.StopBits = ONESTOPBIT;
		m_dcb.fBinary = TRUE ;
			m_dcb.fParity = FALSE; 
				m_dcb.fDtrControl = 0;
SetCommState(m_hCom, &m_dcb);
SetCommMask(m_hCom, EV_RXCHAR|EV_TXEMPTY );
SetupComm(m_hCom, 1024, 128);
SetCommTimeouts( m_hCom, &CommTimeOuts );
/*
GetCommTimeouts (m_hCom, &m_CommTimeouts);
m_CommTimeouts.ReadIntervalTimeout = 20;
m_CommTimeouts.ReadTotalTimeoutConstant = 0;
m_CommTimeouts.ReadTotalTimeoutMultiplier = 0;
m_CommTimeouts.WriteTotalTimeoutConstant = 0;
m_CommTimeouts.WriteTotalTimeoutMultiplier = 0;
SetCommTimeouts (m_hCom, &m_CommTimeouts);
*/
PurgeComm( m_hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ); 
	COM_OPEN_FLAG=1;		
	return RES_CONNECT;
}
return RES_CONNECT_FALSE;

}


ISP_STATE ISP_COMMAND::OPEN_COMPORT(void)
{
		COMMTIMEOUTS CommTimeOuts ; //¶¨Òå³¬Ê±½á¹¹£¬²¢ÌîÐ´¸Ã½á¹¹
		DCB dcb;                    //¶¨ÒåÊý¾Ý¿ØÖÆ¿é½á¹¹ 
		
		memset(&CommTimeOuts, 0, sizeof(CommTimeOuts));
		memset(&dcb, 0, sizeof(dcb));
		CommTimeOuts.ReadIntervalTimeout = 20;//ms
		CommTimeOuts.ReadTotalTimeoutMultiplier=1;
		CommTimeOuts.ReadTotalTimeoutConstant=1;
//m_bPortReady = false;
//m_sComPort = "//";
m_hCom = CreateFile("\\\\.\\com18", 
		GENERIC_READ | GENERIC_WRITE,
		0, // exclusive access
		NULL, // no security
		OPEN_EXISTING,
		0, // no overlapped I/O
		NULL); // null template

//PurgeComm( m_hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ); 
if(SetupComm(m_hCom, 128, 128)==true&&GetCommState(m_hCom, &m_dcb)==true)
{
m_dcb.BaudRate = 115200;
m_dcb.ByteSize = 8;
m_dcb.fBinary = TRUE ;
m_dcb.Parity = NOPARITY;
m_dcb.StopBits = ONESTOPBIT;
		m_dcb.fBinary = TRUE ;
			m_dcb.fParity = FALSE; 
				m_dcb.fDtrControl = 0;
SetCommState(m_hCom, &m_dcb);
SetCommMask(m_hCom, EV_RXCHAR|EV_TXEMPTY );
SetupComm(m_hCom, 1024, 128);
SetCommTimeouts( m_hCom, &CommTimeOuts );
/*
GetCommTimeouts (m_hCom, &m_CommTimeouts);
m_CommTimeouts.ReadIntervalTimeout = 20;
m_CommTimeouts.ReadTotalTimeoutConstant = 0;
m_CommTimeouts.ReadTotalTimeoutMultiplier = 0;
m_CommTimeouts.WriteTotalTimeoutConstant = 0;
m_CommTimeouts.WriteTotalTimeoutMultiplier = 0;
SetCommTimeouts (m_hCom, &m_CommTimeouts);
*/
PurgeComm( m_hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ); 
	COM_OPEN_FLAG=1;		
	return RES_CONNECT;
}
return RES_CONNECT;

}


ISP_STATE ISP_COMMAND::CLOSE_UART_PORT()
{
if(COM_OPEN_FLAG==1)
	CloseHandle(m_hCom);
	return RES_DISCONNECT;

} 


ISP_STATE ISP_COMMAND::CLOSE_USB_PORT(void)
{
if(USB_OPEN_FLAG==1)
	pUSB.CloseDevice();
	return RES_DISCONNECT;
}




ISP_STATE ISP_COMMAND::CHECK_UART_LINK(void)
{
unsigned char cmd[Package_Size] = {0xae,0,0,0,
(PacketNumber&0xff),((PacketNumber>>8)&0xff),((PacketNumber>>16)&0xff),((PacketNumber>>24)&0xff)};
while(1)
{
WriteFile(m_hCom,cmd,Package_Size,&iBytesWritten,NULL);
ReadFile(m_hCom, &buffer,Package_Size, &iBytesRead, NULL);
dbg_printf("package: 0x%x\n\r",buffer[4]);
		if((buffer[4]|((buffer[5]<<8)&0xff00)|((buffer[6]<<16)&0xff0000)|((buffer[7]<<24)&0xff000000))==(PacketNumber+1))
			break;
}
return RES_DETECT_MCU;
}

ISP_STATE ISP_COMMAND::SN_PACKAGE_UART(void)
{
	clock_t start_time, end_time;
	float total_time = 0;

	unsigned char cmd1[Package_Size] = {0xa4,0,0,0,
		(PacketNumber&0xff),((PacketNumber>>8)&0xff),((PacketNumber>>16)&0xff),((PacketNumber>>24)&0xff),
		(PacketNumber&0xff),((PacketNumber>>8)&0xff),((PacketNumber>>16)&0xff),((PacketNumber>>24)&0xff)};
	WriteFile(m_hCom,cmd1,Package_Size,&iBytesWritten,NULL);		
	start_time = clock(); /* mircosecond */ 
	while(1)
	{
		ReadFile(m_hCom, &buffer,Package_Size, &iBytesRead, NULL);
		dbg_printf("package: 0x%x\n\r",buffer[4]);
		if((buffer[4]|((buffer[5]<<8)&0xff00)|((buffer[6]<<16)&0xff0000)|((buffer[7]<<24)&0xff000000))==(PacketNumber+1))
			break;
		end_time = clock(); 
		/* CLOCKS_PER_SEC is defined at time.h */ 
		if((end_time - start_time)>Time_Out_Value)
			return RES_TIME_OUT;
	}
	PacketNumber+=2;
	return RES_SN_OK;
}


unsigned int ISP_COMMAND::READFW_VERSION_UART(void)
{
	clock_t start_time, end_time;
	float total_time = 0;

	unsigned char cmd[Package_Size] = {0xa6,0,0,0,
		(PacketNumber&0xff),((PacketNumber>>8)&0xff),((PacketNumber>>16)&0xff),((PacketNumber>>24)&0xff)};
	WriteFile(m_hCom,cmd,Package_Size,&iBytesWritten,NULL);					
	start_time = clock(); /* mircosecond */  
	while(1)
	{
		ReadFile(m_hCom, &buffer,Package_Size, &iBytesRead, NULL);
		dbg_printf("package: 0x%x\n\r",buffer[4]);
		if((buffer[4]|((buffer[5]<<8)&0xff00)|((buffer[6]<<16)&0xff0000)|((buffer[7]<<24)&0xff000000))==(PacketNumber+1))
			break;

		end_time = clock(); 
		/* CLOCKS_PER_SEC is defined at time.h */ 
		if((end_time - start_time)>Time_Out_Value)
			return 0;

	}
	dbg_printf("fw version:0x%x\n\r",buffer[8]);
	dbg_printf("\n\r");
	PacketNumber+=2;
	return (buffer[8]|((buffer[9]<<8)&0xff00)|((buffer[10]<<16)&0xff0000)|((buffer[11]<<24)&0xff000000));
}


unsigned int ISP_COMMAND::CHECK_BOOT_UART(void)
{
	clock_t start_time, end_time;
	float total_time = 0;

	unsigned char cmd[Package_Size] = { 0xa6,0,0,0,
		(PacketNumber & 0xff),((PacketNumber >> 8) & 0xff),((PacketNumber >> 16) & 0xff),((PacketNumber >> 24) & 0xff) };
	WriteFile(m_hCom, cmd, Package_Size, &iBytesWritten, NULL);
	start_time = clock(); /* mircosecond */
	while (1)
	{
		ReadFile(m_hCom, &buffer, Package_Size, &iBytesRead, NULL);
		dbg_printf("package: 0x%x\n\r", buffer[4]);
		if ((buffer[4] | ((buffer[5] << 8) & 0xff00) | ((buffer[6] << 16) & 0xff0000) | ((buffer[7] << 24) & 0xff000000)) == (PacketNumber + 1))
			break;

		end_time = clock();
		/* CLOCKS_PER_SEC is defined at time.h */
		if ((end_time - start_time) > Time_Out_Value)
			return 0;

	}
	//Mode = 1, Run in APROM
//	Mode = 2, Run in LDROM
	dbg_printf("Mode = 1, Run in APROM\n\r");
	dbg_printf("Mode = 2, Run in LDROM\n\r");
	dbg_printf("BOOT:0x%x\n\r", buffer[8]);


	PacketNumber += 2;
	return (buffer[8] | ((buffer[9] << 8) & 0xff00) | ((buffer[10] << 16) & 0xff0000) | ((buffer[11] << 24) & 0xff000000));
}

unsigned int ISP_COMMAND::READFW_VERSION_USB(void)
{
	clock_t start_time, end_time;
	float total_time = 0;

	unsigned char cmd[Package_Size] = {0xa6,0,0,0,
		(PacketNumber&0xff),((PacketNumber>>8)&0xff),((PacketNumber>>16)&0xff),((PacketNumber>>24)&0xff)};
	pUSB.WriteFile((unsigned char *)&cmd, sizeof(cmd), &Length, 2000);					
	start_time = clock(); /* mircosecond */  
	while(1)
	{
		pUSB.ReadFile(buffer, Package_Size, &Length, 2000);
		dbg_printf("package: 0x%x\n\r",buffer[4]);
		if((buffer[4]|((buffer[5]<<8)&0xff00)|((buffer[6]<<16)&0xff0000)|((buffer[7]<<24)&0xff000000))==(PacketNumber+1))
			break;

		end_time = clock(); 
		/* CLOCKS_PER_SEC is defined at time.h */ 
		if((end_time - start_time)>Time_Out_Value)
			return 0;

	}
	dbg_printf("fw version:0x%x\n\r",buffer[8]);
	dbg_printf("\n\r");
	PacketNumber+=2;
	return (buffer[8]|((buffer[9]<<8)&0xff00)|((buffer[10]<<16)&0xff0000)|((buffer[11]<<24)&0xff000000));
}


void ISP_COMMAND::Erase_APROM_DATA_USB(void)
{
	clock_t start_time, end_time;
	float total_time = 0;

	unsigned char cmd[Package_Size] = { 0xFA,0,0,0,
		(PacketNumber & 0xff),((PacketNumber >> 8) & 0xff),((PacketNumber >> 16) & 0xff),((PacketNumber >> 24) & 0xff) };
	pUSB.WriteFile((unsigned char *)&cmd, sizeof(cmd), &Length, 2000);
	start_time = clock(); /* mircosecond */
	while (1)
	{
		pUSB.ReadFile(buffer, Package_Size, &Length, 2000);
		dbg_printf("package: 0x%x\n\r", buffer[4]);
		if ((buffer[4] | ((buffer[5] << 8) & 0xff00) | ((buffer[6] << 16) & 0xff0000) | ((buffer[7] << 24) & 0xff000000)) == (PacketNumber + 1))
			break;

		end_time = clock();
		/* CLOCKS_PER_SEC is defined at time.h */
		if ((end_time - start_time) > Time_Out_Value)
			break;

	}
	PacketNumber += 2;	
}

void ISP_COMMAND::WRITE_APROM_DATA_USB(_TCHAR* temp, unsigned char w_length)
{
	clock_t start_time, end_time;
	float total_time = 0;

	unsigned char cmd[Package_Size] = { 0xFB,0,0,0,
		(PacketNumber & 0xff),((PacketNumber >> 8) & 0xff),((PacketNumber >> 16) & 0xff),((PacketNumber >> 24) & 0xff) };
	WordsCpy(cmd + 8, temp, w_length);
	pUSB.WriteFile((unsigned char *)&cmd, sizeof(cmd), &Length, 2000);
	start_time = clock(); /* mircosecond */
	while (1)
	{
		pUSB.ReadFile(buffer, Package_Size, &Length, 2000);
		dbg_printf("package: 0x%x\n\r", buffer[4]);
		if ((buffer[4] | ((buffer[5] << 8) & 0xff00) | ((buffer[6] << 16) & 0xff0000) | ((buffer[7] << 24) & 0xff000000)) == (PacketNumber + 1))
			break;

		end_time = clock();
		/* CLOCKS_PER_SEC is defined at time.h */
		if ((end_time - start_time) > Time_Out_Value)
			break;

	}
	PacketNumber += 2;
}

void ISP_COMMAND::UPDATE_RTC_USB(void)
{
	clock_t start_time, end_time;
	float total_time = 0;
	time_t tmpcal_ptr;
	struct tm *tmp_ptr = NULL;
	time(&tmpcal_ptr);
	tmp_ptr = localtime(&tmpcal_ptr);//转换成当地时间

	printf ("after localtime, the time is:%d.%d.%d ", (1900+tmp_ptr->tm_year), (1+tmp_ptr->tm_mon), tmp_ptr->tm_mday);
	printf("%d:%d:%d\n", tmp_ptr->tm_hour, tmp_ptr->tm_min, tmp_ptr->tm_sec);

	unsigned char cmd[Package_Size] = { 0xFC,0,0,0,
		(PacketNumber & 0xff),((PacketNumber >> 8) & 0xff),((PacketNumber >> 16) & 0xff),((PacketNumber >> 24) & 0xff),
                 tmp_ptr->tm_year,tmp_ptr->tm_mon+1,tmp_ptr->tm_mday,tmp_ptr->tm_hour,tmp_ptr->tm_min, tmp_ptr->tm_sec
              };
	pUSB.WriteFile((unsigned char *)&cmd, sizeof(cmd), &Length, 2000);
	start_time = clock(); /* mircosecond */
	while (1)
	{
		pUSB.ReadFile(buffer, Package_Size, &Length, 2000);
		dbg_printf("package: 0x%x\n\r", buffer[4]);
		if ((buffer[4] | ((buffer[5] << 8) & 0xff00) | ((buffer[6] << 16) & 0xff0000) | ((buffer[7] << 24) & 0xff000000)) == (PacketNumber + 1))
			break;

		end_time = clock();
		/* CLOCKS_PER_SEC is defined at time.h */
		if ((end_time - start_time) > Time_Out_Value)
			break;

	}
	PacketNumber += 2;	
}


void ISP_COMMAND::Erase_APROM_IMAGE_USB(void)
{
	clock_t start_time, end_time;
	float total_time = 0;

	unsigned char cmd[Package_Size] = { 0xFD,0,0,0,
		(PacketNumber & 0xff),((PacketNumber >> 8) & 0xff),((PacketNumber >> 16) & 0xff),((PacketNumber >> 24) & 0xff) };
	pUSB.WriteFile((unsigned char *)&cmd, sizeof(cmd), &Length, 2000);
	start_time = clock(); /* mircosecond */
	while (1)
	{
		pUSB.ReadFile(buffer, Package_Size, &Length, 2000);
		dbg_printf("package: 0x%x\n\r", buffer[4]);
		if ((buffer[4] | ((buffer[5] << 8) & 0xff00) | ((buffer[6] << 16) & 0xff0000) | ((buffer[7] << 24) & 0xff000000)) == (PacketNumber + 1))
			break;

		end_time = clock();
		/* CLOCKS_PER_SEC is defined at time.h */
		if ((end_time - start_time) > Time_Out_Value)
			break;

	}
	PacketNumber += 2;	
}

#pragma pack(push, 1) // Ensure structure alignment
typedef struct {
	uint16_t type;           // 文件类型，必须为 'BM'
	uint32_t size;           // 文件大小
	uint16_t reserved1;      // 保留字段
	uint16_t reserved2;      // 保留字段
	uint32_t offset;         // 图像数据起始位置
	uint32_t header_size;    // 文件头大小
	int32_t  width;          // 图像宽度
	int32_t  height;         // 图像高度
	uint16_t planes;         // 颜色平面数，必须为1
	uint16_t bpp;            // 每像素位数
	uint32_t compression;    // 压缩方式，0 表示不压缩
	uint32_t img_size;       // 图像数据大小
	int32_t  x_ppm;          // 水平分辨率
	int32_t  y_ppm;          // 垂直分辨率
	uint32_t color_used;     // 使用的颜色数
	uint32_t color_important;// 重要的颜色数
} BMPHeader;
#pragma pack(pop)
ISP_STATE ISP_COMMAND::WRITE_APROM_IMAGE_USB(_TCHAR* temp)
{
	clock_t start_time, end_time;
	float total_time = 0;


	FILE *file = fopen(temp, "rb"); // 以二进制只读方式打开文件
	if (file == NULL) {
		printf("Error opening file");
		return RES_FILE_NO_FOUND;
	}

	BMPHeader header;
	fread(&header, sizeof(header), 1, file); // 读取文件头信息

	// 验证是否为 BMP 文件
	if (header.type != 0x4D42) {
		fprintf(stderr, "Not a BMP file\n");
		fclose(file);
		return RES_FILE_NO_FOUND;
	}

	// 打印图像信息
	printf("Width: %d\n", header.width);
	printf("Height: %d\n", header.height);
	printf("Bits per pixel: %d\n", header.bpp);
	if ((header.width !=8 )|| (header.height != 8))
		return RES_FILE_NO_FOUND;
	// 计算每行像素字节数
	int bytes_per_pixel = header.bpp / 8;
	int row_size = header.width / 8; // 单色图像中每个像素只占用一个位
	if (header.width % 8 != 0)
		row_size++; // 如果图像宽度不能被 8 整除，则需要向上取整

	// 分配内存来存储像素数据
	unsigned char *data = (unsigned char*)malloc(4 * header.height);
	if (data == NULL) {
		printf("Memory allocation failed");
		fclose(file);
		return RES_FILE_NO_FOUND;
	}

	// 读取像素数据
	fseek(file, header.offset, SEEK_SET);
	fread(data, 4 * header.height, 1, file);
	for (int i = 0; i < 32; i++)
		printf("%d=0x%x\n\r", i, data[i]);

	//to do
	unsigned char cmd[Package_Size] = { 0xFE,0,0,0,
		(PacketNumber & 0xff),((PacketNumber >> 8) & 0xff),((PacketNumber >> 16) & 0xff),((PacketNumber >> 24) & 0xff),
		data[0],data[4],data[8],data[12],data[16],data[20],data[24],data[28]
	};
	//WordsCpy(cmd + 8, temp, w_length);
	pUSB.WriteFile((unsigned char *)&cmd, sizeof(cmd), &Length, 2000);
	start_time = clock(); /* mircosecond */
	while (1)
	{
		pUSB.ReadFile(buffer, Package_Size, &Length, 2000);
		dbg_printf("package: 0x%x\n\r", buffer[4]);
		if ((buffer[4] | ((buffer[5] << 8) & 0xff00) | ((buffer[6] << 16) & 0xff0000) | ((buffer[7] << 24) & 0xff000000)) == (PacketNumber + 1))
			break;

		end_time = clock();
		/* CLOCKS_PER_SEC is defined at time.h */
		if ((end_time - start_time) > Time_Out_Value)
			break;

	}
	PacketNumber += 2;
	// 释放内存并关闭文件
	
	free(data);
	// 在这里可以使用像素数据进行进一步处理
	fclose(file);
	return RES_SN_OK;
}
unsigned int ISP_COMMAND::CHECK_BOOT_USB(void)
{
	clock_t start_time, end_time;
	float total_time = 0;

	unsigned char cmd[Package_Size] = { 0xca,0,0,0,
		(PacketNumber & 0xff),((PacketNumber >> 8) & 0xff),((PacketNumber >> 16) & 0xff),((PacketNumber >> 24) & 0xff) };
	pUSB.WriteFile((unsigned char *)&cmd, sizeof(cmd), &Length, 2000);
	start_time = clock(); /* mircosecond */
	while (1)
	{
		pUSB.ReadFile(buffer, Package_Size, &Length, 2000);
		dbg_printf("package: 0x%x\n\r", buffer[4]);
		if ((buffer[4] | ((buffer[5] << 8) & 0xff00) | ((buffer[6] << 16) & 0xff0000) | ((buffer[7] << 24) & 0xff000000)) == (PacketNumber + 1))
			break;

		end_time = clock();
		/* CLOCKS_PER_SEC is defined at time.h */
		if ((end_time - start_time) > Time_Out_Value)
			return 0;

	}
	//Mode = 1, Run in APROM
//	Mode = 2, Run in LDROM
	dbg_printf("Mode = 1, Run in APROM\n\r");
	dbg_printf("Mode = 2, Run in LDROM\n\r");
	dbg_printf("BOOT:0x%x\n\r", buffer[8]);
	
	PacketNumber += 2;

	return (buffer[8]);
}


void ISP_COMMAND::RUN_TO_APROM_USB(void)
{
	unsigned char cmd[Package_Size] = {0xab,0,0,0,
		(PacketNumber&0xff),((PacketNumber>>8)&0xff),((PacketNumber>>16)&0xff),((PacketNumber>>24)&0xff)};
	pUSB.WriteFile((unsigned char *)&cmd, sizeof(cmd), &Length, 2000);					
	PacketNumber+=2;
}

void ISP_COMMAND::RUN_TO_LDROM_USB(void)
{
	unsigned char cmd[Package_Size] = { 0xac,0,0,0,
		(PacketNumber & 0xff),((PacketNumber >> 8) & 0xff),((PacketNumber >> 16) & 0xff),((PacketNumber >> 24) & 0xff) };
	pUSB.WriteFile((unsigned char *)&cmd, sizeof(cmd), &Length, 2000);
	PacketNumber += 2;
}

void ISP_COMMAND::RUN_TO_APROM_UART(void)
{
	unsigned char cmd[Package_Size] = {0xab,0,0,0,
	(PacketNumber&0xff),((PacketNumber>>8)&0xff),((PacketNumber>>16)&0xff),((PacketNumber>>24)&0xff)};
	WriteFile(m_hCom,cmd,Package_Size,&iBytesWritten,NULL);						
	PacketNumber+=2;
}

void ISP_COMMAND::RUN_TO_LDROM_UART(void)
{
	unsigned char cmd[Package_Size] = { 0xac,0,0,0,
	(PacketNumber & 0xff),((PacketNumber >> 8) & 0xff),((PacketNumber >> 16) & 0xff),((PacketNumber >> 24) & 0xff) };
	WriteFile(m_hCom, cmd, Package_Size, &iBytesWritten, NULL);
	PacketNumber += 2;
}
ISP_STATE ISP_COMMAND::READ_PID_USB(void)
{
	clock_t start_time, end_time;
	float total_time = 0; 
	unsigned char cmd[Package_Size] = {0xB1,0,0,0,
		(PacketNumber&0xff),((PacketNumber>>8)&0xff),((PacketNumber>>16)&0xff),((PacketNumber>>24)&0xff)};	                    
	pUSB.WriteFile((unsigned char *)&cmd, sizeof(cmd), &Length, 2000);					
	start_time = clock(); /* mircosecond */ 
	while(1)
	{
		pUSB.ReadFile(buffer, Package_Size, &Length, 2000);
		dbg_printf("package: 0x%x\n\r",buffer[4]);
		if((buffer[4]|((buffer[5]<<8)&0xff00)|((buffer[6]<<16)&0xff0000)|((buffer[7]<<24)&0xff000000))==(PacketNumber+1))
			break;

		end_time = clock(); 
		/* CLOCKS_PER_SEC is defined at time.h */ 
		if((end_time - start_time)>Time_Out_Value)
			return RES_TIME_OUT;
	}
	dbg_printf("pid: 0x%x\n\r",(buffer[8]|((buffer[9]<<8)&0xff00)|((buffer[10]<<16)&0xff0000)|((buffer[11]<<24)&0xff000000)));
	dbg_printf("\n\r");
	PacketNumber+=2;
	//return (buffer[8]|((buffer[9]<<8)&0xff00)|((buffer[10]<<16)&0xff0000)|((buffer[11]<<24)&0xff000000));
	unsigned int temp_PDID=buffer[8]|((buffer[9]<<8)&0xff00)|((buffer[10]<<16)&0xff0000)|((buffer[11]<<24)&0xff000000);
		printf("Pid=x%x\n\r", temp_PDID);
#if 0
	CPartNumItem temp;

	int i=0,j=(sizeof(g_PartNumItems)/sizeof(g_PartNumItems[0]));
	while(1){
		temp=g_PartNumItems[i];
		if(temp_PDID==temp.PID)
		{
			printf("Part number: %s\n\r",g_PartNumItems[i].PartNumber);
			printf("APROM SIZE: %dKB\n\r",g_PartNumItems[i].APROM);
			printf("LDROM SIZE: %dKB\n\r",g_PartNumItems[i].LDROM);
			printf("DATAFLASH SIZE: %dKB\n\r",g_PartNumItems[i].DATAFLASH);
			APROM_SIZE=g_PartNumItems[i].APROM;
			LDROM_SIZE=g_PartNumItems[i].LDROM;
			DATAFLASH_SIZE=g_PartNumItems[i].DATAFLASH;		
			break;
		}
		i++;
		if(i == j)
			return RES_FALSE;		
	}
#endif
   return RES_PASS;
}

ISP_STATE ISP_COMMAND::READ_PID_UART(void)
{
	clock_t start_time, end_time;
	float total_time = 0; 
	unsigned char cmd[Package_Size] = {0xB1,0,0,0,
		(PacketNumber&0xff),((PacketNumber>>8)&0xff),((PacketNumber>>16)&0xff),((PacketNumber>>24)&0xff)};	                    
	WriteFile(m_hCom,cmd,Package_Size,&iBytesWritten,NULL);						
	start_time = clock(); /* mircosecond */ 
	while(1)
	{
		ReadFile(m_hCom, &buffer,Package_Size, &iBytesRead, NULL);
		dbg_printf("package: 0x%x\n\r",buffer[4]);
		if((buffer[4]|((buffer[5]<<8)&0xff00)|((buffer[6]<<16)&0xff0000)|((buffer[7]<<24)&0xff000000))==(PacketNumber+1))
			break;

		end_time = clock(); 
		/* CLOCKS_PER_SEC is defined at time.h */ 
		if((end_time - start_time)>Time_Out_Value)
			return RES_TIME_OUT;
	}
	dbg_printf("pid: 0x%x\n\r",(buffer[8]|((buffer[9]<<8)&0xff00)|((buffer[10]<<16)&0xff0000)|((buffer[11]<<24)&0xff000000)));
	dbg_printf("\n\r");
	PacketNumber+=2;
	//return (buffer[8]|((buffer[9]<<8)&0xff00)|((buffer[10]<<16)&0xff0000)|((buffer[11]<<24)&0xff000000));
	unsigned int temp_PDID=buffer[8]|((buffer[9]<<8)&0xff00)|((buffer[10]<<16)&0xff0000)|((buffer[11]<<24)&0xff000000);
	printf("Pid=x%x\n\r", temp_PDID);
	/*
	CPartNumItem temp;

	int i=0,j=(sizeof(g_PartNumItems)/sizeof(g_PartNumItems[0]));
	while(1){
		temp=g_PartNumItems[i];
		if(temp_PDID==temp.PID)
		{
			printf("Part number: %s\n\r",g_PartNumItems[i].PartNumber);
			printf("APROM SIZE: %dKB\n\r",g_PartNumItems[i].APROM);
			printf("LDROM SIZE: %dKB\n\r",g_PartNumItems[i].LDROM);
			printf("DATAFLASH SIZE: %dKB\n\r",g_PartNumItems[i].DATAFLASH);
			APROM_SIZE=g_PartNumItems[i].APROM;
			LDROM_SIZE=g_PartNumItems[i].LDROM;
			DATAFLASH_SIZE=g_PartNumItems[i].DATAFLASH;		
			break;
		}
		i++;
		if(i == j)
			return RES_FALSE;		
	}
	*/
   return RES_PASS;
}
ISP_STATE ISP_COMMAND::SN_PACKAGE_USB(void)
{
	clock_t start_time, end_time;
	float total_time = 0;

	unsigned char cmd1[Package_Size] = {0xa4,0,0,0,
		(PacketNumber&0xff),((PacketNumber>>8)&0xff),((PacketNumber>>16)&0xff),((PacketNumber>>24)&0xff),
		(PacketNumber&0xff),((PacketNumber>>8)&0xff),((PacketNumber>>16)&0xff),((PacketNumber>>24)&0xff)};
	pUSB.WriteFile((unsigned char *)&cmd1, sizeof(cmd1), &Length, 2000);		
	start_time = clock(); /* mircosecond */ 
	while(1)
	{
		pUSB.ReadFile(buffer, Package_Size, &Length, 2000);
		dbg_printf("package: 0x%x\n\r",buffer[4]);
		if((buffer[4]|((buffer[5]<<8)&0xff00)|((buffer[6]<<16)&0xff0000)|((buffer[7]<<24)&0xff000000))==(PacketNumber+1))
			break;
		end_time = clock(); 
		/* CLOCKS_PER_SEC is defined at time.h */ 
		if((end_time - start_time)>Time_Out_Value)
			return RES_TIME_OUT;
	}
	PacketNumber+=2;
	return RES_SN_OK;
}

void ISP_COMMAND::READ_CONFIG_USB(void)
{
	clock_t start_time, end_time;
	float total_time = 0;

	unsigned char cmd[Package_Size] = {0xa2,0, 0, 0, 
		(PacketNumber&0xff),((PacketNumber>>8)&0xff),((PacketNumber>>16)&0xff),((PacketNumber>>24)&0xff)};
	pUSB.WriteFile((unsigned char *)&cmd, sizeof(cmd), &Length, 2000);	
	start_time = clock(); /* mircosecond */ 
	while(1)
	{
		pUSB.ReadFile(buffer, Package_Size, &Length, 2000);
		dbg_printf("package: 0x%x\n\r",buffer[4]);
		if((buffer[4]|((buffer[5]<<8)&0xff00)|((buffer[6]<<16)&0xff0000)|((buffer[7]<<24)&0xff000000))==(PacketNumber+1))
			break;
		end_time = clock(); 
		/* CLOCKS_PER_SEC is defined at time.h */ 
		if((end_time - start_time)>Time_Out_Value)
		{
			printf("Time out\n\r");
			break;
		}
	}
	printf("config0: 0x%x\n\r",(buffer[8]|((buffer[9]<<8)&0xff00)|((buffer[10]<<16)&0xff0000)|((buffer[11]<<24)&0xff000000)));
	printf("config1: 0x%x\n\r",(buffer[12]|((buffer[13]<<8)&0xff00)|((buffer[14]<<16)&0xff0000)|((buffer[15]<<24)&0xff000000)));
	printf("\n\r");
	PacketNumber+=2;
}


void ISP_COMMAND::READ_CONFIG_UART(void)
{
	clock_t start_time, end_time;
	float total_time = 0;

	unsigned char cmd[Package_Size] = { 0xa2, 0, 0, 0,
		(PacketNumber & 0xff), ((PacketNumber >> 8) & 0xff), ((PacketNumber >> 16) & 0xff), ((PacketNumber >> 24) & 0xff) };
	WriteFile(m_hCom, cmd, Package_Size, &iBytesWritten, NULL);
	start_time = clock(); /* mircosecond */
	while (1)
	{
		ReadFile(m_hCom, &buffer, Package_Size, &iBytesRead, NULL);
		dbg_printf("package: 0x%x\n\r", buffer[4]);
		if ((buffer[4] | ((buffer[5] << 8) & 0xff00) | ((buffer[6] << 16) & 0xff0000) | ((buffer[7] << 24) & 0xff000000)) == (PacketNumber + 1))
			break;
		end_time = clock();
		/* CLOCKS_PER_SEC is defined at time.h */
		if ((end_time - start_time) > Time_Out_Value)
		{
			printf("Time out\n\r");
			break;
		}
	}
	printf("config0: 0x%x\n\r", (buffer[8] | ((buffer[9] << 8) & 0xff00) | ((buffer[10] << 16) & 0xff0000) | ((buffer[11] << 24) & 0xff000000)));
	printf("config1: 0x%x\n\r", (buffer[12] | ((buffer[13] << 8) & 0xff00) | ((buffer[14] << 16) & 0xff0000) | ((buffer[15] << 24) & 0xff000000)));
	printf("\n\r");
	PacketNumber += 2;
}




ISP_STATE ISP_COMMAND::Copy_BIN_Array(void)
{
	file_size = array_size;

	for (unsigned int i = 0; i < file_size; i++)
	{
		W_APROM_BUFFER[i] = APROM_array[i];
	}
	file_checksum = 0;
	for (unsigned int i = 0; i < file_size; i++)
	{

		file_checksum = file_checksum + W_APROM_BUFFER[i];
	}

	return RES_FILE_LOAD;
}


ISP_STATE ISP_COMMAND::File_Open_APROM(_TCHAR* temp)
{
	FILE *fp;
	file_size=0;
	if((fp=fopen(temp,"rb"))==NULL)
	{
		dbg_printf("APROM FILE OPEN FALSE\n\r");
		return RES_FILE_NO_FOUND;
	}	
	if(fp!=NULL)
	{
		while(!feof(fp)) {	
			fread(&W_APROM_BUFFER[file_size], sizeof(char), 1, fp);                
			file_size++;	
		}
	}

	file_size=file_size-1;	
	fclose(fp);

	//if(file_size>APROM_SIZE)
		//return RES_FILE_SIZE_OVER;

	file_checksum = 0;
	for (unsigned int i = 0; i < file_size; i++)
	{
		
		file_checksum = file_checksum + W_APROM_BUFFER[i];
	}

	return RES_FILE_LOAD;
}

void ISP_COMMAND::APROM_AND_CHECKSUM(void)
{
	APROM_SIZE = baprom_size;
	//write checksum in aprom buffer
	W_APROM_BUFFER[(APROM_SIZE)-8] = (file_size >> 0) & 0xff;
	W_APROM_BUFFER[(APROM_SIZE)-7] = (file_size >> 8) & 0xff;
	W_APROM_BUFFER[(APROM_SIZE)-6] = (file_size >> 16) & 0xff;
	W_APROM_BUFFER[(APROM_SIZE)-5] = (file_size >> 24) & 0xff;
	W_APROM_BUFFER[(APROM_SIZE)-4] = (file_checksum >> 0) & 0xff;
	W_APROM_BUFFER[(APROM_SIZE)-3] = (file_checksum >> 8) & 0xff;
	W_APROM_BUFFER[(APROM_SIZE)-2] = (file_checksum >> 16) & 0xff;
	W_APROM_BUFFER[(APROM_SIZE)-1] = (file_checksum >> 24) & 0xff;

	//for protocol check
	file_checksum_temp = 0;
	for (int i = 0; i < APROM_SIZE; i++)
	{
		file_checksum_temp += W_APROM_BUFFER[i];
	}
	file_checksum_temp = file_checksum_temp & 0xffff;
	file_size = APROM_SIZE;
	file_checksum = file_checksum_temp; //for all aprom 
}

ISP_STATE ISP_COMMAND::UPDATE_APROM_UART(void)
{
	clock_t start_time, end_time;
	float total_time = 0;
	unsigned int count=0; 
	unsigned short get_cksum;
	unsigned long cmdData=0, startaddr=0;
	memset(send_buf, 0, Package_Size);
	cmdData = CMD_UPDATE_APROM;//CMD_UPDATE_APROM
	WordsCpy(send_buf + 0, &cmdData, 4);
	WordsCpy(send_buf + 4, &PacketNumber, 4);
	startaddr = 0;
	WordsCpy(send_buf + 8, &startaddr, 4);
	WordsCpy(send_buf + 12, &file_size, 4);
	WordsCpy(send_buf + 16, W_APROM_BUFFER + 0, 48);
    WriteFile(m_hCom, send_buf, Package_Size, &iBytesWritten, NULL);
	while (1)
	{
		ReadFile(m_hCom, &buffer, Package_Size, &iBytesRead, NULL);
		dbg_printf("package: 0x%x\n\r", buffer[4]);
		if ((buffer[4] | ((buffer[5] << 8) & 0xff00) | ((buffer[6] << 16) & 0xff0000) | ((buffer[7] << 24) & 0xff000000)) == (PacketNumber + 1))
			break;

	}
	PacketNumber = PacketNumber + 2;
	printf("erase down");
	for (unsigned int i = 48; i < file_size; i = i + 56)
	{


		printf("Process=%.2f \r", (float)((float)i / (float)file_size) * 100);

		//clear buffer
		for (unsigned int j = 0; j < 64; j++)
		{
			send_buf[j] = 0;
		}

		WordsCpy(send_buf + 4, &PacketNumber, 4);
		if ((file_size - i) > 56)
		{
			WordsCpy(send_buf + 8, W_APROM_BUFFER + i, 56);
			//read check  package
			WriteFile(m_hCom, send_buf, Package_Size, &iBytesWritten, NULL);
			while (1)
			{
				ReadFile(m_hCom, &buffer, Package_Size, &iBytesRead, NULL);
				dbg_printf("package: 0x%x\n\r", buffer[4]);
				if ((buffer[4] | ((buffer[5] << 8) & 0xff00) | ((buffer[6] << 16) & 0xff0000) | ((buffer[7] << 24) & 0xff000000)) == (PacketNumber + 1))
				{

					break;
				}
				else
				{

					printf("error\n\r");
					return RES_FALSE;
				}
			}
		}
		else
		{

			//fread(&send_buf[8], sizeof(char), file_size - i, fp);
			WordsCpy(send_buf + 8, W_APROM_BUFFER + i, file_size - i);
			WriteFile(m_hCom, send_buf, Package_Size, &iBytesWritten, NULL);
			while (1)
			{
				ReadFile(m_hCom, &buffer, Package_Size, &iBytesRead, NULL);
				dbg_printf("package: 0x%x\n\r", buffer[4]);
				if ((buffer[4] | ((buffer[5] << 8) & 0xff00) | ((buffer[6] << 16) & 0xff0000) | ((buffer[7] << 24) & 0xff000000)) == (PacketNumber + 1))
				{

					break;
				}
				else
				{

					printf("error\n\r");
					return RES_FALSE;
				}
			}
#if 1
			WordsCpy(&get_cksum, buffer + 8, 2);
			printf("get_cksum: 0x%x\n\r", get_cksum);
			if ((file_checksum & 0xffff) != get_cksum)	
			//if ((file_checksum_temp & 0xffff) != get_cksum)
			{
				
				printf("check sum false\n\r");
				goto out1;
			}
#endif 
		}
		PacketNumber = PacketNumber + 2;
	}
	printf("\r                                ");
	printf("\r program progrss: 100%% \n\r");
	return RES_PASS;

out1:
	printf("\r program false!! \n\r");
	return RES_FALSE;
}

ISP_STATE ISP_COMMAND::UPDATE_APROM_USB(void)
{
	unsigned short get_cksum;
	clock_t start_time, end_time;
	float total_time = 0;
	unsigned int count = 0;
	unsigned long cmdData = 0, startaddr = 0;
	memset(send_buf, 0, Package_Size);
	cmdData = CMD_UPDATE_APROM;//CMD_UPDATE_APROM
	WordsCpy(send_buf + 0, &cmdData, 4);
	WordsCpy(send_buf + 4, &PacketNumber, 4);
	startaddr = 0;
	WordsCpy(send_buf + 8, &startaddr, 4);
	WordsCpy(send_buf + 12, &file_size, 4);
	WordsCpy(send_buf + 16, W_APROM_BUFFER + 0, 48);
	//WriteFile(m_hCom, send_buf, Package_Size, &iBytesWritten, NULL);
	pUSB.WriteFile((unsigned char *)&send_buf, sizeof(send_buf), &Length, 2000);
	start_time = clock(); /* mircosecond */
	while (1)
	{
		//ReadFile(m_hCom, &buffer, Package_Size, &iBytesRead, NULL);
		pUSB.ReadFile(buffer, Package_Size, &Length, 2000);
		dbg_printf("package: 0x%x\n\r", buffer[4]);
		if ((buffer[4] | ((buffer[5] << 8) & 0xff00) | ((buffer[6] << 16) & 0xff0000) | ((buffer[7] << 24) & 0xff000000)) == (PacketNumber + 1))
			break;
		end_time = clock();
		//if ((end_time - start_time) > Time_Out_Value)
		//{
		//	printf("Time out\n\r");
		//	return RES_TIME_OUT;
		//}

	}
	PacketNumber = PacketNumber + 2;
	printf("erase down\n\r");
	for (unsigned int i = 48; i < file_size; i = i + 56)
	{


		printf("Process=%.2f \r", (float)((float)i / (float)file_size) * 100);

		//clear buffer
		for (unsigned int j = 0; j < 64; j++)
		{
			send_buf[j] = 0;
		}

		WordsCpy(send_buf + 4, &PacketNumber, 4);
		if ((file_size - i) > 56)
		{
			WordsCpy(send_buf + 8, W_APROM_BUFFER + i, 56);
			//read check  package
			//WriteFile(m_hCom, send_buf, Package_Size, &iBytesWritten, NULL);
			pUSB.WriteFile((unsigned char *)&send_buf, sizeof(send_buf), &Length, 2000);
			while (1)
			{
				//ReadFile(m_hCom, &buffer, Package_Size, &iBytesRead, NULL);
				pUSB.ReadFile(buffer, Package_Size, &Length, 2000);
				dbg_printf("package: 0x%x\n\r", buffer[4]);
				if ((buffer[4] | ((buffer[5] << 8) & 0xff00) | ((buffer[6] << 16) & 0xff0000) | ((buffer[7] << 24) & 0xff000000)) == (PacketNumber + 1))
				{

					break;
				}
				else
				{

					printf("error\n\r");
					return RES_FALSE;
				}
			}
		}
		else
		{

			//fread(&send_buf[8], sizeof(char), file_size - i, fp);
			WordsCpy(send_buf + 8, W_APROM_BUFFER + i, file_size - i);
			//WriteFile(m_hCom, send_buf, Package_Size, &iBytesWritten, NULL);
			pUSB.WriteFile((unsigned char *)&send_buf, sizeof(send_buf), &Length, 2000);
			while (1)
			{
				//ReadFile(m_hCom, &buffer, Package_Size, &iBytesRead, NULL);
				pUSB.ReadFile(buffer, Package_Size, &Length, 2000);
				dbg_printf("package: 0x%x\n\r", buffer[4]);
				if ((buffer[4] | ((buffer[5] << 8) & 0xff00) | ((buffer[6] << 16) & 0xff0000) | ((buffer[7] << 24) & 0xff000000)) == (PacketNumber + 1))
				{

					break;
				}
				else
				{

					printf("error\n\r");
					return RES_FALSE;
				}
			}
#if 1
			WordsCpy(&get_cksum, buffer + 8, 2);
			printf("get_cksum: 0x%x\n\r", get_cksum);
			if ((file_checksum & 0xffff) != get_cksum)
				//if ((file_checksum_temp & 0xffff) != get_cksum)
			{

				printf("check sum false\n\r");
				goto out1;
				}
#endif 
		}
		PacketNumber = PacketNumber + 2;
	}
	printf("\r                                ");
	printf("\r program progrss: 100%% \n\r");
	return RES_PASS;

out1:
	printf("\r program false!! \n\r");
	return RES_FALSE;
}




ISP_COMMAND::ISP_COMMAND(void)
{
	COM_OPEN_FLAG=0;;
	USB_OPEN_FLAG=0;
	Address=0;
	PacketNumber=1;
	APROM_SIZE=0;
	LDROM_SIZE=0;
	DATAFLASH_SIZE=0;
}


ISP_COMMAND::~ISP_COMMAND(void)
{

}

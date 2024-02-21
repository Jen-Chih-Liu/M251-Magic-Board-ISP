#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#define baprom_size 63*1024
typedef enum {
	RES_FALSE = 0,		
	RES_PASS,		    
	RES_FILE_NO_FOUND,
    RES_PROGRAM_FALSE,
	RES_CONNECT,
	RES_CONNECT_FALSE,
	RES_CHIP_FOUND,
	RES_DISCONNECT,
	RES_FILE_LOAD,
	RES_FILE_SIZE_OVER,
	RES_TIME_OUT,
	RES_SN_OK,
	RES_DETECT_MCU,
	RES_NO_DETECT,
} ISP_STATE;

class ISP_COMMAND
{


private:
	HANDLE	m_hCom;
	DCB		m_dcb;
	DWORD iBytesWritten;
	DWORD iBytesRead;
	COMMTIMEOUTS m_CommTimeouts;
		//发送报告用的OVERLAPPED。
	OVERLAPPED WriteOverlapped;
	//接收报告用的OVERLAPPED。
	OVERLAPPED ReadOverlapped;
public:
	unsigned char W_APROM_BUFFER[baprom_size];
    unsigned int file_size;
	unsigned int file_checksum;
	unsigned int file_checksum_temp;
	unsigned int USB_OPEN_FLAG;
	unsigned int COM_OPEN_FLAG;
	char P_NUMBER[255];
    unsigned int APROM_SIZE;
    unsigned int LDROM_SIZE;
    unsigned int DATAFLASH_SIZE;
public:

	void READ_CONFIG_UART(void);
	void READ_CONFIG_USB(void);
	unsigned int CHECK_BOOT_USB(void);
	unsigned int CHECK_BOOT_UART(void);
	unsigned int READFW_VERSION_UART(void);
	unsigned int READFW_VERSION_USB(void);
	void APROM_AND_CHECKSUM(void);
	ISP_STATE Copy_BIN_Array(void);
	ISP_STATE OPEN_COMPORT(_TCHAR* temp);
	ISP_STATE OPEN_COMPORT(void);
	ISP_STATE OPEN_USBPORT(void);

	ISP_STATE CLOSE_UART_PORT(void);
	ISP_STATE CLOSE_USB_PORT(void);

	ISP_STATE READ_PID_UART(void);
	ISP_STATE READ_PID_USB(void);

	ISP_STATE SN_PACKAGE_UART(void);
	ISP_STATE SN_PACKAGE_USB(void);

	ISP_STATE File_Open_APROM(_TCHAR* temp);
	void WRITE_APROM_DATA_USB(_TCHAR* temp,unsigned char w_length);
	void Erase_APROM_DATA_USB(void);
	ISP_STATE WRITE_APROM_IMAGE_USB(_TCHAR* temp);
	void Erase_APROM_IMAGE_USB(void);
	void UPDATE_RTC_USB(void);
	ISP_STATE UPDATE_APROM_UART(void);		
	ISP_STATE UPDATE_APROM_USB(void);
	void RUN_TO_APROM_UART(void);
	void RUN_TO_APROM_USB(void);
	void RUN_TO_LDROM_UART(void);
	void RUN_TO_LDROM_USB(void);
	//ISP_STATE UPDATE_DATAFLASH(void);
	ISP_STATE CHECK_UART_LINK(void);
	ISP_COMMAND(void);
	~ISP_COMMAND(void);
};


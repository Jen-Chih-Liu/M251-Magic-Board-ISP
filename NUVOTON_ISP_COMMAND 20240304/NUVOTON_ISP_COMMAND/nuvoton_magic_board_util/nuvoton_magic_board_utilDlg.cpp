
// nuvoton_magic_board_utilDlg.cpp : implementation file
//

#define  _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "nuvoton_magic_board_util.h"
#include "nuvoton_magic_board_utilDlg.h"
#include "afxdialogex.h"
#include <io.h>  
#include <fcntl.h>
#include "stdint.h"
#include "ISP_COMMAND.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CnuvotonmagicboardutilDlg dialog



CnuvotonmagicboardutilDlg::CnuvotonmagicboardutilDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NUVOTON_MAGIC_BOARD_UTIL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CnuvotonmagicboardutilDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CnuvotonmagicboardutilDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CnuvotonmagicboardutilDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CnuvotonmagicboardutilDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CnuvotonmagicboardutilDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON8, &CnuvotonmagicboardutilDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON4, &CnuvotonmagicboardutilDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON7, &CnuvotonmagicboardutilDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON2, &CnuvotonmagicboardutilDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON6, &CnuvotonmagicboardutilDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CnuvotonmagicboardutilDlg message handlers

BOOL CnuvotonmagicboardutilDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	this->SetWindowText("NUVOTON MAGIC BOARD UTIL");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CnuvotonmagicboardutilDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CnuvotonmagicboardutilDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CnuvotonmagicboardutilDlg::OnClose()
{
	Sleep(1);
	DestroyWindow();
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CnuvotonmagicboardutilDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CnuvotonmagicboardutilDlg::OnBnClickedButton1()
{
	TRACE("printf test\n\r");
	CEdit* pEdit;
	pEdit = (CEdit *)GetDlgItem(IDC_EDIT1);
	CString str;
	pEdit->GetWindowTextA(str);
	TRACE("%s\n\r",str);
	if (strlen(str) == 0) {
		TRACE("string is empty\n");
		AfxMessageBox(_T("string is empty, please input char in the box"));
		return;
	}
	

	if (strlen(str) > 48) {
		TRACE("string over 48\n");
		AfxMessageBox(_T("string over 48"));
		return;
	}

	ISP_COMMAND *ISP = new ISP_COMMAND();
#if 1
	if (ISP->OPEN_USBPORT() != RES_CONNECT)
	{
		TRACE("USB NO FOUND\n\r");
		AfxMessageBox(_T("USB NO FOUND"));
		goto USB_EXIT;
	}

	//START ;
	ISP->SN_PACKAGE_USB();
	ISP->SN_PACKAGE_USB();

	if (ISP->CHECK_BOOT_USB() != 1)
	{
		TRACE("it is boot in ldrom\n\r");
		AfxMessageBox(_T("Please check boot in APROM"));
		goto USB_EXIT;
	}

	_TCHAR user_String[256]; 
	_tcscpy(user_String, str);
	ISP->WRITE_APROM_DATA_USB(user_String, strlen(str));
	ISP->UPDATE_RTC_USB();
USB_EXIT:
	//close usb port
	ISP->CLOSE_USB_PORT();
#endif
	delete ISP;
}


void CnuvotonmagicboardutilDlg::OnBnClickedButton3()
{

	TCHAR szFilter[] = _T("bin file(*.bin)|*.bin||");
 
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
	CString strFilePath;

 
	if (IDOK == fileDlg.DoModal())
	{		   
		strFilePath = fileDlg.GetPathName();
		TRACE(strFilePath);
	}
	else {
		return;
	}
	ISP_COMMAND *ISP = new ISP_COMMAND();

	if (ISP->OPEN_USBPORT() != RES_CONNECT)
	{
		TRACE("USB NO FOUND\n\r");
		AfxMessageBox(_T("USB NO FOUND"));
		goto USB_EXIT;
	}
	//START ;
	ISP->SN_PACKAGE_USB();
	ISP->SN_PACKAGE_USB();

	_TCHAR filename[256]; 
	_tcscpy(filename, strFilePath);

	if (ISP->File_Open_APROM(filename) == RES_FILE_NO_FOUND)
	{
		
		TRACE("FILE NO FOUND\n\r");
		delete ISP;
		return ;
	}
	TRACE("File name: %s\n\r", filename);
	TRACE("File size: %d\n\r", ISP->file_size);
	TRACE("File checksum: %d\n\r", ISP->file_checksum);

	if (ISP->file_size > aprom_size_t)
	{
		TRACE("bin file over flash size\n\r");
		delete ISP;
		return ;
	}

	printf("File size: 0x%x\n\r", ISP->file_size);
	printf("File checksum: 0x%x\n\r", ISP->file_checksum);

ISP->APROM_AND_CHECKSUM();
ISP->file_size = 128 * 1024;
TRACE("aprom with check sum inside File size: %d\n\r", ISP->file_size);
TRACE("aprom with check sun inside File checksum: %d\n\r", ISP->file_checksum);

//CHECK FW
TRACE("FW version: 0x%x \n\r", ISP->READFW_VERSION_USB());
//CHECK PID
if (ISP->READ_PID_USB() == RES_FALSE)
{
	TRACE("CHIP NO FOUND\n\r");
	goto USB_EXIT;
}
if (ISP->CHECK_BOOT_USB() != 2)
{
	TRACE("it is boot in aprom\n\r");
	AfxMessageBox(_T("Please check boot in LDROM"));
	goto USB_EXIT;
}
//READ CONFIG
TRACE("config \n\r");
ISP->READ_CONFIG_USB();
CStatic* pStatic;
pStatic = (CStatic *)GetDlgItem(IDC_STATICISP);
pStatic->SetWindowTextA("Do ISP Command");
//test updata aprom
if (ISP->UPDATE_APROM_USB()== RES_PASS)
pStatic->SetWindowTextA("ISP Down");
else
pStatic->SetWindowTextA("ISP False");
//reboot mcu to aprom
ISP->RUN_TO_APROM_USB();
USB_EXIT:
//close usb port
ISP->CLOSE_USB_PORT();

delete ISP;

}


void CnuvotonmagicboardutilDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	ISP_COMMAND *ISP = new ISP_COMMAND();
#if 1
	if (ISP->OPEN_USBPORT() != RES_CONNECT)
	{
		TRACE("USB NO FOUND\n\r");
		AfxMessageBox(_T("USB NO FOUND"));
		goto USB_EXIT;
	}
	//START ;
	ISP->SN_PACKAGE_USB();
	ISP->SN_PACKAGE_USB();
	ISP->RUN_TO_LDROM_USB();

USB_EXIT:
	//close usb port
	ISP->CLOSE_USB_PORT();
#endif
	delete ISP;

	// TODO: Add your control notification handler code here
}


void CnuvotonmagicboardutilDlg::OnBnClickedButton8()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	ISP_COMMAND *ISP = new ISP_COMMAND();
#if 1
	if (ISP->OPEN_USBPORT() != RES_CONNECT)
	{
		TRACE("USB NO FOUND\n\r");
		AfxMessageBox(_T("USB NO FOUND"));
		goto USB_EXIT;
	}
	//START ;
	ISP->SN_PACKAGE_USB();
	ISP->SN_PACKAGE_USB();
	ISP->RUN_TO_LDROM_USB();

USB_EXIT:
	//close usb port
	ISP->CLOSE_USB_PORT();
#endif
	delete ISP;
}


void CnuvotonmagicboardutilDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	ISP_COMMAND *ISP = new ISP_COMMAND();
#if 1
	if (ISP->OPEN_USBPORT() != RES_CONNECT)
	{
		TRACE("USB NO FOUND\n\r");
		AfxMessageBox(_T("USB NO FOUND"));
		goto USB_EXIT;
	}

	//START ;
	ISP->SN_PACKAGE_USB();
	ISP->SN_PACKAGE_USB();
	unsigned int temp = 0;
	temp = ISP->CHECK_BOOT_USB();
	TRACE("Mode = 1, Run in APROM\n\r");
	TRACE("Mode = 2, Run in LDROM\n\r");
	TRACE("BOOT:0x%x\n\r", temp);
	CStatic* pStatic;
	pStatic = (CStatic *)GetDlgItem(IDC_STATICBOOT);
	if (temp==1)
		pStatic->SetWindowTextA("Run in APROM");
	if (temp==2)
		pStatic->SetWindowTextA("Run in LDROM");
USB_EXIT:
	//close usb port
	ISP->CLOSE_USB_PORT();
#endif
	delete ISP;

}


void CnuvotonmagicboardutilDlg::OnBnClickedButton7()
{
	CString val = "";
	// TODO: Add your control notification handler code here
	ISP_COMMAND *ISP = new ISP_COMMAND();
#if 1
	if (ISP->OPEN_USBPORT() != RES_CONNECT)
	{
		TRACE("USB NO FOUND\n\r");
		AfxMessageBox(_T("USB NO FOUND"));
		goto USB_EXIT;
	}

	//START ;
	ISP->SN_PACKAGE_USB();
	ISP->SN_PACKAGE_USB();


	unsigned int temp = ISP->READFW_VERSION_USB();
	TRACE("FW version: 0x%x \n\r", temp);
	CStatic* pStatic;
	pStatic = (CStatic *)GetDlgItem(IDC_STATICVERSION);

	val.Format(_T("0x%x"), temp);
	pStatic->SetWindowTextA(val);
USB_EXIT:
	//close usb port
	ISP->CLOSE_USB_PORT();
#endif
	delete ISP;
}

#pragma pack(push, 1) // Ensure structure alignment
typedef struct {
	uint16_t type;           
	uint32_t size;           
	uint16_t reserved1;      
	uint16_t reserved2;      
	uint32_t offset;         
	uint32_t header_size;    
	int32_t  width;          
	int32_t  height;         
	uint16_t planes;         
	uint16_t bpp;            
	uint32_t compression;    
	uint32_t img_size;       
	int32_t  x_ppm;          
	int32_t  y_ppm;          
	uint32_t color_used;     
	uint32_t color_important;
} BMPHeader;
#pragma pack(pop)
void CnuvotonmagicboardutilDlg::OnBnClickedButton2()
{
	TCHAR szFilter[] = _T("bmp file(*.bmp)|*.bmp||");

	CFileDialog fileDlg(TRUE, _T("open image  8x8 file"), NULL, 0, szFilter, this);
	CString strFilePath;

	if (IDOK == fileDlg.DoModal())
	{
		strFilePath = fileDlg.GetPathName();
		TRACE(strFilePath);
	}
	else {
		return;
	}
	FILE *file = fopen(strFilePath, "rb"); 
	if (file == NULL) {
		TRACE("Error opening file");
		AfxMessageBox(_T("Load 8x8 bmp file false"));
		return;
	}

	BMPHeader header;
	fread(&header, sizeof(header), 1, file); 


	if (header.type != 0x4D42) {
		fprintf(stderr, "Not a BMP file\n");
		fclose(file);
		AfxMessageBox(_T("Load 8x8 bmp file false"));
		return ;
	}

	
	TRACE("Width: %d\n", header.width);
	TRACE("Height: %d\n", header.height);
	TRACE("Bits per pixel: %d\n", header.bpp);
	if ((header.width != 8) || (header.height != 8))
	{
		AfxMessageBox(_T("Load 8x8 bmp file false"));
		return;
	}

	int bytes_per_pixel = header.bpp / 8;
	int row_size = header.width / 8; 
	if (header.width % 8 != 0)
		row_size++; 


	unsigned char *data = (unsigned char*)malloc(4 * header.height);
	if (data == NULL) {
		TRACE("Memory allocation failed");
		fclose(file);
		AfxMessageBox(_T("Load bmp file false"));
		return ;
	}

	fseek(file, header.offset, SEEK_SET);
	fread(data, 4 * header.height, 1, file);
	//for (int i = 0; i < 32; i++)
		//TRACE("%d=0x%x\n\r", i, data[i]);
	unsigned char tempdata[8] = { 0 };
	tempdata[0] = ~data[0];
	tempdata[1] = ~data[4];
	tempdata[2] = ~data[8];
	tempdata[3] = ~data[12];
	tempdata[4] = ~data[16],
	tempdata[5] = ~data[20];
	tempdata[6] = ~data[24];
	tempdata[7] = ~data[28];

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			BOOL bCheck = (tempdata[i] & (1 << j)) != 0; 
			UINT nIDC = IDC_CHECK1 + i * 8 + j; 
			((CButton*)GetDlgItem(nIDC))->SetCheck(bCheck); 
		}
	}

#if 0
	if ((tempdata[0]&0x01)==0x01)
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(TRUE);
	else
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(FALSE);
	if ((tempdata[0] & 0x02) == 0x02)
		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(FALSE);
#endif 
#if 0
	BOOL itisChecked=false;
	itisChecked=IsDlgButtonChecked(IDC_CHECK1);
	TRACE("%d", itisChecked);
#endif
	// TODO: Add your control notification handler code here
}


void CnuvotonmagicboardutilDlg::OnBnClickedButton6()
{
	unsigned char tempdata[8];

	for (int i = 0; i < 8; ++i)
	{
		tempdata[i] = 0; 
	}

	for (int i = 0; i < 64; ++i)
	{
		UINT nIDC = IDC_CHECK1 + i; 

		BOOL bCheck = ((CButton*)GetDlgItem(nIDC))->GetCheck(); // ??checkbox??

		if (bCheck)
		{
			int index = i / 8;
			int bit = i % 8;

			tempdata[index] |= (1 << bit); 
		}
	}
	for (int i = 0; i < 8; i++)
		TRACE("%d=0x%x\n\r", i, tempdata[i]);
	// TODO: Add your control notification handler code here
	ISP_COMMAND *ISP = new ISP_COMMAND();
#if 1
	if (ISP->OPEN_USBPORT() != RES_CONNECT)
	{
		TRACE("USB NO FOUND\n\r");
		AfxMessageBox(_T("USB NO FOUND"));
		goto USB_EXIT;
	}

	//START ;
	ISP->SN_PACKAGE_USB();
	ISP->SN_PACKAGE_USB();
	if (ISP->CHECK_BOOT_USB() != 1)
	{
		TRACE("it is boot in ldrom\n\r");
		AfxMessageBox(_T("Please check boot in APROM"));
		goto USB_EXIT;
	}
	ISP->WRITE_APROM_IMG_RAWDATA_USB(tempdata,8);
	ISP->UPDATE_RTC_USB();
USB_EXIT:
	//close usb port
	ISP->CLOSE_USB_PORT();
#endif
	delete ISP;
}

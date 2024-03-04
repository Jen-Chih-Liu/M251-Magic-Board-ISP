
// nuvoton_magic_board_util.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CnuvotonmagicboardutilApp:
// See nuvoton_magic_board_util.cpp for the implementation of this class
//

class CnuvotonmagicboardutilApp : public CWinApp
{
public:
	CnuvotonmagicboardutilApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CnuvotonmagicboardutilApp theApp;

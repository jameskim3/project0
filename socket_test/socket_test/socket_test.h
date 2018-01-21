
// socket_test.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Csocket_testApp:
// See socket_test.cpp for the implementation of this class
//

class Csocket_testApp : public CWinApp
{
public:
	Csocket_testApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Csocket_testApp theApp;
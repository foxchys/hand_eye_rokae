
// touch_ball.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Ctouch_ballApp:
// See touch_ball.cpp for the implementation of this class
//

class Ctouch_ballApp : public CWinApp
{
public:
	Ctouch_ballApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Ctouch_ballApp theApp;
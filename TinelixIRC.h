// TinelixIRC.h : main header file for the TINELIXIRC application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// IRCClient:
// See TinelixIRC.cpp for the implementation of this class
//

class IRCClient : public CWinApp
{
public:
	IRCClient();
	char version[64];
	char version_number[6];
	char stabillity_level[6];
	char build_date[32];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(IRCClient)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(IRCClient)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

// TinelixIRC.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TinelixIRC.h"
#include "MainWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// IRCClient

BEGIN_MESSAGE_MAP(IRCClient, CWinApp)
	//{{AFX_MSG_MAP(IRCClient)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// IRCClient construction

IRCClient::IRCClient()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only IRCClient object

IRCClient theApp;

/////////////////////////////////////////////////////////////////////////////
// IRCClient initialization

BOOL IRCClient::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	sprintf(version, "0.2.7 Beta for Windows");
	sprintf(build_date, "January 8, 2022");
	sprintf(version_number, "0.2.7");
	sprintf(stabillity_level, "Beta");
	
	if (GetVersion() & 0x80000000 && (GetVersion() & 0xFF) ==3) {
		MessageBox(NULL, "This version of the program does not work on Windows 3.1x."
			"\r\nYou can download the 0.1.5 version at "
			"https://github.com/tinelix/irc-client-for-win32s/releases", "Tinelix IRC Client", 
			MB_ICONSTOP|MB_OK);
		return FALSE;
	};
	
	MainWindow dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

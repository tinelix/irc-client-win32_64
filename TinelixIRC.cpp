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

void IRCClient::delsymbs(char *str, int begin, int lng)
{
	for(begin; begin < strlen(str); begin++) {
		*(str + begin) = 0;	
	};
};


BOOL IRCClient::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	char exe_path[MAX_PATH] = {0};
	char dll_path[MAX_PATH] = {0};
	char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename

	GetModuleFileName(NULL, exe_path, MAX_PATH);
	GetModuleFileName(NULL, dll_path, MAX_PATH);

	IRCClient::delsymbs(exe_path, strlen(exe_path) - strlen(exe_name) - 1, strlen(exe_path) - strlen(exe_name) - 1); // deleting EXE filename
	IRCClient::delsymbs(dll_path, strlen(dll_path) - strlen(exe_name) - 1, strlen(dll_path) - strlen(exe_name) - 1); // deleting EXE filename

	strcat(exe_path, "\\settings.ini");	// add settings filename

	strcat(dll_path, "\\parser.dll");

	char language_string[MAX_PATH];

	GetPrivateProfileString("Main", "Language", "", language_string, MAX_PATH, exe_path);

	CString lng_selitemtext_2(language_string);
	if( GetVersion() & 0x80000000 && (GetVersion() & 0xFF) ==3) {
		if (lng_selitemtext_2 == "Russian") {
			MessageBox(NULL, "Данная версия программы не работает в Windows 3.1x."
				" Вы можете скачать версию для Win32s по адресу https://github.com/tinelix/irc-client-for-win32s", "Tinelix IRC Client", MB_ICONSTOP|MB_OK); 
		} if (lng_selitemtext_2 == "English") {
			MessageBox(NULL, "This version of the program does not work on Windows 3.1x."
				" You can download the version for Win32s at https://github.com/tinelix/irc-client-for-win32s", "Tinelix IRC Client", MB_ICONSTOP|MB_OK);
		};
		exit(0);
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

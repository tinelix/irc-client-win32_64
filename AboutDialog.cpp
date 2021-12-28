// AboutDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TinelixIRC.h"
#include "AboutDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AboutDialog dialog


AboutDialog::AboutDialog(CWnd* pParent /*=NULL*/)
	: CDialog(AboutDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(AboutDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void AboutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AboutDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AboutDialog, CDialog)
	//{{AFX_MSG_MAP(AboutDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AboutDialog message handlers

void AboutDialog::delsymbs(char *str, int begin, int lng)
{
	for(begin; begin < strlen(str); begin++) {
		*(str + begin) = 0;	
	};
};

BOOL AboutDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	IRCClient* application = (IRCClient*)AfxGetApp();
	
	char exe_path[MAX_PATH] = {0};
	char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename

	GetModuleFileName(NULL, exe_path, MAX_PATH);  

	AboutDialog::delsymbs(exe_path, strlen(exe_path) - strlen(exe_name) - 1, strlen(exe_path) - strlen(exe_name) - 1); // deleting EXE filename

	strcat(exe_path, "\\settings.ini");	// add settings filename

	TCHAR language_string[MAX_PATH] = {0};

	mainfont.CreateFont(8, 0, 0, 0, FW_REGULAR, FALSE, FALSE, 0, DEFAULT_CHARSET, 0, 0, 
	0, 0, "MS Sans Serif");
	boldfont.CreateFont(8, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, 0, 0, 
	0, 0, "MS Sans Serif");
	GetDlgItem(IDC_APPNAME)->SetFont(&boldfont);

	char app_name[128];

	sprintf(app_name, "Tinelix IRC Client %s %s", application->version_number, application->stabillity_level);

	GetDlgItem(IDC_APPNAME)->SetWindowText(app_name);

	try {
		GetPrivateProfileString("Main", "Language", "English", language_string, MAX_PATH, exe_path);

		CString lng_selitemtext_2(language_string);
		if(lng_selitemtext_2 == "Russian") {
			SetWindowText("О программе Tinelix IRC Client");
			GetDlgItem(IDC_WIN32_EDITION)->SetWindowText("для Windows");
			GetDlgItem(IDC_TECHINFO)->SetWindowText("Техническая информация");
			GetDlgItem(IDC_WINVER_LABEL)->SetWindowText("Версия Windows:");
			GetDlgItem(IDC_LICENSE_NOTIFICATION)->SetWindowText("Это свободное программное обеспечение с"
			" открытым исходным кодом, распространяемое с условиями GNU General Public License 3.0. Прочитайте"
			" на сайте https://www.gnu.org/licenses/gnu-3.0-ru.html или смотрите файл LICENSE.TXT.\n\nБЕЗ КАКИХ-ЛИБО"
			" ГАРАНТИЙ, ВКЛЮЧАЯ КОММЕРЧЕСКУЮ ЦЕННОСТЬ.");			
		} else {
			SetWindowText("About Tinelix IRC Client");
			GetDlgItem(IDC_WIN32_EDITION)->SetWindowText("for Windows");
			GetDlgItem(IDC_TECHINFO)->SetWindowText("Technical Information");
			GetDlgItem(IDC_WINVER_LABEL)->SetWindowText("Windows version:");
			GetDlgItem(IDC_LICENSE_NOTIFICATION)->SetWindowText("Windows version:");
			GetDlgItem(IDC_LICENSE_NOTIFICATION)->SetWindowText("This is free software with open source,"
			" distributed under the terms of the GNU General Public License 3.0. Read"
			" on https://www.gnu.org/licenses/gnu-3.0.html or watch LICENSE.TXT file.\n\nWITHOUT ANY"
			" WARRANTY, NOT EVEN FOR MERCHANTABILITY.");
		};
	} catch(...) {
	};
	
	char winver_info[400];
	
	int winver = GetVersion();
	sprintf(winver_info, "%d.%d", LOBYTE(LOWORD(winver)), 
	HIBYTE(LOWORD(winver)));
	GetDlgItem(IDC_WINVER)->SetWindowText(winver_info);	
	
	CDialog::OnInitDialog();
	CenterWindow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void AboutDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

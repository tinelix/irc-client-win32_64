// EditDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TinelixIRC.h"
#include "EditDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EditDialog dialog


EditDialog::EditDialog(CWnd* pParent /*=NULL*/)
	: CDialog(EditDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(EditDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void EditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EditDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EditDialog, CDialog)
	//{{AFX_MSG_MAP(EditDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EditDialog message handlers

void EditDialog::delsymbs(char *str, int begin, int lng)
{
	for(begin; begin < strlen(str); begin++) {
		*(str + begin) = 0;	
	};
};

void EditDialog::SetSettingProfileState(BOOL value) 
{
	setting_profile = value;
};

void EditDialog::SetProfileName(char* name) 
{
	CString profname(name);
	sprintf(profilename, "%s", profname);
};


BOOL EditDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void EditDialog::OnOK() 
{
	
	GetDlgItem(IDC_ENTERTEXTEDIT)->GetWindowText(profilename, 79);
	
	char exe_path[MAX_PATH] = {0};
	char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename
	char quit_msg[512];
	char show_infomsg[12];

	GetModuleFileName(NULL, exe_path, MAX_PATH);  

	*(strrchr(exe_path, '\\')+1)='\0';

	strcat(exe_path, "\\settings.ini");

	char language_string[MAX_PATH] = {0};
	GetPrivateProfileString("Main", "Language", "English", language_string, MAX_PATH, exe_path);
	GetPrivateProfileString("Main", "ShowInfoMessages", "", show_infomsg, MAX_PATH, exe_path);
	CString lng_selitemtext_2(language_string);
	IRCClient* application = (IRCClient*)AfxGetApp();
	
	sprintf(quit_msg, "Tinelix IRC Client ver. %s", application->version);

	WritePrivateProfileString(profilename, "Nickname", "", exe_path);
	WritePrivateProfileString(profilename, "ReserveNickname", "", exe_path);
	WritePrivateProfileString(profilename, "Password", "", exe_path);
	WritePrivateProfileString(profilename, "Realname", "", exe_path);
	WritePrivateProfileString(profilename, "Server", "", exe_path);
	WritePrivateProfileString(profilename, "Port", "", exe_path);
	WritePrivateProfileString(profilename, "QuitMessage", quit_msg, exe_path);
	WritePrivateProfileString(profilename, "HideIP", "Disabled", exe_path);

	if(strcmp(profilename, "Main") == 0 || strcmp(profilename, "Parser") == 0) {
		try {
			if(lng_selitemtext_2 == "Russian") {
				MessageBox("Нельзя создавать профиль с таким именем, так как имя \"Main\" и \"Parser\" уже зарезервировано для настроек клиента.", "Ошибка", MB_OK | MB_ICONSTOP);
			} else {
				MessageBox("You cannot create a profile with this name because \"Main\" and \"Parser\" is already reserved for client settings.", "Error", MB_OK | MB_ICONSTOP);
			};
		} catch(...) {
		
		};
		return;
	};	
	
	CDialog::OnOK();
}

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

	GetModuleFileName(NULL, exe_path, MAX_PATH);  

	EditDialog::delsymbs(exe_path, strlen(exe_path) - strlen(exe_name) - 1, strlen(exe_path) - strlen(exe_name) - 1); // deleting EXE filename

	strcat(exe_path, "\\settings.ini");

	char language_string[MAX_PATH] = {0};
	GetPrivateProfileString("Main", "Language", "English", language_string, MAX_PATH, exe_path);
	CString lng_selitemtext_2(language_string);

	WritePrivateProfileString(profilename, "Nickname", "", exe_path);
	WritePrivateProfileString(profilename, "ReserveNickname", "", exe_path);
	WritePrivateProfileString(profilename, "Password", "", exe_path);
	WritePrivateProfileString(profilename, "Realname", "", exe_path);
	WritePrivateProfileString(profilename, "Server", "", exe_path);
	WritePrivateProfileString(profilename, "Port", "", exe_path);
	


	if((UINT)ShellExecute(NULL, "open", "notepad.exe", exe_path, NULL, SW_SHOWNORMAL) <= 32) {
		try {
			if(lng_selitemtext_2 == "Russian") {
				MessageBox("Блокнот не найден. Воспользуйтесь другим редактором для изменения данного конфигурационного файла.", "Ошибка", MB_OK | MB_ICONSTOP);
			} else {
				MessageBox("Notepad not found. Use another editor to modify this configuration file.", "Error", MB_OK | MB_ICONSTOP);
			};
		} catch(...) {
		
		};
	};
	
	CDialog::OnOK();
}

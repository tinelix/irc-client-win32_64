// InfoMessageDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TinelixIRC.h"
#include "InfoMessageDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// InfoMessageDialog dialog


InfoMessageDialog::InfoMessageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(InfoMessageDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(InfoMessageDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void InfoMessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(InfoMessageDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(InfoMessageDialog, CDialog)
	//{{AFX_MSG_MAP(InfoMessageDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// InfoMessageDialog message handlers

BOOL InfoMessageDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	char exe_path[MAX_PATH] = {0};
	char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename

	GetModuleFileName(NULL, exe_path, MAX_PATH);  

	*(strrchr(exe_path, '\\')+1)='\0';	// add settings filename

	strcat(exe_path, "\\settings.ini");

	TCHAR language_string[MAX_PATH] = {0};

	GetPrivateProfileString("Main", "Language", "English", language_string, MAX_PATH, exe_path);

	CString language_string_2(language_string);

	if(language_string_2 == "Russian") {
		GetDlgItem(IDC_DONT_SHOW)->SetWindowText("Больше не показывать информационные сообщения");
	} else {
		GetDlgItem(IDC_DONT_SHOW)->SetWindowText("Don't show information messages again");
	};
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void InfoMessageDialog::OnOK() 
{
	char exe_path[MAX_PATH] = {0};
	char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename

	GetModuleFileName(NULL, exe_path, MAX_PATH);  

	*(strrchr(exe_path, '\\')+1)='\0';	// add settings filename

	strcat(exe_path, "\\settings.ini");

	CButton* dont_show_infomsg_cb = (CButton*)GetDlgItem(IDC_DONT_SHOW);
	if(dont_show_infomsg_cb->GetCheck() == 1) {
		WritePrivateProfileString("Main", "ShowInfoMessages", "Disabled", exe_path);
	};
	GetParent()->SendMessage(WM_CLOSE_INFOMSG, (WPARAM)infomsg_code, NULL);
	
	CDialog::OnOK();
}

void InfoMessageDialog::SetInfoMessageCode(int code) {
	infomsg_code = code;
};

// StatisticsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TinelixIRC.h"
#include "StatisticsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// StatisticsDialog dialog


StatisticsDialog::StatisticsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(StatisticsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(StatisticsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void StatisticsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StatisticsDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(StatisticsDialog, CDialog)
	//{{AFX_MSG_MAP(StatisticsDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// StatisticsDialog message handlers

void StatisticsDialog::delsymbs(char *str, int begin, int lng)
{
	for(begin; begin < strlen(str); begin++) {
		*(str + begin) = 0;	
	};
};

BOOL StatisticsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	char exe_path[MAX_PATH] = {0};
	char dll_path[MAX_PATH] = {0};
	char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename
	TCHAR language_string[MAX_PATH] = {0};
	
	GetModuleFileName(NULL, exe_path, MAX_PATH);
	GetModuleFileName(NULL, dll_path, MAX_PATH);

	StatisticsDialog::delsymbs(exe_path, strlen(exe_path) - strlen(exe_name) - 1, strlen(exe_path) - strlen(exe_name) - 1); // deleting EXE filename
	StatisticsDialog::delsymbs(dll_path, strlen(dll_path) - strlen(exe_name) - 1, strlen(dll_path) - strlen(exe_name) - 1); // deleting EXE filename

	strcat(exe_path, "\\settings.ini");	// add settings filename

	strcat(dll_path, "\\parser.dll");

	GetPrivateProfileString("Main", "Language", "", language_string, MAX_PATH, exe_path);

	CString lng_selitemtext_2(language_string);

	int total_bytes_count = sended_bytes_count + recieved_bytes_count;
	
	if(lng_selitemtext_2 == "Russian") {
		SetWindowText("Статистика");
		GetDlgItem(IDC_TRAFFIC_GROUP)->SetWindowText("Трафик");
		GetDlgItem(IDC_SENDED_LABEL)->SetWindowText("Отправлено:");
		GetDlgItem(IDC_RECIEVED_LABEL)->SetWindowText("Получено:");
		GetDlgItem(IDC_TOTAL_LABEL)->SetWindowText("Итого:");
		char sended_bytes_value[128];
		char recieved_bytes_value[128];
		char total_bytes_value[128];
		if(sended_bytes_count >= 1048576) {
			sprintf(sended_bytes_value, "%.2f МБ", (double)(sended_bytes_count) / 1048576);
		} else if(sended_bytes_count >= 1024) {
			sprintf(sended_bytes_value, "%.2f кБ", (double)(sended_bytes_count) / 1024);
		} else if(sended_bytes_count < 0) {
			sprintf(sended_bytes_value, "(неизвестно)");
		} else {
			sprintf(sended_bytes_value, "%d байт", sended_bytes_count);
		};
		if(recieved_bytes_count >= 1048576) {
			sprintf(recieved_bytes_value, "%.2f МБ", (double)(recieved_bytes_count) / 1048576);
		} else if(recieved_bytes_count >= 1024) {
			sprintf(recieved_bytes_value, "%.2f кБ", (double)(recieved_bytes_count) / 1024);
		} else if(recieved_bytes_count < 0) {
			sprintf(recieved_bytes_value, "(неизвестно)");
		} else {
			sprintf(recieved_bytes_value, "%d байт", recieved_bytes_count);
		};
		if(total_bytes_count >= 1048576) {
			sprintf(total_bytes_value, "%.2f МБ", (double)(total_bytes_count) / 1048576);
		} else if(total_bytes_count >= 1024) {
			sprintf(total_bytes_value, "%.2f кБ", (double)(total_bytes_count) / 1024);
		} else if(total_bytes_count < 0) {
			sprintf(total_bytes_value, "(неизвестно)");
		} else {
			sprintf(total_bytes_value, "%d байт", total_bytes_count);
		};
		GetDlgItem(IDC_SENDED_LABEL2)->SetWindowText(sended_bytes_value);
		GetDlgItem(IDC_RECIEVED_LABEL2)->SetWindowText(recieved_bytes_value);
		GetDlgItem(IDC_TOTAL_LABEL2)->SetWindowText(total_bytes_value);
	} else {
		SetWindowText("Statistics");
		GetDlgItem(IDC_TRAFFIC_GROUP)->SetWindowText("Traffic");
		GetDlgItem(IDC_SENDED_LABEL)->SetWindowText("Sended:");
		GetDlgItem(IDC_RECIEVED_LABEL)->SetWindowText("Recieved:");
		GetDlgItem(IDC_TOTAL_LABEL)->SetWindowText("Total:");
		char sended_bytes_value[128];
		char recieved_bytes_value[128];
		char total_bytes_value[128];
		if(sended_bytes_count >= 1048576) {
			sprintf(sended_bytes_value, "%.2f MB", (double)(sended_bytes_count) / 1048576);
		} else if(sended_bytes_count >= 1024) {
			sprintf(sended_bytes_value, "%.2f kB", (double)(sended_bytes_count) / 1024);
		} else if(sended_bytes_count < 0) {
			sprintf(sended_bytes_value, "(unknown)");
		} else {
			sprintf(sended_bytes_value, "%d bytes", sended_bytes_count);
		};
		if(recieved_bytes_count >= 1048576) {
			sprintf(recieved_bytes_value, "%.2f MB", (double)(recieved_bytes_count) / 1048576);
		} else if(recieved_bytes_count >= 1024) {
			sprintf(recieved_bytes_value, "%.2f kB", (double)(recieved_bytes_count) / 1024);
		} else if(recieved_bytes_count < 0) {
			sprintf(recieved_bytes_value, "(unknown)");
		} else {
			sprintf(recieved_bytes_value, "%d bytes", recieved_bytes_count);
		};
		if(total_bytes_count >= 1048576) {
			sprintf(total_bytes_value, "%.2f MB", (double)(total_bytes_count) / 1048576);
		} else if(total_bytes_count >= 1024) {
			sprintf(total_bytes_value, "%.2f kB", (double)(total_bytes_count) / 1024);
		} else if(total_bytes_count < 0) {
			sprintf(total_bytes_value, "(unknown)");
		} else {
			sprintf(total_bytes_value, "%d bytes", total_bytes_count);
		};
		GetDlgItem(IDC_SENDED_LABEL2)->SetWindowText(sended_bytes_value);
		GetDlgItem(IDC_RECIEVED_LABEL2)->SetWindowText(recieved_bytes_value);
		GetDlgItem(IDC_TOTAL_LABEL2)->SetWindowText(total_bytes_value);
	};
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT StatisticsDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_UPDATING_STATISTICS) {
		IRC_STATS* irc_stats = (IRC_STATS*)lParam;
		sended_bytes_count = irc_stats->sended_bytes;
		recieved_bytes_count = irc_stats->recieved_bytes;

		char exe_path[MAX_PATH] = {0};
		char dll_path[MAX_PATH] = {0};
		char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename
		TCHAR language_string[MAX_PATH] = {0};
	
		GetModuleFileName(NULL, exe_path, MAX_PATH);
		GetModuleFileName(NULL, dll_path, MAX_PATH);

		StatisticsDialog::delsymbs(exe_path, strlen(exe_path) - strlen(exe_name) - 1, strlen(exe_path) - strlen(exe_name) - 1); // deleting EXE filename
		StatisticsDialog::delsymbs(dll_path, strlen(dll_path) - strlen(exe_name) - 1, strlen(dll_path) - strlen(exe_name) - 1); // deleting EXE filename

		strcat(exe_path, "\\settings.ini");	// add settings filename

		strcat(dll_path, "\\parser.dll");

		GetPrivateProfileString("Main", "Language", "", language_string, MAX_PATH, exe_path);

		CString lng_selitemtext_2(language_string);

		int total_bytes_count = sended_bytes_count + recieved_bytes_count;

		if(lng_selitemtext_2 == "Russian") {
			SetWindowText("Статистика");
			GetDlgItem(IDC_TRAFFIC_GROUP)->SetWindowText("Трафик");
			GetDlgItem(IDC_SENDED_LABEL)->SetWindowText("Отправлено:");
			GetDlgItem(IDC_RECIEVED_LABEL)->SetWindowText("Получено:");
			GetDlgItem(IDC_TOTAL_LABEL)->SetWindowText("Итого:");
			char sended_bytes_value[128];
			char recieved_bytes_value[128];
			char total_bytes_value[128];
			if(sended_bytes_count >= 1048576) {
				sprintf(sended_bytes_value, "%.2f МБ", (double)(sended_bytes_count) / 1048576);
			} else if(sended_bytes_count >= 1024) {
				sprintf(sended_bytes_value, "%.2f кБ", (double)(sended_bytes_count) / 1024);
			} else if(sended_bytes_count < 0) {
				sprintf(sended_bytes_value, "(неизвестно)", (double)(sended_bytes_count) / 1024);
			} else {
				sprintf(sended_bytes_value, "%d байт", sended_bytes_count);
			};
			if(recieved_bytes_count >= 1048576) {
				sprintf(recieved_bytes_value, "%.2f МБ", (double)(recieved_bytes_count) / 1048576);
			} else if(recieved_bytes_count >= 1024) {
				sprintf(recieved_bytes_value, "%.2f кБ", (double)(recieved_bytes_count) / 1024);
			} else if(recieved_bytes_count < 0) {
				sprintf(recieved_bytes_value, "(неизвестно)");
			} else {
				sprintf(recieved_bytes_value, "%d байт", recieved_bytes_count);
			};
			if(total_bytes_count >= 1048576) {
				sprintf(total_bytes_value, "%.2f МБ", (double)(total_bytes_count) / 1048576);
			} else if(total_bytes_count >= 1024) {
				sprintf(total_bytes_value, "%.2f кБ", (double)(total_bytes_count) / 1024);
			} else if(total_bytes_count < 0) {
				sprintf(total_bytes_value, "(неизвестно)");
			} else {
				sprintf(total_bytes_value, "%d байт", total_bytes_count);
			};
			GetDlgItem(IDC_SENDED_LABEL2)->SetWindowText(sended_bytes_value);
			GetDlgItem(IDC_RECIEVED_LABEL2)->SetWindowText(recieved_bytes_value);
			GetDlgItem(IDC_TOTAL_LABEL2)->SetWindowText(total_bytes_value);
		} else {
			SetWindowText("Statistics");
			GetDlgItem(IDC_TRAFFIC_GROUP)->SetWindowText("Traffic");
			GetDlgItem(IDC_SENDED_LABEL)->SetWindowText("Sended:");
			GetDlgItem(IDC_RECIEVED_LABEL)->SetWindowText("Recieved:");
			GetDlgItem(IDC_TOTAL_LABEL)->SetWindowText("Total:");
			char sended_bytes_value[128];
			char recieved_bytes_value[128];
			char total_bytes_value[128];
			if(sended_bytes_count >= 1048576) {
				sprintf(sended_bytes_value, "%.2f MB", (double)(sended_bytes_count) / 1048576);
			} else if(sended_bytes_count >= 1024) {
				sprintf(sended_bytes_value, "%.2f kB", (double)(sended_bytes_count) / 1024);
			} else {
				sprintf(sended_bytes_value, "%d bytes", sended_bytes_count);
			};
			if(recieved_bytes_count >= 1048576) {
				sprintf(recieved_bytes_value, "%.2f MB", (double)(recieved_bytes_count) / 1048576);
			} else if(recieved_bytes_count >= 1024) {
				sprintf(recieved_bytes_value, "%.2f kB", (double)(recieved_bytes_count) / 1024);
			} else {
				sprintf(recieved_bytes_value, "%d bytes", recieved_bytes_count);
			};
			if(total_bytes_count >= 1048576) {
				sprintf(total_bytes_value, "%.2f MB", (double)(total_bytes_count) / 1048576);
			} else if(total_bytes_count >= 1024) {
				sprintf(total_bytes_value, "%.2f kB", (double)(total_bytes_count) / 1024);
			} else {
				sprintf(total_bytes_value, "%d bytes", total_bytes_count);
			};
			GetDlgItem(IDC_SENDED_LABEL2)->SetWindowText(sended_bytes_value);
			GetDlgItem(IDC_RECIEVED_LABEL2)->SetWindowText(recieved_bytes_value);
			GetDlgItem(IDC_TOTAL_LABEL2)->SetWindowText(total_bytes_value);
		};
	};
	
	return CDialog::WindowProc(message, wParam, lParam);
}

// MainWindow.cpp : implementation file
//

#include "stdafx.h"
#include "TinelixIRC.h"
#include "MainWindow.h"
#include "ConnectionManagerDialog.h"
#include "SettingsDialog.h"
#include "AboutDialog.h"

#include <stdio.h>
#include <iostream.h>
#include <winsock.h>
#include <process.h>
#include <direct.h>
#include <locale.h>
#include <afxtempl.h>
#include <conio.h>

#pragma comment(lib, "wsock32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef void (WINAPI *cfunc) ();
typedef void (WINAPI *ParseMessage)(char*, char*[], char*, BOOL, char*);
typedef void (WINAPI *ParseNamesMessage)(char*, char*[], char*[], char*[], char*[], int, int, int, BOOL);

/////////////////////////////////////////////////////////////////////////////
// MainWindow dialog

MainWindow::MainWindow(CWnd* pParent /*=NULL*/)
	: CDialog(MainWindow::IDD, pParent)
{
	//{{AFX_DATA_INIT(MainWindow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void MainWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MainWindow)
	DDX_Control(pDX, IDC_IRC_CHAT_TABS, m_irc_tabs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MainWindow, CDialog)
	//{{AFX_MSG_MAP(MainWindow)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_CONNECT, OnFileConnect)
	ON_COMMAND(ID_VIEW_SETTINGS, OnViewSettings)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(ID_FILE_QUIT, OnFileQuit)
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_STATISTICS, OnFileStatistics)
	ON_NOTIFY(TCN_SELCHANGE, IDC_IRC_CHAT_TABS, OnSelchangeIrcChatTabs)
	ON_NOTIFY(TCN_SELCHANGING, IDC_IRC_CHAT_TABS, OnSelchangingIrcChatTabs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MainWindow message handlers

SOCKET sock;

void MainWindow::delsymbs(char *str, int begin, int lng)
{
	for(begin; begin < strlen(str); begin++) {
		*(str + begin) = 0;	
	};
};

BOOL MainWindow::OnInitDialog() 
{
	CDialog::OnInitDialog();

	channels_count = 0;
	
	IRCClient* application = (IRCClient*)AfxGetApp();
	
	HICON m_hIcon = application->LoadIcon(IDR_MAINFRAME);

	SetIcon(m_hIcon, TRUE);
	
	irc_chat_page = new IRCChatPage;
	
	stats_dlg = new StatisticsDialog;
	stats_dlg->Create(StatisticsDialog::IDD, this);

	info_msg_dlg = new InfoMessageDialog;

	char exe_path[MAX_PATH] = {0};
	char dll_path[MAX_PATH] = {0};
	char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename

	TCHAR language_string[MAX_PATH] = {0};
	TCHAR mini2tray_string[MAX_PATH] = {0};
	TCHAR showinfomsg_string[MAX_PATH] = {0};

	GetModuleFileName(NULL, exe_path, MAX_PATH);
	GetModuleFileName(NULL, dll_path, MAX_PATH);

	*(strrchr(exe_path, '\\')+1)='\0';
	*(strrchr(dll_path, '\\')+1)='\0';

	strcat(exe_path, "\\settings.ini");	// add settings filename

	strcat(dll_path, "\\parser.dll");

	GetPrivateProfileString("Main", "Language", "", language_string, MAX_PATH, exe_path);
	GetPrivateProfileString("Main", "MinimizeToTray", "", mini2tray_string, MAX_PATH, exe_path);
	GetPrivateProfileString("Main", "ShowInfoMessages", "", showinfomsg_string, MAX_PATH, exe_path);

	CString lng_selitemtext_2(language_string);
	CString mini2tray_string_2(mini2tray_string);
	CString showinfomsg_string_2(showinfomsg_string);

	if(mini2tray_string_2 == "Enabled") {
		TrayMessage(NIM_ADD);
	} else if (mini2tray_string_2 == "") {
		WritePrivateProfileString("Main", "MinimizeToTray", "Disabled", exe_path);
	} else {
		TrayMessage(NIM_DELETE);
	};

	if (showinfomsg_string_2 == "") {
		WritePrivateProfileString("Main", "ShowInfoMessages", "Enabled", exe_path);
	};

	parsing_array = new char*[32768];
	for (int array_index = 0; array_index < sizeof(parsing_array); array_index++) {
		parsing_array[array_index] = new char[32768];
	};

	owners_array = new char*[32768];
	for (int owners_array_index = 0; owners_array_index < sizeof(owners_array); owners_array_index++) {
		owners_array[owners_array_index] = new char[32768];
		sprintf(owners_array[owners_array_index], "");
	};

	operators_array = new char*[32768];
	for (int operators_array_index = 0; operators_array_index < sizeof(operators_array); operators_array_index++) {
		operators_array[operators_array_index] = new char[32768];
		sprintf(operators_array[operators_array_index], "");
	};

	members_array = new char*[32768];
	for (int members_array_index = 0; members_array_index < sizeof(members_array); members_array_index++) {
		members_array[members_array_index] = new char[32768];
		sprintf(members_array[members_array_index], "");
	};

	channels = new char*[1024];
	
	for (int ch_array_index = 0; ch_array_index < sizeof(channels); ch_array_index++) {
		channels[ch_array_index] = new char[128];
		sprintf(channels[ch_array_index], "");
	};
	

	TC_ITEM tci;
	tci.mask = TCIF_TEXT;
	tci.iImage = -1;
	if(lng_selitemtext_2 == "Russian") {
		tci.pszText = "Поток";
	} else {
		tci.pszText = "Thread";
	};
	m_irc_tabs.InsertItem(0, &tci);
	tci.mask = TCIF_PARAM;
	tci.lParam = (LPARAM)irc_chat_page;
	m_irc_tabs.SetItem(0, &tci);
	CRect rcClient, rcWindow;

	m_irc_tabs.GetClientRect(&rcClient);
	m_irc_tabs.AdjustRect(FALSE, &rcClient);
	m_irc_tabs.GetWindowRect(&rcWindow);
	ScreenToClient(rcWindow);

	sended_bytes_count = 0;
	recieved_bytes_count = 0;

	rcClient.OffsetRect(rcWindow.left, rcWindow.top);

	irc_chat_page->Create(IRCChatPage::IDD, &m_irc_tabs);

	for(int ch_array_index_2 = 0; ch_array_index_2 < 1024; ch_array_index_2++) {
		irc_channel_page[ch_array_index_2] = new IRCChannelPage;
		//irc_channel_page[ch_array_index_2]->Create(IRCChannelPage::IDD, &m_irc_tabs);
	};
	
	hwnd_chat = irc_chat_page->m_hWnd;
	irc_chat_page->MoveWindow(&rcClient, TRUE);
	int page_frame_w = rcWindow.Width() - 4;
	int page_frame_h = rcWindow.Height() - 23;
	irc_chat_page->SetWindowPos(NULL, 2, 21, page_frame_w, page_frame_h, SWP_NOZORDER);
	irc_chat_page->GetDlgItem(IDC_SOCKMSGS)->MoveWindow(2, 2, page_frame_w - 4, page_frame_h - 28);
	irc_chat_page->GetDlgItem(IDC_MSGTEXT)->MoveWindow(2, page_frame_h - 24, page_frame_w - 72, 22);
	irc_chat_page->GetDlgItem(IDC_SENDMSG)->MoveWindow(page_frame_w - 68, page_frame_h - 24, 66, 22);

	TRACE("Tinelix IRC Client ver. %s\r\nCopyright © 2021-2022 Dmitry Tretyakov (aka. Tinelix)\r\n"
	"https:/github.com/tinelix/irc-client-for-windows\r\n\r\n", application->version);

	char font_string[48] = {0};
	int font_size = 9;
	HDC hDC = ::GetDC(NULL);
	GetPrivateProfileString("Main", "MsgFont", "Fixedsys", font_string, 48, exe_path);
	font_size = GetPrivateProfileInt("Main", "MsgFontSize", 8, exe_path);
	int font_height = 0;
	font_height = -MulDiv(font_size, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	font.CreateFont(font_height, 0, 0, 0, FW_REGULAR, FALSE, FALSE, 0, DEFAULT_CHARSET, 0, 0,
	0, 0, font_string);
	mainfont.CreateFont(8, 0, 0, 0, FW_REGULAR, FALSE, FALSE, 0, DEFAULT_CHARSET, 0, 0,
	0, 0, "MS Sans Serif");
	irc_chat_page->GetDlgItem(IDC_SOCKMSGS)->SetFont(&font);
	IsConnected = FALSE;
	SetWindowText("Tinelix IRC Client");
	char buffer_size[40];
	

	if(!(GetVersion() & 0x80000000 && (GetVersion() & 0xFF) ==3)) {
		parserLib = (HINSTANCE)malloc(sizeof(HINSTANCE));
		parserLib = LoadLibrary(dll_path);
		if(parserLib) {
			TRACE("PARSER.DLL loaded.\r\n");
		};
	};

	CMenu* mainmenu;

	WSADATA WSAData;
	char szTemp[80];

	int status = (WSAStartup(MAKEWORD(1,1), &WSAData));

	//try {
		if (lng_selitemtext_2 == "Russian") {
			irc_chat_page->GetDlgItem(IDC_SENDMSG)->SetWindowText("Отправить");
			if (status == 0) {
				TRACE("Winsock initialed.\r\n");
    		}
    		else {
				MessageBox("WinSock не может быть инициализирован.", "Ошибка", MB_OK|MB_ICONSTOP);
    		};
			setlocale(LC_ALL, "Russian");
			mainmenu = (CMenu*)malloc(sizeof(CMenu));
			mainmenu->m_hMenu = NULL;
			mainmenu->CreateMenu();
			CMenu* file_submenu = (CMenu*)malloc(sizeof(CMenu));
			file_submenu->m_hMenu = NULL;
			file_submenu->CreatePopupMenu();
			file_submenu->AppendMenu(MF_STRING, ID_FILE_CONNECT, "Подключиться...");
			file_submenu->AppendMenu(MF_SEPARATOR);
			file_submenu->AppendMenu(MF_STRING, ID_FILE_STATISTICS, "Статистика");
			file_submenu->AppendMenu(MF_SEPARATOR);
			file_submenu->AppendMenu(MF_STRING, ID_FILE_QUIT, "Выход");
			CMenu* view_submenu = (CMenu*)malloc(sizeof(CMenu));
			view_submenu->m_hMenu = NULL;
			view_submenu->CreatePopupMenu();
			view_submenu->AppendMenu(MF_STRING, ID_VIEW_SETTINGS, "Настройки");
			CMenu* help_submenu = (CMenu*)malloc(sizeof(CMenu));
			help_submenu->m_hMenu = NULL;
			help_submenu->CreatePopupMenu();
			help_submenu->AppendMenu(MF_STRING, ID_HELP_ABOUT, "О программе...");
			mainmenu->AppendMenu(MF_STRING | MF_POPUP, (UINT)file_submenu->m_hMenu, "Файл");
            mainmenu->AppendMenu(MF_STRING | MF_POPUP, (UINT)view_submenu->m_hMenu, "Вид");
			mainmenu->AppendMenu(MF_STRING | MF_POPUP, (UINT)help_submenu->m_hMenu, "Справка");
			SetMenu(mainmenu);
			free(mainmenu);
			free(file_submenu);
			free(view_submenu);
			free(help_submenu);
			GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText("Готово");
		} if (lng_selitemtext_2 == "English") {
			irc_chat_page->GetDlgItem(IDC_SENDMSG)->SetWindowText("Send");
			if (status == 0) {
				TRACE("WinSock initialed.\r\n");
    		}
    		else {
				MessageBox("WinSock cannot be initialed.", "Error", MB_OK|MB_ICONSTOP);
    		};
			setlocale(LC_ALL, "English");
			mainmenu = (CMenu*)malloc(sizeof(CMenu));
			mainmenu->m_hMenu = NULL;
			mainmenu->LoadMenu(IDR_MAINMENU);
			SetMenu(mainmenu);
			free(mainmenu);
			GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText("Ready");
		} if (lng_selitemtext_2 == "") {
			irc_chat_page->GetDlgItem(IDC_SENDMSG)->SetWindowText("Send");
			if (status == 0) {
				TRACE("Winsock initialed.\r\n");
    		}
    		else {
				MessageBox("WinSock cannot be initialed.", "Error", MB_OK|MB_ICONSTOP);
    		};
			setlocale(LC_ALL, "English");
			mainmenu = (CMenu*)malloc(sizeof(CMenu));
			mainmenu->m_hMenu = NULL;
			mainmenu->LoadMenu(IDR_MAINMENU);
			SetMenu(mainmenu);
			free(mainmenu);
			WritePrivateProfileString("Main", "Language", "English", exe_path);
			WritePrivateProfileString("Main", "MsgHistory", "Enabled", exe_path);
			GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText("Ready");
		};

	irc_chat_page->ShowWindow(SW_SHOW);
	
	MoveWindow(0, 0, 512, 360);

	CenterWindow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MainWindow::OnSize(UINT nType, int cx, int cy) 
{
	char exe_path[MAX_PATH] = {0};
	char dll_path[MAX_PATH] = {0};
	char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename

	TCHAR language_string[MAX_PATH] = {0};
	TCHAR mini2tray_string[MAX_PATH] = {0};


	GetModuleFileName(NULL, exe_path, MAX_PATH);
	GetModuleFileName(NULL, dll_path, MAX_PATH);

	*(strrchr(exe_path, '\\')+1)='\0';
	*(strrchr(dll_path, '\\')+1)='\0';

	strcat(exe_path, "\\settings.ini");	// add settings filename

	strcat(dll_path, "\\parser.dll");

	GetPrivateProfileString("Main", "MinimizeToTray", "", mini2tray_string, MAX_PATH, exe_path);

	CString mini2tray_string_2(mini2tray_string);

	
	if(nType == SIZE_MINIMIZED && mini2tray_string_2 == "Enabled"){
		ShowWindow(SW_HIDE);
	}
	else{
		CDialog::OnSize(nType, cx, cy);
	};
	
	CRect rcClient, rcWindow;

	if(m_irc_tabs.m_hWnd == NULL || irc_chat_page->m_hWnd == NULL) {
		return;
	};
	GetClientRect(&rcClient);
	m_irc_tabs.SetWindowPos(NULL, 4, 4, cx - 8, cy - 30, SWP_NOZORDER);
	m_irc_tabs.AdjustRect(FALSE, &rcClient);

	m_irc_tabs.GetClientRect(&rcClient);
	m_irc_tabs.AdjustRect(FALSE, &rcClient);
	m_irc_tabs.GetWindowRect(&rcWindow);
	ScreenToClient(rcWindow);

	rcClient.OffsetRect(rcWindow.left, rcWindow.top);
	irc_chat_page->MoveWindow(&rcClient, TRUE);
	int page_frame_w = rcWindow.Width() - 4;
	int page_frame_h = rcWindow.Height() - 23;
	irc_chat_page->SetWindowPos(NULL, 2, 21, page_frame_w, page_frame_h, SWP_NOZORDER);
	irc_chat_page->GetDlgItem(IDC_SOCKMSGS)->MoveWindow(2, 2, page_frame_w - 4, page_frame_h - 28);
	irc_chat_page->GetDlgItem(IDC_MSGTEXT)->MoveWindow(2, page_frame_h - 24, page_frame_w - 72, 22);
	irc_chat_page->GetDlgItem(IDC_SENDMSG)->MoveWindow(page_frame_w - 68, page_frame_h - 24, 66, 22);
	AfxGetMainWnd()->GetDlgItem(IDC_STATUSBAR_TEXT)->MoveWindow(4, cy - 20, cx - 4, 22);

	TC_ITEM tci;
	tci.mask = TCIF_PARAM;

	for (int ch_array_index = 0; ch_array_index < sizeof(channels); ch_array_index++) {
		if(m_hWnd != NULL && irc_channel_page[ch_array_index]->m_hWnd != NULL) {
			m_irc_tabs.GetItem(ch_array_index + 1, &tci);
			IRCChannelPage* channel_page = (IRCChannelPage *)tci.lParam;
			channel_page->MoveWindow(&rcClient, TRUE);
			channel_page->SetWindowPos(NULL, 2, 21, page_frame_w, page_frame_h, SWP_NOZORDER);
			channel_page->GetDlgItem(IDC_SOCKMSGS)->MoveWindow(2, 2, page_frame_w - 112, page_frame_h - 28);
			channel_page->GetDlgItem(IDC_MEMBERS_LIST)->MoveWindow(page_frame_w - 108, 2, 106, page_frame_h - 28);
			channel_page->GetDlgItem(IDC_MSGTEXT)->MoveWindow(2, page_frame_h - 24, page_frame_w - 72, 22);
			channel_page->GetDlgItem(IDC_SENDMSG)->MoveWindow(page_frame_w - 68, page_frame_h - 24, 66, 22);
		};
	};
	
}

void MainWindow::OnDestroy() 
{
	CDialog::OnDestroy();
	
}


void MainWindow::OnFileConnect() 
{
	ConnectionManagerDialog connman;
	connman.SetConnectionState(IsConnected);
	int connman_modal = connman.DoModal();
	if(connman_modal == IDOK) {
		IRC_STATS irc_stats;
		irc_stats.recieved_bytes = recieved_bytes_count;
		irc_stats.sended_bytes = sended_bytes_count;
		stats_dlg->SendMessage(WM_UPDATING_STATISTICS, NULL, (LPARAM)&irc_stats);
	};
}

void MainWindow::ConnectionFunc(HWND hwnd, PARAMETERS params) 
{
	char exe_path[MAX_PATH] = {0};
	char h_pch[MAX_PATH] = {0};
	char h_pch2[MAX_PATH] = {0};
	char h_fch[MAX_PATH] = {0};
	char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename
	GetModuleFileName(NULL, exe_path, MAX_PATH);
	MainWindow* mainwin = (MainWindow*)AfxGetMainWnd();
	*(strrchr(exe_path, '\\')+1)='\0';
	strcat(exe_path, "settings.ini");
	char msg_history_string[80];
	char language_string[80];
	GetPrivateProfileString("Main", "MsgHistory", "Enabled", msg_history_string, 80, exe_path);
	GetPrivateProfileString("Main", "Language", "", language_string, 80, exe_path);
	CString msg_history_string2(msg_history_string);
	CString language_string2(language_string);
	char history_path[MAX_PATH] = {0};
	char server[256];
	sprintf(server, "%s", params.server);
	char realname[80];
	sprintf(realname, "%s", params.realname);
	char connecting_msgb[100];
	int port = params.port;
	GetModuleFileName(NULL, history_path, MAX_PATH);
	mainwin->delsymbs(history_path, strlen(history_path) - strlen(exe_name) - 1, strlen(history_path) - strlen(exe_name) - 1); // deleting EXE filename
	strcat(history_path, "\\history");	// add history filename

	if (server == NULL || server == "" || server == " ")
	{
	  	if(language_string2 == "Russian") {
			MessageBox("Не указан адрес сервера.", "Ошибка", MB_OK|MB_ICONSTOP);
			char statusbar_text[512];
			sprintf(statusbar_text, "Ошибка подключения: Не указан адрес сервера");
			AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText(statusbar_text);
		} else {
			MessageBox("Not specified server address.", "Error", MB_OK|MB_ICONSTOP);
			char statusbar_text[512];
			sprintf(statusbar_text, "Connection error: Not specified server address");
			AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText(statusbar_text);
		};
		return;
	};
	if (port == NULL || port == 0)
	{
		if(language_string2 == "Russian") {
			MessageBox("Не указан порт сервера.", "Ошибка", MB_OK|MB_ICONSTOP);
			char statusbar_text[512];
			sprintf(statusbar_text, "Ошибка подключения: Не указан порт сервера");
			AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText(statusbar_text);
		} else {
			MessageBox("Not specified server port.", "Error", MB_OK|MB_ICONSTOP);
			char statusbar_text[512];
			sprintf(statusbar_text, "Connection error: Not specified server port");
			AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText(statusbar_text);
		};
		return;
	};

	char* connecting_msg;
	connecting_msg = "Trying connecting to %s:%d...\n";
	TRACE(connecting_msg, &server, port);
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN client_param;
	hostent* host;
	if (sock == SOCKET_ERROR) {
		if(language_string2 == "Russian") {
			char error_msg[512];
			int errorcode = WSAGetLastError();
			sprintf(error_msg, "Не удалось запустить WinSock с кодом ошибки %d", errorcode);
			MessageBox(error_msg, "Ошибка", MB_OK|MB_ICONSTOP);
			char statusbar_text[512];
			sprintf(statusbar_text, "Ошибка подключения: Не удалось запустить WinSock с кодом ошибки %d", errorcode);
			AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText(statusbar_text);
		} else {
			char error_msg[512];
			int errorcode = WSAGetLastError();
			sprintf(error_msg, "Could not start WinSock with error code %d", errorcode);
			MessageBox(error_msg, "Error", MB_OK|MB_ICONSTOP);
			char statusbar_text[512];
			sprintf(statusbar_text, "Connection error: Could not start WinSock with error code %d", errorcode);
			AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText(statusbar_text);
		};
		return;
	};

	client_param.sin_family = AF_INET;
	connecting_msg = "Getting IP address...\n";
	TRACE0(connecting_msg);
	char statusbar_text[512];
	if(language_string2 == "Russian") {
		sprintf(statusbar_text, "Получение IP-адреса для %s...", server);
	} else {
		sprintf(statusbar_text, "Getting IP address for %s...", server);
	};
	AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText(statusbar_text);
	host = gethostbyname(server);
	if (host) {
		client_param.sin_addr.s_addr = inet_addr((char*)inet_ntoa(**(in_addr**)host->h_addr_list));
	}
	else
	{
		if(language_string2 == "Russian") {
			MessageBox("Не удалось получить IP-адрес для этого имени хоста.", "Ошибка", MB_OK|MB_ICONSTOP);
			char statusbar_text[512];
			sprintf(statusbar_text, "Ошибка подключения: Не удалось получить IP-адрес для этого имени хоста");
			AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText(statusbar_text);
		} else {
			MessageBox("Failed to get the IP address for this hostname.", "Error", MB_OK|MB_ICONSTOP);
			char statusbar_text[512];
			sprintf(statusbar_text, "Connection error: Failed to get the IP address for this hostname");
			AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText(statusbar_text);
		};
		return;
	};
	client_param.sin_port = htons(port);
	connecting_msg = "Connecting...\n";
	if(language_string2 == "Russian") {
		sprintf(statusbar_text, "Подключение к %s:%d...", server, port);
	} else {
		sprintf(statusbar_text, "Connecting to %s:%d...", server, port);
	};
	AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText(statusbar_text);
	TRACE(connecting_msg);
	int status;
	status = connect(sock, (SOCKADDR*)&client_param, sizeof(client_param));
	if (status == SOCKET_ERROR || status == INVALID_SOCKET) {
		if(language_string2 == "Russian") {
			char error_msg[512];
			int errorcode = WSAGetLastError();
			sprintf(error_msg, "Не удалось запустить WinSock с кодом ошибки %d", errorcode);
			MessageBox(error_msg, "Ошибка", MB_OK|MB_ICONSTOP);
			char statusbar_text[512];
			sprintf(statusbar_text, "Ошибка подключения: Не удалось запустить WinSock с кодом ошибки %d", errorcode);
			AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText(statusbar_text);
		} else {
			char error_msg[512];
			int errorcode = WSAGetLastError();
			sprintf(error_msg, "Could not start WinSock with error code %d", errorcode);
			MessageBox(error_msg, "Error", MB_OK|MB_ICONSTOP);
			char statusbar_text[512];
			sprintf(statusbar_text, "Connection error: Could not start WinSock with error code %d", errorcode);
			AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText(statusbar_text);
		};
		return;
	};
	try {
		mainwin->TrayMessage(NIM_DELETE);
		mainwin->IsConnected = TRUE;
		mainwin->TrayMessage(NIM_ADD);
	} catch(...) {

	};
	char sock_buffer[32768];
	char listing[32768];
	char listing_file[32768];
	char previous_listing[32768];
	char previous_listing_file[32768];

	char window_title[384];
	sprintf(window_title, "Tinelix IRC Client | %s", params.server);
	if(msg_history_string2 == "Enabled") {
		GetModuleFileName(NULL, h_pch, MAX_PATH);
		mainwin->delsymbs(h_pch, strlen(h_pch) - strlen(exe_name) - 1, strlen(h_pch) - strlen(exe_name) - 1); // deleting EXE filename
		DWORD winver = GetVersion();
		//if ((GetVersion() & 0x80000000l) == 0) {
		strcat(h_pch, "\\HISTORY");
		//sprintf(mainwin->history_path, "\0C:\\HISTORY");
		GetModuleFileName(NULL, h_pch2, MAX_PATH);
		*(strrchr(h_pch2, '\\')+1)='\0';
		strcat(h_pch2, "HISTORY");
		char current_time_ch[9];
		time_t current_time = time(NULL);
		strftime(current_time_ch, 9, "%y_%m_%d", localtime(&current_time));
		strcat(h_pch2, "\\");
		strcat(h_pch2, current_time_ch);	// add history filename
		sprintf(mainwin->history_path, "%s", h_pch);
		sprintf(mainwin->history_path2, "%s", h_pch2);
		char info_msg[120];
		int create_dir_status = CreateDirectory(h_pch, NULL);

		if(create_dir_status != 0 || GetLastError() == ERROR_ALREADY_EXISTS) {
			int create_dir_status_2 = CreateDirectory(h_pch2, NULL);
			if(create_dir_status_2 != 0 || GetLastError() == ERROR_ALREADY_EXISTS) {
				strcat(h_fch, h_pch2);
				strftime(current_time_ch, 9, "%H_%M_%S", localtime(&current_time));
				strcat(h_fch, "\\");
				strcat(h_fch, current_time_ch);
				strcat(h_fch, ".txt");
				sprintf(mainwin->history_file, "%s", h_fch);
			} if(GetLastError() == ERROR_PATH_NOT_FOUND || GetLastError() == ERROR_ACCESS_DENIED
				 || GetLastError() == ERROR_WRITE_PROTECT) {
				if(language_string2 == "Russian") {
                    char err_msg[8192];
                    char err_description[320];
                    if(GetLastError() == ERROR_PATH_NOT_FOUND) {
                        sprintf(err_description, "Путь не найден");
                    } if (GetLastError() == ERROR_ACCESS_DENIED) {
                        sprintf(err_description, "Доступ запрещен");
                    } if (GetLastError() == ERROR_WRITE_PROTECT) {
                        sprintf(err_description, "Данные защищены от записи");
                    };
                    sprintf(err_msg,"Ошибка создании папки: %s\r\nКод ошибки: %d (%s)", mainwin->history_path2, GetLastError(),
                            err_description);
                    MessageBox(err_msg, "Ошибка", MB_OK|MB_ICONSTOP);
                } else {
                    char err_msg[4096];
                    char err_description[320];
                    if(GetLastError() == ERROR_PATH_NOT_FOUND) {
                        sprintf(err_description, "Path not found");
                    } if (GetLastError() == ERROR_ACCESS_DENIED) {
                        sprintf(err_description, "Access denied");
                    } if (GetLastError() == ERROR_WRITE_PROTECT) {
                        sprintf(err_description, "Data is write-protected");
                    };
                    sprintf(err_msg,"Error creating folder: %s\r\nError code: %d (%s)", mainwin->history_path2, GetLastError(),
                            err_description);
                    MessageBox(err_msg, "Error", MB_OK|MB_ICONSTOP);
                };
			};

		} if(GetLastError() == ERROR_PATH_NOT_FOUND || GetLastError() == ERROR_ACCESS_DENIED
			 || GetLastError() == ERROR_WRITE_PROTECT) {
			if(language_string2 == "Russian") {
                    char err_msg[8192];
                    char err_description[320];
                    if(GetLastError() == ERROR_PATH_NOT_FOUND) {
                        sprintf(err_description, "Путь не найден");
                    } if (GetLastError() == ERROR_ACCESS_DENIED) {
                        sprintf(err_description, "Доступ запрещен");
                    } if (GetLastError() == ERROR_WRITE_PROTECT) {
                        sprintf(err_description, "Данные защищены от записи");
                    };
                    sprintf(err_msg,"Ошибка создании папки: %s\r\nКод ошибки: %d (%s)", mainwin->history_path, GetLastError(),
                            err_description);
                    MessageBox(err_msg, "Ошибка", MB_OK|MB_ICONSTOP);
            } else {
                    char err_msg[4096];
                    char err_description[320];
                    if(GetLastError() == ERROR_PATH_NOT_FOUND) {
                        sprintf(err_description, "Path not found");
                    } if (GetLastError() == ERROR_ACCESS_DENIED) {
                        sprintf(err_description, "Access denied");
                    } if (GetLastError() == ERROR_WRITE_PROTECT) {
                        sprintf(err_description, "Data is write-protected");
                    };
                    sprintf(err_msg,"Error creating folder: %s\r\nError code: %d (%s)", mainwin->history_path, GetLastError(),
                            err_description);
                    MessageBox(err_msg, "Error", MB_OK|MB_ICONSTOP);
                };
		};
	};

	AfxGetApp()->GetMainWnd()->SetWindowText(window_title);

	int WSAAsync = WSAAsyncSelect(sock, AfxGetApp()->GetMainWnd()->GetSafeHwnd(), WM_SOCKET_MESSAGE, FD_READ | FD_CLOSE);
	if (WSAAsync > 0) {
		if(language_string2 == "Russian") {
			char error_msg[512];
			int errorcode = WSAGetLastError();
			sprintf(error_msg, "Не удалось запустить WinSock с кодом ошибки %d", errorcode);
			MessageBox(error_msg, "Ошибка", MB_OK|MB_ICONSTOP);
			char statusbar_text[512];
			sprintf(statusbar_text, "Ошибка подключения: Не удалось запустить WinSock с кодом ошибки %d", errorcode);
			AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText(statusbar_text);
		} else {
			char error_msg[512];
			int errorcode = WSAGetLastError();
			sprintf(error_msg, "Could not start WinSock with error code %d", errorcode);
			MessageBox(error_msg, "Error", MB_OK|MB_ICONSTOP);
			char statusbar_text[512];
			sprintf(statusbar_text, "Connection error: Could not start WinSock with error code %d", errorcode);
			AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText(statusbar_text);
		};
		return;
	};
	char ident_sending[400];
	int ident_sending_parts;
	ident_sending_parts = sprintf(ident_sending, "USER %s ", params.nickname);
	ident_sending_parts += sprintf(ident_sending + ident_sending_parts, "%s ", params.nickname);
	ident_sending_parts += sprintf(ident_sending + ident_sending_parts, "%s :", params.nickname);
	ident_sending_parts += sprintf(ident_sending + ident_sending_parts, "%s\r\n", params.realname);
	status = send(sock, ident_sending, strlen(ident_sending), 0);
	if(status != SOCKET_ERROR && status > 0) {
		sended_bytes_count = sended_bytes_count + status;
	};
	IRC_STATS irc_stats;
	irc_stats.recieved_bytes = recieved_bytes_count;
	irc_stats.sended_bytes = sended_bytes_count;
	mainwin->stats_dlg->SendMessage(WM_UPDATING_STATISTICS, NULL, (LPARAM)&irc_stats);
	char nick_sending[400];
	int nick_sending_parts;
	nick_sending_parts = sprintf(nick_sending, "NICK %s\n", params.nickname);
	mainwin->irc_chat_page->GetDlgItem(IDC_MSGTEXT)->EnableWindow(TRUE);
	status = send(sock, nick_sending, strlen(nick_sending), 0);
	if(status != SOCKET_ERROR && status > 0) {
		sended_bytes_count = sended_bytes_count + status;
	};
	irc_stats.recieved_bytes = recieved_bytes_count;
	irc_stats.sended_bytes = sended_bytes_count;
	mainwin->stats_dlg->SendMessage(WM_UPDATING_STATISTICS, NULL, (LPARAM)&irc_stats);
	if(params.hide_ip == TRUE) {
		char mode_x_sending[400];
		int mode_x_sending_parts;
		mode_x_sending_parts = sprintf(mode_x_sending, "MODE %s +x\n", params.nickname);
		status = send(sock, mode_x_sending, strlen(mode_x_sending), 0);
		if(status != SOCKET_ERROR && status > 0) {
			sended_bytes_count = sended_bytes_count + status;
		};
		irc_stats.recieved_bytes = recieved_bytes_count;
		irc_stats.sended_bytes = sended_bytes_count;
		mainwin->stats_dlg->SendMessage(WM_UPDATING_STATISTICS, NULL, (LPARAM)&irc_stats);
	};
	if(language_string2 == "Russian") {
		sprintf(statusbar_text, "Готово");
	} else {
		sprintf(statusbar_text, "Ready");
	};
	AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_STATUSBAR_TEXT)->SetWindowText(statusbar_text);

}

void MainWindow::CreateConnectionThread(PARAMETERS profile_params) 
{
	HWND hwnd;
	hwnd = m_hWnd;
	char change_font[100];
	sprintf(params.nickname, "%s", profile_params.nickname);
	sprintf(params.reserve_nickname, "%s", profile_params.reserve_nickname);
	sprintf(params.server, "%s", profile_params.server);
	params.port = profile_params.port;
	sprintf(params.quit_msg, "%s", profile_params.quit_msg);
	MainWindow::ConnectionFunc(hwnd, profile_params);

}

LRESULT MainWindow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	char exe_path[MAX_PATH] = {0};
	char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename

	GetModuleFileName(NULL, exe_path, MAX_PATH);  

	*(strrchr(exe_path, '\\')+1)='\0';

	strcat(exe_path, "\\settings.ini");	// add settings filename
	
	TCHAR language_string[MAX_PATH] = {0};
	TCHAR msg_history_string[MAX_PATH] = {0};
	TCHAR show_msgtime_string[MAX_PATH] = {0};
	TCHAR msgtime_pos_string[MAX_PATH] = {0};
	//try {
	if (message == WM_SOCKET_MESSAGE) {
				GetPrivateProfileString("Parser", "ShowMsgTime", "Enabled", show_msgtime_string, MAX_PATH, exe_path);
				GetPrivateProfileString("Main", "Language", "English", language_string, MAX_PATH, exe_path);
				GetPrivateProfileString("Main", "MsgHistory", "Enabled", msg_history_string, MAX_PATH, exe_path);
				GetPrivateProfileString("Parser", "MsgTimePos", "Right", msgtime_pos_string, MAX_PATH, exe_path);
				CString lng_selitemtext(language_string);
				CString msg_history_string2(msg_history_string);
				int status;
				char sock_buffer[32768] = {0};
				status = recv((SOCKET)wParam, (char*) &sock_buffer, 32767, 0);
				recieved_bytes_count = recieved_bytes_count + status;
				IRC_STATS irc_stats;
				irc_stats.recieved_bytes = recieved_bytes_count;
				irc_stats.sended_bytes = sended_bytes_count;
				stats_dlg->SendMessage(WM_UPDATING_STATISTICS, NULL, (LPARAM)&irc_stats);
				if (status == SOCKET_ERROR) {
					char error_msg[100];
					if(lng_selitemtext == "Russian") {
						sprintf(error_msg, "Ошибка сокета: %d", WSAGetLastError());
						MessageBox(error_msg, "Ошибка", MB_OK|MB_ICONSTOP);
					} else {
						sprintf(error_msg, "Socket error: %d", WSAGetLastError());
						MessageBox(error_msg, "Error", MB_OK|MB_ICONSTOP);
					};
					try {
						AfxGetApp()->GetMainWnd()->SetWindowText("Tinelix IRC Client");
					} catch(...) {

					};
					exit;
				};
				if (status == 0) {
					if(lng_selitemtext == "Russian") {
						MessageBox("Соединение потеряно.", "Информация", MB_OK|MB_ICONINFORMATION);
					} else {
						MessageBox("Connection lost.", "Information", MB_OK|MB_ICONINFORMATION);
					};
					IsConnected = FALSE;
					closesocket(sock);
					AfxGetApp()->GetMainWnd()->SetWindowText("Tinelix IRC Client");
					exit;
				};
				char listing[32768] = {0};
				char previous_listing[32768] = {0};
				int sp;
				CString sock_buffer_str;
				char pong_msg[400];
				int string_index = 0;
				int array_index_2 = 0;
				char nwline[32768];
				char* parsed_msg;
				char* parsed_msg_list;
				parsed_msg_list = (char*)malloc(sizeof(char) * 32768);
				char* nw_token;
				CArray<CString, CString> new_line_splitter;
				while(AfxExtractSubString(sock_buffer_str, sock_buffer, string_index, '\n'))
				{
					new_line_splitter.Add(sock_buffer_str);
					++string_index;
				};
				CString pong;
				nw_token = strtok(sock_buffer, "\r\n");										   
				while(nw_token != NULL) {
					new_line_splitter[array_index_2++] = nw_token;
					nw_token = strtok(NULL, "\r\n");
				};

				int parsed_msg_index = 0;

                for (int i = 0; i < new_line_splitter.GetSize(); i++) {
					pong = "";
					//TRACE("LINE: %s\r\n", new_line_splitter[i]);
					if (new_line_splitter[i].Left(4) == "PING") {
						int pong_index;
						sprintf(pong_msg, "PONG %s\r\n", new_line_splitter[i].Right(strlen(new_line_splitter[i]) - 5));
						status = send((SOCKET)wParam, pong_msg, strlen(pong_msg), 0);
						if(status != SOCKET_ERROR && status > 0) {
							sended_bytes_count = sended_bytes_count + status;
						};
						IRC_STATS irc_stats;
						irc_stats.recieved_bytes = recieved_bytes_count;
						irc_stats.sended_bytes = sended_bytes_count;
						stats_dlg->SendMessage(WM_UPDATING_STATISTICS, NULL, (LPARAM)&irc_stats);
						pong = pong_msg;
						i = new_line_splitter.GetSize();
					} else {
						if(parserLib == NULL) {
							int iTab = m_irc_tabs.GetCurSel();
							TC_ITEM tci;
							tci.mask = TCIF_PARAM;
							m_irc_tabs.GetItem(iTab, &tci);
							IRCChannelPage* channel_page = (IRCChannelPage *)tci.lParam;
							if(i >= new_line_splitter.GetSize() - 1) {
								parsed_msg_index += sprintf(parsed_msg_list + parsed_msg_index, "%s", new_line_splitter[i]);
							} else {
								parsed_msg_index += sprintf(parsed_msg_list + parsed_msg_index, "%s\r\n", new_line_splitter[i]);
							};
							channel_page->GetDlgItem(IDC_SOCKMSGS)->GetWindowText(previous_listing, 32767);
							sp = sprintf(listing, "%s", previous_listing);
							if(strlen(previous_listing) + strlen(parsed_msg_list) < 32768) {
								sp += sprintf(listing + sp, "%s", parsed_msg_list);
							} else {
								sp = sprintf(listing + sp, "%s", parsed_msg_list);
							};
						} else {	
							char* unparsed_msg;
							char* parsed_msg;
							unparsed_msg = (char*)calloc(sizeof(char), 32768 + 1);
							parsed_msg = (char*)calloc(sizeof(char), 32768 + 1);
							sprintf(unparsed_msg, "%s", new_line_splitter[i]);
							for (int owners_array_index = 0; owners_array_index < sizeof(owners_array); owners_array_index++) {
								sprintf(owners_array[owners_array_index], "");
							};

							for (int operators_array_index = 0; operators_array_index < sizeof(operators_array); operators_array_index++) {
								sprintf(operators_array[operators_array_index], "");
							};

							for (int members_array_index = 0; members_array_index < sizeof(members_array); members_array_index++) {
								sprintf(members_array[members_array_index], "");
							};
							ParseMessage ParseMsg;
							ParseNamesMessage ParseNamesMsg;
							ParseMsg = (ParseMessage)GetProcAddress((HMODULE)parserLib, "ParseMessage");
							ParseNamesMsg = (ParseNamesMessage)GetProcAddress((HMODULE)parserLib, "ParseNamesMessage");
							try {
								if(strcmp(show_msgtime_string, "Enabled") == 0) {
									if(strcmp(msgtime_pos_string, "Left") == 0) {
										ParseMsg(unparsed_msg, parsing_array, parsed_msg, FALSE, "left");
									} else {
										ParseMsg(unparsed_msg, parsing_array, parsed_msg, FALSE, "right");
									};
								} else {
									ParseMsg(unparsed_msg, parsing_array, parsed_msg, FALSE, "noshow");
								};
								CString p_msg(parsed_msg);
								if(strcmp(parsing_array[1], "433") == 0) {
									char nick_msg[128];
									sprintf(nick_msg, "NICK %s\r\n", params.reserve_nickname);
									status = send((SOCKET)wParam, nick_msg, strlen(nick_msg), 0);
									parsed_msg_index += sprintf(parsed_msg_list + parsed_msg_index, "%s", "WARNING: We will use next nickname.\r\n");
								} else if(strcmp(parsing_array[1], "353") == 0) {
									sprintf(unparsed_msg, "%s", new_line_splitter[i]);
									int owners_count = 0;
									int operators_count = 0;
									int members_count = 0;
									ParseNamesMsg(unparsed_msg, parsing_array, owners_array, operators_array, members_array, owners_count, operators_count, members_count, TRUE);
									if(parsing_array[4][0] == '#') {
										for (int ch_array_index = 0; ch_array_index < sizeof(channels); ch_array_index++) {
											if(strcmp(parsing_array[4], channels[ch_array_index]) == 0 && strlen(channels[ch_array_index]) > 0) {
												TC_ITEM tci;
												tci.mask = TCIF_PARAM;
												m_irc_tabs.GetItem(ch_array_index + 1, &tci);
												IRCChannelPage* channel_page = (IRCChannelPage *)tci.lParam;
												if(channel_page->m_hWnd != NULL) {
													CTreeCtrl* members_tree = (CTreeCtrl*)channel_page->GetDlgItem(IDC_MEMBERS_LIST);
													HTREEITEM owners_item = 0, operators_item = 0, members_item = 0;
													for(int owners_index = 0; owners_index < sizeof(owners_array); owners_index++) {
														if(strlen(owners_array[owners_index]) > 0) {
															owners_count = owners_count + 1;
														};
													};
													for(int operators_index = 0; operators_index < sizeof(operators_array); operators_index++) {
														if(strlen(operators_array[operators_index]) > 0) {
															operators_count = operators_count + 1;
														};
													};
													for(int members_index = 0; members_index < sizeof(members_array); members_index++) {
														if(strlen(members_array[members_index]) > 0) {
															members_count = members_count + 1;
														};
													};
													members_tree->DeleteAllItems();
													char parent_name[128];
													if(lng_selitemtext == "Russian") {
														sprintf(parent_name, "Владельцы (%d)", owners_count);
														owners_item = members_tree->InsertItem(parent_name, TVI_ROOT);
													} else {
														sprintf(parent_name, "Owners (%d)", owners_count);
														owners_item = members_tree->InsertItem(parent_name, TVI_ROOT);
													};
													if(lng_selitemtext == "Russian") {
														sprintf(parent_name, "Операторы (%d)", operators_count);
														operators_item = members_tree->InsertItem(parent_name, TVI_ROOT);
													} else {
														sprintf(parent_name, "Operators (%d)", operators_count);
														operators_item = members_tree->InsertItem(parent_name, TVI_ROOT);
													};
													if(lng_selitemtext == "Russian") {
														sprintf(parent_name, "Участники (%d)", members_count);
														members_item = members_tree->InsertItem(parent_name, TVI_ROOT);
													} else {
														sprintf(parent_name, "Members (%d)", members_count);
														members_item = members_tree->InsertItem(parent_name, TVI_ROOT);
													};
													for(owners_index = 0; owners_index < sizeof(owners_array); owners_index++) {
														if(strlen(owners_array[owners_index]) > 0) {
															members_tree->InsertItem(owners_array[owners_index], owners_item);
														};
													};
													for(operators_index = 0; operators_index < sizeof(operators_array); operators_index++) {
														if(strlen(operators_array[operators_index]) > 0) {
															members_tree->InsertItem(operators_array[operators_index], operators_item);
															
														};
													};
													for(members_index = 0; members_index < sizeof(members_array); members_index++) {
														if(strlen(members_array[members_index]) > 0) {
															members_count++;
															members_tree->InsertItem(members_array[members_index], members_item);
														};
													};
													
													members_tree->Expand(owners_item,TVE_EXPAND);
													members_tree->Expand(operators_item,TVE_EXPAND);
													members_tree->Expand(members_item,TVE_EXPAND);
												};
											};
										};
									};
								};
								if(parsing_array[2][0] == '#') {
									for (int ch_array_index = 0; ch_array_index < sizeof(channels); ch_array_index++) {
										if(strcmp(parsing_array[2], channels[ch_array_index]) == 0 && strlen(channels[ch_array_index]) > 0) {
											TC_ITEM tci;
											tci.mask = TCIF_PARAM;
											m_irc_tabs.GetItem(ch_array_index + 1, &tci);
											IRCChannelPage* channel_page = (IRCChannelPage *)tci.lParam;
											if(channel_page->m_hWnd != NULL) {
												channel_page->GetDlgItem(IDC_SOCKMSGS)->GetWindowText(previous_listing, 32767);
												sp = sprintf(listing, "%s", previous_listing);
												if(strlen(previous_listing) + strlen(parsed_msg_list) < 32768) {
													sp += sprintf(listing + sp, "%s", parsed_msg);
												} else {
													sp = sprintf(listing + sp, "%s", parsed_msg);
												};
												channel_page->GetDlgItem(IDC_SOCKMSGS)->SetWindowText(listing);
												CEdit* msg_box = (CEdit*)channel_page->GetDlgItem(IDC_SOCKMSGS);
												msg_box->SetSel(0, -1);
												msg_box->SetSel(-1);
											};
										};
									};
								} else if(channels_count > 0 && strcmp(parsing_array[1], "JOIN") == 0 || strcmp(parsing_array[1], "PART") == 0 || 
									strcmp(parsing_array[1], "NICK") == 0 || strcmp(parsing_array[1], "QUIT") == 0) { 
									for (int ch_array_index = 0; ch_array_index < sizeof(channels); ch_array_index++) {
										if(strlen(channels[ch_array_index]) > 0) {
											char names_msg[160];
											sprintf(names_msg, "NAMES #%s\r\n", channels[ch_array_index]);
											status = send((SOCKET)wParam, names_msg, strlen(names_msg), 0);		
										};
									};
								} else {
									int iTab = m_irc_tabs.GetCurSel();
									TC_ITEM tci;
									tci.mask = TCIF_PARAM;
									m_irc_tabs.GetItem(iTab, &tci);
									IRCChannelPage* channel_page = (IRCChannelPage *)tci.lParam;
									if(channel_page->m_hWnd != NULL) {
										channel_page->GetDlgItem(IDC_SOCKMSGS)->GetWindowText(previous_listing, 32767);
										sp = sprintf(listing, "%s", previous_listing);
										if(strlen(previous_listing) + strlen(parsed_msg_list) < 32768) {
											sp += sprintf(listing + sp, "%s", parsed_msg);
										} else {
											sp = sprintf(listing + sp, "%s", parsed_msg);
										};
										channel_page->GetDlgItem(IDC_SOCKMSGS)->SetWindowText(listing);
										CEdit* msg_box = (CEdit*)channel_page->GetDlgItem(IDC_SOCKMSGS);
										msg_box->SetSel(0, -1);
										msg_box->SetSel(-1);
									};
								};
							} catch(...) {

							};

							delete unparsed_msg;
							delete parsed_msg;

						};
					};
				};
				if(pong == "") {
					if(msg_history_string2 == "Enabled") {
						if(history_file_stdio.Open(history_file, CFile::modeReadWrite)) {
							history_file_stdio.Write(listing, strlen(listing));
							history_file_stdio.Close();
						} else {
							if (history_file_stdio.Open(history_file, CFile::modeReadWrite | CFile::modeCreate)) {
								history_file_stdio.Write(listing, strlen(listing));
								history_file_stdio.Close();
							} else {
							};
						};
					};
					if(parserLib == NULL) {
						int iTab = m_irc_tabs.GetCurSel();
						TC_ITEM tci;
						tci.mask = TCIF_PARAM;
						m_irc_tabs.GetItem(iTab, &tci);
						IRCChannelPage* channel_page = (IRCChannelPage *)tci.lParam;
						channel_page->GetDlgItem(IDC_SOCKMSGS)->SetWindowText(listing);
						CEdit* msg_box = (CEdit*)channel_page->GetDlgItem(IDC_SOCKMSGS);
						msg_box->SetSel(0, -1);
						msg_box->SetSel(-1);
					};
					delete parsed_msg_list;
				};
	} else if(message == WM_SENDING_SOCKET_MESSAGE) {
		char *text = (char*)wParam;
		int status = MainWindow::SendSocketMessage(text);
		if(status != SOCKET_ERROR && status > 0) {
			sended_bytes_count = sended_bytes_count + status;
		};
		IRC_STATS irc_stats;
		irc_stats.recieved_bytes = recieved_bytes_count;
		irc_stats.sended_bytes = sended_bytes_count;
		stats_dlg->SendMessage(WM_UPDATING_STATISTICS, NULL, (LPARAM)&irc_stats);
	} else if(message == WM_SENDING_QUIT_MESSAGE) {
		char quit_msg[640];
		sprintf(quit_msg, "QUIT :%s\r\n", params.quit_msg);
		int status = MainWindow::SendSocketMessage(quit_msg);
	} else if(message == WM_NOTIFYICON && (wParam == IDR_TRAY || wParam == IDR_TRAY_NC)) {
		if(lParam == WM_LBUTTONDBLCLK) {
			ShowWindow(SW_NORMAL);
			SetForegroundWindow();
			SetFocus();
		} else if(lParam == 517) {
			char exe_path[MAX_PATH] = {0};
			char dll_path[MAX_PATH] = {0};
			char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename

			TCHAR language_string[MAX_PATH] = {0};
			TCHAR mini2tray_string[MAX_PATH] = {0};
			TCHAR showinfomsg_string[MAX_PATH] = {0};

			GetModuleFileName(NULL, exe_path, MAX_PATH);
			GetModuleFileName(NULL, dll_path, MAX_PATH);

			*(strrchr(exe_path, '\\')+1)='\0';
			*(strrchr(dll_path, '\\')+1)='\0';

			strcat(exe_path, "\\settings.ini");	// add settings filename

			strcat(dll_path, "\\parser.dll");

			GetPrivateProfileString("Main", "Language", "", language_string, MAX_PATH, exe_path);
			GetPrivateProfileString("Main", "MinimizeToTray", "", mini2tray_string, MAX_PATH, exe_path);
			GetPrivateProfileString("Main", "ShowInfoMessages", "", showinfomsg_string, MAX_PATH, exe_path);

			CString lng_selitemtext_2(language_string);
			
			if (lng_selitemtext_2 == "Russian") {
				tray_context_menu = (CMenu*)malloc(sizeof(CMenu));
				tray_context_menu->m_hMenu = NULL;
				tray_context_menu->CreatePopupMenu();
				tray_context_menu->AppendMenu(MF_STRING, ID_FILE_CONNECT, "Показать окно");
				tray_context_menu->AppendMenu(MF_STRING, ID_FILE_STATISTICS, "Статистика");
				tray_context_menu->AppendMenu(MF_SEPARATOR);
				tray_context_menu->AppendMenu(MF_STRING, ID_VIEW_SETTINGS, "Настройки");
				tray_context_menu->AppendMenu(MF_SEPARATOR);
				tray_context_menu->AppendMenu(MF_STRING, ID_FILE_QUIT, "Выход");
			} else {
				tray_context_menu = (CMenu*)malloc(sizeof(CMenu));
				tray_context_menu->m_hMenu = NULL;
				tray_context_menu->CreatePopupMenu();
				tray_context_menu->AppendMenu(MF_STRING, ID_FILE_CONNECT, "Show window");
				tray_context_menu->AppendMenu(MF_STRING, ID_FILE_STATISTICS, "Statistics");
				tray_context_menu->AppendMenu(MF_SEPARATOR);
				tray_context_menu->AppendMenu(MF_STRING, ID_VIEW_SETTINGS, "Settings");
				tray_context_menu->AppendMenu(MF_SEPARATOR);
				tray_context_menu->AppendMenu(MF_STRING, ID_FILE_QUIT, "Quit");
			};
			POINT pt;
			if (GetCursorPos(&pt) != NULL) {
				TrackPopupMenu(tray_context_menu->m_hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, m_hWnd, NULL);
				DestroyMenu(tray_context_menu->m_hMenu);
			};

		}
	} else if(message == WM_PARSER_SWITCH) {
		char *args = (char*)wParam;
		char exe_path[MAX_PATH] = {0};
		char dll_path[MAX_PATH] = {0};
		char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename

		TCHAR language_string[MAX_PATH] = {0};
		TCHAR mini2tray_string[MAX_PATH] = {0};

		GetModuleFileName(NULL, exe_path, MAX_PATH);
		GetModuleFileName(NULL, dll_path, MAX_PATH);

		*(strrchr(exe_path, '\\')+1)='\0';
		*(strrchr(dll_path, '\\')+1)='\0';

		strcat(dll_path, "\\parser.dll");
		
		if(strcmp(args, "io") == 0 && parserLib == NULL) {
			parserLib = (HINSTANCE)malloc(sizeof(HINSTANCE));
			parserLib = LoadLibrary(dll_path);
		} else if(strcmp(args, "io") == 0) {
			FreeLibrary(parserLib);
			parserLib = NULL;
		};
	} else if(message == WM_JOINING_TO_CHANNEL) {
		GetModuleFileName(NULL, exe_path, MAX_PATH);
		MainWindow* mainwin = (MainWindow*)AfxGetMainWnd();
		*(strrchr(exe_path, '\\')+1)='\0';
		strcat(exe_path, "settings.ini");
		char information_txt[512];
		char show_infomsg[12];
		GetPrivateProfileString("Main", "Language", "English", language_string, MAX_PATH, exe_path);
		GetPrivateProfileString("Main", "ShowInfoMessages", "", show_infomsg, 12, exe_path);
		CString lng_selitemtext_2(language_string);
		if(parserLib == NULL && strcmp(show_infomsg, "Enabled") == 0 && lng_selitemtext_2 == "Russian") {
			sprintf(information_txt, "Вывод сообщений в отдельных вкладках недоступна без DLL-библиотеки Tinelix "
				"IRC Parser (файла PARSER.DLL).\r\n\r\nДля решения этой проблемы проверьте, существует ли файл с таким именем. "
				"Если этого DLL-файла не существует, cкачайте, распакуйте дистрибутив IRC-клиента целиком или соберите "
				"библиотеку из исходных кодов: github.com/tinelix/irc-client-for-windows/tree/beta/PARSER");
			info_msg_dlg->Create(InfoMessageDialog::IDD, this);
			info_msg_dlg->GetDlgItem(IDC_INFOMSG_TEXT)->SetWindowText(information_txt);
			info_msg_dlg->SetWindowText("Не найдена необходимая библиотека");
			info_msg_dlg->CenterWindow();
			info_msg_dlg->ShowWindow(SW_NORMAL);
		} else if(parserLib == NULL && strcmp(show_infomsg, "Enabled") == 0 && lng_selitemtext_2 == "English") {
			sprintf(information_txt, "Displaying messages in separate tabs is not available without a Tinelix IRC Parser DLL"
				"(PARSER.DLL file).\r\n\r\nTo solve this problem, check if this file exists. If this DLL file does not exist, download, "
				"unpack IRC client distribution, or build the library from sources: github.com/tinelix/irc-client-for-windows/tree/beta/PARSER");
			info_msg_dlg->Create(InfoMessageDialog::IDD, this);
			info_msg_dlg->GetDlgItem(IDC_INFOMSG_TEXT)->SetWindowText(information_txt);
			info_msg_dlg->SetWindowText("Required library not found");
			info_msg_dlg->CenterWindow();
			info_msg_dlg->ShowWindow(SW_NORMAL);
		} else {
			SendMessage(WM_CLOSE_INFOMSG, NULL, NULL);
			
			irc_channel_page[channels_count]->Create(IRCChannelPage::IDD, &m_irc_tabs);	
			
			char* channel_name = (char*)lParam;
			sprintf(channels[channels_count], "%s", channel_name);

			char font_string[48] = {0};
			int font_size = 9;

			TC_ITEM tci;
			tci.mask = TCIF_TEXT;
			tci.iImage = -1;
			tci.pszText = channels[channels_count];
			m_irc_tabs.InsertItem(channels_count + 1, &tci);
			tci.mask = TCIF_PARAM;
			tci.lParam = (LPARAM)irc_channel_page[channels_count];
			m_irc_tabs.SetItem(channels_count + 1, &tci);
			CRect rcClient, rcWindow;

			m_irc_tabs.GetClientRect(&rcClient);
			m_irc_tabs.AdjustRect(FALSE, &rcClient);
			m_irc_tabs.GetWindowRect(&rcWindow);
			ScreenToClient(rcWindow);

			sended_bytes_count = 0;
			recieved_bytes_count = 0;

			rcClient.OffsetRect(rcWindow.left, rcWindow.top);
			
			m_irc_tabs.GetItem(channels_count + 1, &tci);
			IRCChannelPage* channel_page = (IRCChannelPage *)tci.lParam;

			sprintf(channel_page->channel, channel_name);

			channel_page->MoveWindow(&rcClient, TRUE);

			HDC hDC = ::GetDC(NULL);
			GetPrivateProfileString("Main", "MsgFont", "Fixedsys", font_string, 48, exe_path);
			font_size = GetPrivateProfileInt("Main", "MsgFontSize", 8, exe_path);
			int font_height = 0;
			font_height = -MulDiv(font_size, GetDeviceCaps(hDC, LOGPIXELSY), 72);
			
			CFont font;
			
			font.CreateFont(font_height, 0, 0, 0, FW_REGULAR, FALSE, FALSE, 0, DEFAULT_CHARSET, 0, 0,
			0, 0, font_string);

			int page_frame_w = rcWindow.Width() - 4;
			int page_frame_h = rcWindow.Height() - 23;
			channel_page->SetWindowPos(NULL, 2, 21, page_frame_w, page_frame_h, SWP_NOZORDER);
			channel_page->GetDlgItem(IDC_SOCKMSGS)->MoveWindow(2, 2, page_frame_w - 112, page_frame_h - 28);
			channel_page->GetDlgItem(IDC_MEMBERS_LIST)->MoveWindow(page_frame_w - 108, 2, 106, page_frame_h - 28);
			channel_page->GetDlgItem(IDC_MSGTEXT)->MoveWindow(2, page_frame_h - 24, page_frame_w - 72, 22);
			channel_page->GetDlgItem(IDC_SENDMSG)->MoveWindow(page_frame_w - 68, page_frame_h - 24, 66, 22);
			channel_page->GetDlgItem(IDC_MSGTEXT)->EnableWindow(TRUE);
			channel_page->GetDlgItem(IDC_SOCKMSGS)->SetFont(&font);
			font.Detach();

			int iTab = m_irc_tabs.GetCurSel();
			tci.mask = TCIF_PARAM;
			m_irc_tabs.GetItem(iTab, &tci);
			CWnd* pWnd = (CWnd *)tci.lParam;
			pWnd->ShowWindow(SW_HIDE); 

			m_irc_tabs.SetCurSel(channels_count + 1);

			iTab = m_irc_tabs.GetCurSel();
			tci.mask = TCIF_PARAM;
			m_irc_tabs.GetItem(iTab, &tci);
			pWnd = (CWnd *)tci.lParam;
			pWnd->ShowWindow(SW_SHOW); 

			channels_count++;
		};
	} else if(message == WM_LEAVING_CHANNEL) {
			char* channel_name = (char*)wParam;
			for (int channels_index = 0; channels_index < channels_count; channels_index++) {
				TC_ITEM tci;
				tci.mask = TCIF_PARAM;
				m_irc_tabs.GetItem(channels_index + 1, &tci);
				IRCChannelPage* channel_page = (IRCChannelPage *)tci.lParam;
				if(strlen(channels[channels_index]) > 0 && strcmp(channels[channels_index], channel_page->channel) == 0) {
					channels_count = channels_count - 1;
					int iTab = m_irc_tabs.GetCurSel();
					TC_ITEM tci;
					tci.mask = TCIF_PARAM;
					m_irc_tabs.GetItem(iTab, &tci);
					CWnd* pWnd = (CWnd *)tci.lParam;
					pWnd->ShowWindow(SW_HIDE); 
					
					m_irc_tabs.SetCurSel(m_irc_tabs.GetCurSel() - 1);
					
					iTab = m_irc_tabs.GetCurSel();
					m_irc_tabs.GetItem(iTab, &tci);
					pWnd = (CWnd *)tci.lParam;
					pWnd->ShowWindow(SW_SHOW); 

					m_irc_tabs.DeleteItem(channels_index + 1);
					channel_page->DestroyWindow();
	
				};
			};
		};
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL MainWindow::DestroyWindow() 
{
	closesocket(sock);
	WSACleanup();
	TRACE("Quiting...\r\n");
	FreeLibrary(parserLib);
	delete irc_chat_page;
	for(int ch_array_index = 0; ch_array_index < 1024; ch_array_index++) {
		delete irc_channel_page[ch_array_index];
	};

	delete stats_dlg;
	TRACE("Freeing memory...\r\n");
	delete parsing_array;
	delete[] channels;
	delete info_msg_dlg;
	delete[] owners_array;
	delete[] operators_array;
	delete[] members_array;
	return CDialog::DestroyWindow();
}


UINT MainWindow::SendSocketMessage(char *str)
{
	int status = send(sock, str, strlen(str), 0);
	if(status != SOCKET_ERROR && status > 0) {
		sended_bytes_count += status;
	};
	IRC_STATS irc_stats;
	irc_stats.recieved_bytes = recieved_bytes_count;
	irc_stats.sended_bytes = sended_bytes_count;
	stats_dlg->SendMessage(WM_UPDATING_STATISTICS, NULL, (LPARAM)&irc_stats);
	return status;
};

void MainWindow::OnViewSettings() 
{
	SettingsDialog setgdlg;
	int settings_modal = setgdlg.DoModal();
	if(settings_modal == IDOK) {
		IRC_STATS irc_stats;
		irc_stats.recieved_bytes = recieved_bytes_count;
		irc_stats.sended_bytes = sended_bytes_count;
		stats_dlg->SendMessage(WM_UPDATING_STATISTICS, NULL, (LPARAM)&irc_stats);
	};
	
}

void MainWindow::OnHelpAbout() 
{
	AboutDialog aboutdlg;
	aboutdlg.DoModal();
	
}

void MainWindow::OnOK()
{
	// CDialog::OnOK();
};

void MainWindow::OnCancel()
{
	// CDialog::OnCancel();
};

void MainWindow::OnFileQuit() 
{
	char exe_path[MAX_PATH] = {0};
	char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename
	TCHAR language_string[MAX_PATH] = {0};
	
	GetModuleFileName(NULL, exe_path, MAX_PATH);

	*(strrchr(exe_path, '\\')+1)='\0';

	strcat(exe_path, "\\settings.ini");
	
	if(IsConnected == 1) {
		char quit_msg[640];
		sprintf(quit_msg, "QUIT :%s\r\n", params.quit_msg);
		TRACE("Sending quit message...\r\n");
		SendSocketMessage(quit_msg);
	};
	TrayMessage(NIM_DELETE);
	EndDialog(NULL);	
}

void MainWindow::OnClose() 
{
	char exe_path[MAX_PATH] = {0};
	char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename
	TCHAR language_string[MAX_PATH] = {0};
	
	GetModuleFileName(NULL, exe_path, MAX_PATH);

	*(strrchr(exe_path, '\\')+1)='\0';

	strcat(exe_path, "\\settings.ini");
	
	if(IsConnected == 1) {
		char quit_msg[640];
		sprintf(quit_msg, "QUIT :%s\r\n", params.quit_msg);
		TRACE("Sending quit message...\r\n");
		SendSocketMessage(quit_msg);
	};
	TrayMessage(NIM_DELETE);
	EndDialog(NULL);
	CDialog::OnClose();
}

void MainWindow::OnFileStatistics() 
{
	char exe_path[MAX_PATH] = {0};
	char dll_path[MAX_PATH] = {0};
	char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename
	TCHAR language_string[MAX_PATH] = {0};
	
	GetModuleFileName(NULL, exe_path, MAX_PATH);
	GetModuleFileName(NULL, dll_path, MAX_PATH);

	*(strrchr(exe_path, '\\')+1)='\0';
	*(strrchr(dll_path, '\\')+1)='\0';

	strcat(exe_path, "\\settings.ini");	// add settings filename

	strcat(dll_path, "\\parser.dll");

	GetPrivateProfileString("Main", "Language", "", language_string, MAX_PATH, exe_path);

	CString lng_selitemtext_2(language_string);

	if(IsConnected == 0) {
		if(lng_selitemtext_2 == "Russian") {
			MessageBox("Сначала подключитесь к IRC-серверу и повторите попытку.", "Tinelix IRC Client", MB_OK|MB_ICONSTOP);	
		} else {
			MessageBox("First connect to the IRC server and try again.", "Tinelix IRC Client", MB_OK|MB_ICONSTOP);
		};
		return;
	};
	stats_dlg->sended_bytes_count = sended_bytes_count;
	stats_dlg->recieved_bytes_count = recieved_bytes_count;
	stats_dlg->CenterWindow();
	stats_dlg->ShowWindow(SW_SHOW);
	stats_dlg->SetForegroundWindow();
}

BOOL MainWindow::TrayMessage(DWORD dwMessage)
  {

  CString sTip("Tinelix IRC Client");
  NOTIFYICONDATA tnd;
  tnd.cbSize = sizeof(NOTIFYICONDATA);
  tnd.hWnd = m_hWnd;
  if(IsConnected == TRUE) {
	tnd.uID = IDR_TRAY;
  } else {
	tnd.uID = IDR_TRAY_NC;
  };
  tnd.uFlags = NIF_MESSAGE|NIF_ICON;
  tnd.uCallbackMessage = WM_NOTIFYICON;
  tnd.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
  if(IsConnected == TRUE) {
	VERIFY( tnd.hIcon = LoadIcon(AfxGetInstanceHandle(),
                               MAKEINTRESOURCE (IDR_TRAY)));
  } else {
	VERIFY( tnd.hIcon = LoadIcon(AfxGetInstanceHandle(),
                               MAKEINTRESOURCE (IDR_TRAY_NC)));
  };
  lstrcpyn(tnd.szTip, (LPCTSTR)sTip, sizeof(tnd.szTip));

  return Shell_NotifyIcon(dwMessage, &tnd);

 }

void MainWindow::OnSelchangeIrcChatTabs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int iTab = m_irc_tabs.GetCurSel();
	TC_ITEM tci;
	tci.mask = TCIF_PARAM;
	m_irc_tabs.GetItem(iTab, &tci);
	CWnd* pWnd = (CWnd *)tci.lParam;
	pWnd->ShowWindow(SW_SHOW); 
	
	*pResult = 0;
}

void MainWindow::OnSelchangingIrcChatTabs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	int iTab = m_irc_tabs.GetCurSel();
	TC_ITEM tci;
	tci.mask = TCIF_PARAM;
	m_irc_tabs.GetItem(iTab, &tci);
	CWnd* pWnd = (CWnd *)tci.lParam;
	pWnd->ShowWindow(SW_HIDE); 
	
	*pResult = 0;
}

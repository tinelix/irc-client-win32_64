#include "framework.h"
#include <windows.h>
#include <CommCtrl.h>
#include <wchar.h>
#include "resource.h"
#include "MainWindow.h"
<<<<<<< Updated upstream
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

	IRCClient* application = (IRCClient*)AfxGetApp();
	
	HICON m_hIcon = application->LoadIcon(IDR_MAINFRAME);

	SetIcon(m_hIcon, TRUE);
	
	irc_chat_page = new IRCChatPage;

	stats_dlg = new StatisticsDialog;
	stats_dlg->Create(StatisticsDialog::IDD, this);

	mention_wnd = new MentionWindow;
	mention_wnd->Create(MentionWindow::IDD, this);

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
	
	TC_ITEM tci;
	tci.mask = TCIF_TEXT;
	tci.iImage = -1;
	if(lng_selitemtext_2 == "Russian") {
		tci.pszText = "Поток";
	} else {
		tci.pszText = "Thread";
	};
	m_irc_tabs.InsertItem(0, &tci);
	CRect rcClient, rcWindow;

	m_irc_tabs.GetClientRect(&rcClient);
	m_irc_tabs.AdjustRect(FALSE, &rcClient);
	m_irc_tabs.GetWindowRect(&rcWindow);
	ScreenToClient(rcWindow);

	sended_bytes_count = 0;
	recieved_bytes_count = 0;

	rcClient.OffsetRect(rcWindow.left, rcWindow.top);

	irc_chat_page->Create(IRCChatPage::IDD, &m_irc_tabs);
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
			if (GetVersion() & 0x80000000 && (GetVersion() & 0xFF) ==3) {
				MessageBox("String parsing through the \"PARSER.DLL\" library works only in Windows NT or 9x. Running "
                 "this library in Win32s may cause the program to crash.", "Error", MB_OK);
				FreeLibrary(parserLib);
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

=======
#include "Globals.h"

WCHAR szTitle[256];                  // Текст строки заголовка
WCHAR szWindowClass[256];            // имя класса главного окна

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TINELIXIRC));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TINELIXIRC);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
>>>>>>> Stashed changes
}

HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
<<<<<<< Updated upstream
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
				irc_chat_page->GetDlgItem(IDC_SOCKMSGS)->GetWindowText(previous_listing, 32767);
				if(previous_listing == NULL) {
				};
				sp = sprintf(listing, "%s", previous_listing);
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
							if(i >= new_line_splitter.GetSize() - 1) {
								parsed_msg_index += sprintf(parsed_msg_list + parsed_msg_index, "%s", new_line_splitter[i]);
							} else {
								parsed_msg_index += sprintf(parsed_msg_list + parsed_msg_index, "%s\r\n", new_line_splitter[i]);
							};
						} else {	
							char* unparsed_msg;
							char* parsed_msg;
							unparsed_msg = (char*)calloc(sizeof(char), 32768 + 1);
							parsed_msg = (char*)calloc(sizeof(char), 32768 + 1);
							sprintf(unparsed_msg, "%s", new_line_splitter[i]);
							ParseMessage ParseMsg;
							ParseMsg = (ParseMessage)GetProcAddress((HMODULE)parserLib, "ParseMessage");
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
								//TRACE("OUTPUT: %s\r\n", parsed_msg);
								parsed_msg_index += sprintf(parsed_msg_list + parsed_msg_index, "%s", p_msg);
								if(strcmp(parsing_array[1], "433") == 0) {
									char nick_msg[128];
									sprintf(nick_msg, "NICK %s\r\n", params.reserve_nickname);
									status = send((SOCKET)wParam, nick_msg, strlen(nick_msg), 0);
									parsed_msg_index += sprintf(parsed_msg_list + parsed_msg_index, "%s", "WARNING: We will use next nickname.\r\n");
								} else if(strcmp(parsing_array[1], "PRIVMSG") == 0) {
									try {
										MENTIONED_MSG mentioned_message;
										CString mentioned_message_text(parsing_array[0]);
										sprintf(mentioned_message.mentioner, "%s", mentioned_message_text);
										int message_text_index = 0;
										int mention_index = 0;
										for(int array_index = 3; array_index < 32768; array_index++) {
											try {
												if(array_index == 3 && strlen(parsing_array[array_index]) > 0) {
													message_text_index += sprintf(mentioned_message.message + message_text_index, "%s", parsing_array[array_index]);
												} else if(array_index > 3 && strlen(parsing_array[array_index]) > 0) {
													message_text_index += sprintf(mentioned_message.message + message_text_index, " %s", parsing_array[array_index]);
												};
											} catch(...) {
												array_index = 32768;
											};
										};

										CString parsed_array_part(mentioned_message.message);
										mention_index = parsed_array_part.Find(params.nickname);
										char limited_parsed_array[120];
										if(parsed_array_part.GetLength() <= 120) {
											sprintf(mentioned_message.message, "%s", parsed_array_part);
										} else {
											sprintf(mentioned_message.message, "%s...", parsed_array_part.Right(parsed_array_part.GetLength() - 120));
										};
										if(mention_index != -1) {
											mention_wnd->SendMessage(WM_USER_MENTION, NULL, (LPARAM)&mentioned_message);
										};
									} catch(...) {
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
					if(strlen(previous_listing) + strlen(parsed_msg_list) < 32768) {
						sp += sprintf(listing + sp, "%s",  parsed_msg_list);
					} else {
						sp = sprintf(listing, "%s",  parsed_msg_list);
					};
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
					irc_chat_page->GetDlgItem(IDC_SOCKMSGS)->SetWindowText(listing);
					CEdit* msg_box = (CEdit*)irc_chat_page->GetDlgItem(IDC_SOCKMSGS);
					msg_box->SetSel(0, -1);
					msg_box->SetSel(-1);

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
	} else if(message == WM_NOTIFYICON && lParam == WM_LBUTTONDBLCLK && (wParam == IDR_TRAY || wParam == IDR_TRAY_NC)) {\
		ShowWindow(SW_NORMAL);
		SetForegroundWindow();
		SetFocus();
	} else if(message == WM_PARSER_SWITCH) {
		char *args = (char*)wParam;
		char exe_path[MAX_PATH] = {0};
		char dll_path[MAX_PATH] = {0};
		char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename

		TCHAR language_string[MAX_PATH] = {0};
		TCHAR mini2tray_string[MAX_PATH] = {0};

		GetModuleFileName(NULL, exe_path, MAX_PATH);
		GetModuleFileName(NULL, dll_path, MAX_PATH);

		MainWindow::delsymbs(exe_path, strlen(exe_path) - strlen(exe_name) - 1, strlen(exe_path) - strlen(exe_name) - 1); // deleting EXE filename
		MainWindow::delsymbs(dll_path, strlen(dll_path) - strlen(exe_name) - 1, strlen(dll_path) - strlen(exe_name) - 1); // deleting EXE filename

		strcat(dll_path, "\\parser.dll");
		
		if(strcmp(args, "io") == 0 && parserLib == NULL) {
			parserLib = (HINSTANCE)malloc(sizeof(HINSTANCE));
			parserLib = LoadLibrary(dll_path);
			TRACE("Loaded!\r\n");
		} else if(strcmp(args, "io") == 0) {
			FreeLibrary(parserLib);
			parserLib = NULL;
			TRACE("Unloaded!\r\n");
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
	delete stats_dlg;
	TRACE("Freeing memory...\r\n");
	delete parsing_array;
	return CDialog::DestroyWindow();
=======
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAINWINDOW), NULL, NULL);

    if (hWnd)
    {
        HMENU hSysMenu = GetSystemMenu(hWnd, FALSE);
        InsertMenu(hSysMenu, 5, MF_BYPOSITION, IDM_ABOUT, TEXT("About..."));
        InsertMenu(hSysMenu, 5, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
        SetWindowText(hWnd, szTitle);
        HWND tabctrl = GetDlgItem(hWnd, IDC_THREADTABS);
        TCITEM tci = { 0 };
        tci.mask = TCIF_TEXT;
        tci.pszText = (LPWSTR)L"Thread";
        TabCtrl_InsertItem(tabctrl, 0, &tci);
        HMENU hMainMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDC_TINELIXIRC));
        SetMenu(hWnd, hMainMenu);

    }
    else {
        MessageBox(hWnd, TEXT("Something wen\'t wrong..."), TEXT("Error"), MB_ICONERROR | MB_OK);
    }

    return hWnd;
>>>>>>> Stashed changes
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
<<<<<<< Updated upstream
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
	
=======
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
>>>>>>> Stashed changes
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
<<<<<<< Updated upstream
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
=======
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
>>>>>>> Stashed changes

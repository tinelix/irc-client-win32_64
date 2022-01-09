// IRCChatPage.cpp : implementation file
//

#include "stdafx.h"
#include "TinelixIRC.h"
#include "IRCChatPage.h"

#include <stdio.h>
#include <iostream.h>
#include <winsock.h>
#include <process.h>
#include <direct.h>
#include <locale.h>
#include <afxtempl.h>
#include <conio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// IRCChatPage dialog


IRCChatPage::IRCChatPage(CWnd* pParent /*=NULL*/)
	: CDialog(IRCChatPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(IRCChatPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void IRCChatPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(IRCChatPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(IRCChatPage, CDialog)
	//{{AFX_MSG_MAP(IRCChatPage)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_MSGTEXT, OnChangeMsgtext)
	ON_BN_CLICKED(IDC_SENDMSG, OnSendmsg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// IRCChatPage message handlers

//void IRCChatPage::delsymbs(char *str, int begin, int lng)
//{
	//for(begin; begin < strlen(str); begin++) {
		//*(str + begin) = 0;	
	//};
//};

void IRCChatPage::delsymbs(char *str, int begin, int lng)
{
	for(begin; begin < strlen(str); begin++) {
		*(str + begin) = 0;	
	};
};

void IRCChatPage::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	
}

void IRCChatPage::OnSize(UINT nType, int cx, int cy) 
{
	
	CDialog::OnSize(nType, cx, cy);
	
}

void IRCChatPage::OnChangeMsgtext() 
{
	char msg_text[1024] = {0};
	GetDlgItem(IDC_MSGTEXT)->GetWindowText(msg_text, 1024);
	CString msg_txt(msg_text);
	if (msg_txt != "" && msg_txt != " ") {
		GetDlgItem(IDC_SENDMSG)->EnableWindow(TRUE);
	} else {
		GetDlgItem(IDC_SENDMSG)->EnableWindow(FALSE);
	};
	
}

void IRCChatPage::OnSendmsg() 
{
	WPARAM wp;
	LPARAM lp;
	char exe_path[MAX_PATH] = {0};
	char exe_name[MAX_PATH] = "TLX_IRC.EXE"; // EXE filename

	GetModuleFileName(NULL, exe_path, MAX_PATH);  

	*(strrchr(exe_path, '\\')+1)='\0'; // deleting EXE filename

	strcat(exe_path, "\\settings.ini");	// add settings filename

	TCHAR language_string[MAX_PATH] = {0};
	CMenu* mainmenu;
	
	WSADATA WSAData;
	char szTemp[80];

	int status = (WSAStartup(MAKEWORD(1,1), &WSAData));

	GetPrivateProfileString("Main", "Language", "English", language_string, MAX_PATH, exe_path);

	CString language_string_2(language_string);
	
	char msg_text[4096] = {0};
	char socks_msg[32768] = {0};
	GetDlgItem(IDC_SOCKMSGS)->GetWindowText(socks_msg, 32767);
	GetDlgItem(IDC_MSGTEXT)->GetWindowText(msg_text, 4096);
	CString msg_txt(msg_text);
	CArray<CString, CString> spaces_splitter;
	int string_index = 0;
	while(AfxExtractSubString(msg_txt, msg_text, string_index, ' '))
	{
		if(strlen(msg_txt) > 0) {
			spaces_splitter.Add(msg_txt);
		};
		++string_index;
	};
	
	CString first_symbol_in_args;
	CString args;
	CString slash;

	if(spaces_splitter.GetSize() >= 0) {
		exit;
	};

	slash = spaces_splitter[0][0];

	if(spaces_splitter.GetSize() >= 2) {
			first_symbol_in_args = spaces_splitter[1][0];
			for (int i = 1; i > spaces_splitter.GetSize(); i++) {
		  		args += spaces_splitter[i];
				if(i < (spaces_splitter.GetSize() - 1)) {
					args += " ";
				};
			}; 
	} else {
		first_symbol_in_args = "";
		args = "";
	};


	if(spaces_splitter[0] == "/join" && first_symbol_in_args != "#") {
		if(spaces_splitter.GetSize() > 1) {
			char join_msg[1024] = {0};
			sprintf(join_msg, "JOIN #%s\r\n", spaces_splitter[1]);
			sprintf(channel, "#%s", spaces_splitter[1]);
			AfxGetMainWnd()->SendMessage(WM_SENDING_SOCKET_MESSAGE, (WPARAM)join_msg, NULL);
		} else {
			if(language_string_2 == "Russian") {
				MessageBox("Пожалуйста, укажите аргумент к данной команде.", "Ошибка", MB_OK | MB_ICONSTOP);
			} else {
				MessageBox("Please enter an argument for this command.", "Error", MB_OK | MB_ICONSTOP);
			};
		};
	} else if(spaces_splitter[0] == "/join" && first_symbol_in_args == "#") {
		if(spaces_splitter.GetSize() > 1) {
            char join_msg[1024] = {0};
            sprintf(join_msg, "JOIN %s\r\n", spaces_splitter[1]);
            sprintf(channel, "%s", spaces_splitter[1]);
            AfxGetMainWnd()->SendMessage(WM_SENDING_SOCKET_MESSAGE, (WPARAM)join_msg, (LPARAM)join_msg);
        } else {
			if(language_string_2 == "Russian") {
				MessageBox("Пожалуйста, укажите аргумент к данной команде.", "Ошибка", MB_OK | MB_ICONSTOP);
			} else {
				MessageBox("Please enter an argument for this command.", "Error", MB_OK | MB_ICONSTOP);
			};
		};
	} else if(spaces_splitter[0] == "/part" && first_symbol_in_args != "#") {
        if(spaces_splitter.GetSize() > 1) {
            char part_msg[1024] = {0};
            sprintf(part_msg, "PART #%s\r\n", spaces_splitter[1]);
            sprintf(channel, "#%s", spaces_splitter[1]);
            AfxGetMainWnd()->SendMessage(WM_SENDING_SOCKET_MESSAGE, (WPARAM)part_msg, lp);
        } else {
			if(language_string_2 == "Russian") {
				MessageBox("Пожалуйста, укажите аргумент к данной команде.", "Ошибка", MB_OK | MB_ICONSTOP);
			} else {
				MessageBox("Please enter an argument for this command.", "Error", MB_OK | MB_ICONSTOP);
			};
		};
	} else if(spaces_splitter[0] == "/names" && first_symbol_in_args == "#") {
		if(spaces_splitter.GetSize() > 1) {
            char join_msg[1024] = {0};
            sprintf(join_msg, "NAMES %s\r\n", spaces_splitter[1]);
            sprintf(channel, "%s", spaces_splitter[1]);
            AfxGetMainWnd()->SendMessage(WM_SENDING_SOCKET_MESSAGE, (WPARAM)join_msg, (LPARAM)join_msg);
        } else {
			if(language_string_2 == "Russian") {
				MessageBox("Пожалуйста, укажите аргумент к данной команде.", "Ошибка", MB_OK | MB_ICONSTOP);
			} else {
				MessageBox("Please enter an argument for this command.", "Error", MB_OK | MB_ICONSTOP);
			};
		};
	} else if(spaces_splitter[0] == "/part" && first_symbol_in_args == "#") {
        if(spaces_splitter.GetSize() > 1) {
            char part_msg[1024] = {0};
            sprintf(part_msg, "PART %s\r\n", spaces_splitter[1]);
            sprintf(channel, "%s", spaces_splitter[1]);
            AfxGetMainWnd()->SendMessage(WM_SENDING_SOCKET_MESSAGE, (WPARAM)part_msg, lp);
        } else {
            if(language_string_2 == "Russian") {
				MessageBox("Пожалуйста, укажите аргумент к данной команде.", "Ошибка", MB_OK | MB_ICONSTOP);
			} else {
				MessageBox("Please enter an argument for this command.", "Error", MB_OK | MB_ICONSTOP);
			};
        };
	} else if(spaces_splitter[0] == "/whois" && first_symbol_in_args != "#") {
        if(spaces_splitter.GetSize() > 1) {
            char whois_msg[1024] = {0};
            sprintf(whois_msg, "WHOIS %s\r\n", spaces_splitter[1]);
            AfxGetMainWnd()->SendMessage(WM_SENDING_SOCKET_MESSAGE, (WPARAM)whois_msg, lp);
        } else {
            if(language_string_2 == "Russian") {
				MessageBox("Пожалуйста, укажите аргумент к данной команде.", "Ошибка", MB_OK | MB_ICONSTOP);
			} else {
				MessageBox("Please enter an argument for this command.", "Error", MB_OK | MB_ICONSTOP);
			};
        };
	} else if(spaces_splitter[0] == "/nick" && first_symbol_in_args != "#") {
        if(spaces_splitter.GetSize() > 1) {
            char nick_msg[1024] = {0};
            sprintf(nick_msg, "NICK %s\r\n", spaces_splitter[1]);
            AfxGetMainWnd()->SendMessage(WM_SENDING_SOCKET_MESSAGE, (WPARAM)nick_msg, lp);
        } else {
            if(language_string_2 == "Russian") {
				MessageBox("Пожалуйста, укажите аргумент к данной команде.", "Ошибка", MB_OK | MB_ICONSTOP);
			} else {
				MessageBox("Please enter an argument for this command.", "Error", MB_OK | MB_ICONSTOP);
			};
        };
	} else if(spaces_splitter[0] == "/ping" && first_symbol_in_args != "#") {
        if(spaces_splitter.GetSize() > 1) {
            char ping_msg[1024] = {0};
            sprintf(ping_msg, "PING %s\r\n", spaces_splitter[1]);
            AfxGetMainWnd()->SendMessage(WM_SENDING_SOCKET_MESSAGE, (WPARAM)ping_msg, lp);
        } else {
            if(language_string_2 == "Russian") {
				MessageBox("Пожалуйста, укажите аргумент к данной команде.", "Ошибка", MB_OK | MB_ICONSTOP);
			} else {
				MessageBox("Please enter an argument for this command.", "Error", MB_OK | MB_ICONSTOP);
			};
        };
	} else if(spaces_splitter[0] == "/nickserv" && first_symbol_in_args != "#") {
		char nickserv_msg[1024] = {0};
		int msg_index = 0;
		if(spaces_splitter.GetSize() > 2) {
			msg_index = sprintf(nickserv_msg, "NICKSERV %s", spaces_splitter[1]);
			for (int i = 2; i < spaces_splitter.GetSize(); i++) {
				if(i == spaces_splitter.GetSize() - 1) {
					msg_index += sprintf(nickserv_msg + msg_index, " %s\r\n", spaces_splitter[i]);
				} else if (i < spaces_splitter.GetSize() - 1) {
					msg_index += sprintf(nickserv_msg + msg_index, " %s", spaces_splitter[i]);
				};
			};
			TRACE(nickserv_msg);
			AfxGetMainWnd()->SendMessage(WM_SENDING_SOCKET_MESSAGE, (WPARAM)nickserv_msg, lp);
		};
	} else if(spaces_splitter[0] == "/mode" && first_symbol_in_args != "#") {
		char mode_msg[1024] = {0};
		int msg_index = 0;
		if(spaces_splitter.GetSize() > 2) {
			msg_index = sprintf(mode_msg, "MODE %s", spaces_splitter[1]);
			for (int i = 2; i < spaces_splitter.GetSize(); i++) {
				if(i == spaces_splitter.GetSize() - 1) {
					msg_index += sprintf(mode_msg + msg_index, " %s\r\n", spaces_splitter[i]);
				} else if (i < spaces_splitter.GetSize() - 1) {
					msg_index += sprintf(mode_msg + msg_index, " %s", spaces_splitter[i]);
				};
			};
			AfxGetMainWnd()->SendMessage(WM_SENDING_SOCKET_MESSAGE, (WPARAM)mode_msg, lp);
		};
	} else if(spaces_splitter[0] == "/msg") {
		char cmd_msg[1024] = {0};
		int msg_index = 0;
		if(spaces_splitter.GetSize() > 2) {
			msg_index = sprintf(cmd_msg, "PRIVMSG %s", spaces_splitter[1]);
			for (int i = 2; i < spaces_splitter.GetSize(); i++) {
				if(i == spaces_splitter.GetSize() - 1) {
					msg_index += sprintf(cmd_msg + msg_index, " %s\r\n", spaces_splitter[i]);
				} else if (i < spaces_splitter.GetSize() - 1) {
					msg_index += sprintf(cmd_msg + msg_index, " %s", spaces_splitter[i]);
				};
			};
			AfxGetMainWnd()->SendMessage(WM_SENDING_SOCKET_MESSAGE, (WPARAM)cmd_msg, lp);
		};
	} else if(spaces_splitter[0] == "/privmsg") {
		char cmd_msg[1024] = {0};
		int msg_index = 0;
		if(spaces_splitter.GetSize() > 2) {
			msg_index = sprintf(cmd_msg, "PRIVMSG %s", spaces_splitter[1]);
			for (int i = 2; i < spaces_splitter.GetSize(); i++) {
				if(i == spaces_splitter.GetSize() - 1) {
					msg_index += sprintf(cmd_msg + msg_index, " %s\r\n", spaces_splitter[i]);
				} else if (i < spaces_splitter.GetSize() - 1) {
					msg_index += sprintf(cmd_msg + msg_index, " %s", spaces_splitter[i]);
				};
			};
			AfxGetMainWnd()->SendMessage(WM_SENDING_SOCKET_MESSAGE, (WPARAM)cmd_msg, lp);
		};
	} else if(spaces_splitter[0] == "/quit") {
		AfxGetMainWnd()->SendMessage(WM_SENDING_QUIT_MESSAGE, NULL, lp);
	} else if(slash == "/") {
				char cmd_msg[1024] = {0};
				int msg_index;
				msg_index = sprintf(cmd_msg, "%s", spaces_splitter[0].Right(strlen(spaces_splitter[0]) - 1));
				if(args != "") {
					msg_index += sprintf(cmd_msg + msg_index, " %s\r\n", args);
				} else {
					msg_index += sprintf(cmd_msg + msg_index, "\r\n");
				};
				AfxGetMainWnd()->SendMessage(WM_SENDING_SOCKET_MESSAGE, (WPARAM)cmd_msg, lp);	
	} else {
	   	char msg[1024] = {0};
		int msg_index;
		if(strcmp(channel, "(None)") != 0) {
	   		msg_index = sprintf(msg, "PRIVMSG %s", channel);
			msg_index += sprintf(msg + msg_index, " :%s\r\n", msg_text);
			TRACE("[%s]", msg);
			//int status = send(sock, msg, strlen(msg), 0);
			AfxGetMainWnd()->SendMessage(WM_SENDING_SOCKET_MESSAGE, (WPARAM)msg, lp);
		};
	};
	char listing[32768] = {0};
	sprintf(listing, "%sYou: %s\r\n", socks_msg, msg_text);
	CString listing_2(listing);
	GetDlgItem(IDC_SOCKMSGS)->SetWindowText(listing_2);
	GetDlgItem(IDC_MSGTEXT)->SetWindowText("");
	CEdit* msg_box = (CEdit*)GetDlgItem(IDC_SOCKMSGS);
	msg_box->SetSel(0, -1);
	msg_box->SetSel(-1);	
}


void IRCChatPage::OnOK() 
{
	IRCChatPage::OnSendmsg();
};

void IRCChatPage::OnCancel()
{
	//CDialog::OnCancel();
	TRACE("ESC");
};

BOOL IRCChatPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	sprintf(channel, "(None)");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void IRCChatPage::OnClose() 
{

}

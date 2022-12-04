#define MAINWND_H

#include <windows.h>

<<<<<<< Updated upstream
#define WM_SOCKET_MESSAGE 0xAFFF
#define WM_SENDING_SOCKET_MESSAGE 0xAFFE
#define WM_CHANGING_MSGFONT 0xAFFD
#define WM_UPDATING_STATISTICS 0xAFFC
#define WM_SENDING_QUIT_MESSAGE 0xAFFB
#define WM_PARSER_SWITCH 0xAFFA
#define WM_NOTIFYICON (WM_USER+2)
#define WM_SOCKET_TIMER 1;
#define WM_USER_MENTION 0xAFFB


#include "IRCChatPage.h"
#include "StatisticsDialog.h"
#include "MentionWindow.h"

class MainWindow : public CDialog
{
// Construction
public:
	MainWindow(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MainWindow)
	enum { IDD = IDD_MAINWINDOW };
	CTabCtrl	m_irc_tabs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MainWindow)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	IRCChatPage* irc_chat_page;
	StatisticsDialog* stats_dlg;
	MentionWindow* mention_wnd;
	char** parsing_array;
	CFont font;
	CFont mainfont;

	// Generated message map functions
	//{{AFX_MSG(MainWindow)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileConnect();
	afx_msg void OnViewSettings();
	afx_msg void OnHelpAbout();
	afx_msg void OnFileQuit();
	afx_msg void OnClose();
	afx_msg void OnFileStatistics();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	struct PARAMETERS
	{
		char server[256];
		char nickname[80];
		char reserve_nickname[80];
		char realname[80];
		int port;
		HWND hwnd;
		char quit_msg[512];
		BOOL hide_ip;
	};
	struct IRC_STATS
	{
		int sended_bytes;
		int recieved_bytes;
	};
	struct MENTIONED_MSG 
	{
		char mentioner[128];
		char message[512];
	};
	PARAMETERS params;
	char channel[256];
	char exe_path[MAX_PATH];
	char history_path[MAX_PATH];
	char history_path2[MAX_PATH];
	char history_file[MAX_PATH];
	time_t current_time;
	CStdioFile history_file_stdio;
	BOOL IsConnected;
	HWND hwnd_chat;
	int sended_bytes_count;
	int recieved_bytes_count;
	void MainWindow::ConnectionFunc(HWND hwnd, PARAMETERS params);
	void MainWindow::delsymbs(char *str, int begin, int lng);
	void MainWindow::CreateConnectionThread(PARAMETERS params);
	UINT MainWindow::SendSocketMessage(char *str);
	void MainWindow::OnOK();
	void MainWindow::OnCancel();
	BOOL MainWindow::TrayMessage(DWORD dwMessage);
	HINSTANCE parserLib;

friend class IRCChatPage;
friend class StatisticsDialog;
friend class SettingsDialog;
friend class ConnectionManagerDialog;
};
=======
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

>>>>>>> Stashed changes

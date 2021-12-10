// IRCChatPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// IRCChatPage dialog

#define WM_SENDING_SOCKET_MESSAGE 0xAFFE

class IRCChatPage : public CDialog
{
// Construction
public:
	IRCChatPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(IRCChatPage)
	enum { IDD = IDD_IRCCHATPAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(IRCChatPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(IRCChatPage)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChangeMsgtext();
	afx_msg void OnSendmsg();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void IRCChatPage::delsymbs(char *str, int begin, int lng);
	void IRCChatPage::OnOK();
	char channel[256];
	char exe_path[MAX_PATH];
	char history_path[MAX_PATH];
	char history_path2[MAX_PATH];
	char history_file[MAX_PATH];
	time_t current_time;

};

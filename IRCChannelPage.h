// IRCChannelPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// IRCChannelPage dialog

#define WM_SENDING_SOCKET_MESSAGE 0xAFFE
#define WM_SENDING_QUIT_MESSAGE 0xAFFB
#define WM_JOINING_TO_CHANNEL 0xAFEE
#define WM_LEAVING_CHANNEL 0xAFED

class IRCChannelPage : public CDialog
{
// Construction
public:
	IRCChannelPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(IRCChannelPage)
	enum { IDD = IDD_IRCCHANNELPAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(IRCChannelPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(IRCChannelPage)
	afx_msg void OnSendmsg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	char channel[256];
	char exe_path[MAX_PATH];
	char history_path[MAX_PATH];
	char history_path2[MAX_PATH];
	char history_file[MAX_PATH];
	time_t current_time;
	void IRCChannelPage::OnOK();
	void IRCChannelPage::OnCancel(); 
};

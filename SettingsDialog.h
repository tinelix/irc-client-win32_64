// SettingsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SettingsDialog dialog

#define WM_CHANGING_MSGFONT 0xAFFD

class SettingsDialog : public CDialog
{
// Construction
public:
	SettingsDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SettingsDialog)
	enum { IDD = IDD_SETTINGSDLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SettingsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SettingsDialog::delsymbs(char *str, int begin, int lng);
	// Generated message map functions
	//{{AFX_MSG(SettingsDialog)
	virtual void OnOK();
	afx_msg void OnChangeMsgFont();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CFont mainfont;
	char font_family[128];
	int font_size;
	int font_weight;
	char font_key[160];

private:
	CFont msg_font;

friend class MainWindow;

};

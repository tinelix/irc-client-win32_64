// AboutDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// AboutDialog dialog

class AboutDialog : public CDialog
{
// Construction
public:
	AboutDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(AboutDialog)
	enum { IDD = IDD_ABOUTDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AboutDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont mainfont;
	CFont boldfont;

	// Generated message map functions
	//{{AFX_MSG(AboutDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void AboutDialog::delsymbs(char *str, int begin, int lng);
};

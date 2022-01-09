// InfoMessageDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// InfoMessageDialog dialog

#define WM_CLOSE_INFOMSG 0xAFEF

class InfoMessageDialog : public CDialog
{
// Construction
public:
	InfoMessageDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(InfoMessageDialog)
	enum { IDD = IDD_INFOMSG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(InfoMessageDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(InfoMessageDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int infomsg_code;
	void SetInfoMessageCode(int code);

friend class ConnectionManagerDialog;
};

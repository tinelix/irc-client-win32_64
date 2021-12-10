// EditDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EditDialog dialog

class EditDialog : public CDialog
{
// Construction
public:
	EditDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(EditDialog)
	enum { IDD = IDD_EDITTEXT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EditDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EditDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void delsymbs(char *str, int begin, int lng);
	void SetSettingProfileState(BOOL value);
	void SetProfileName(char* name);
	BOOL setting_profile;
	char profilename[80];

};

#if !defined(AFX_TESTDLG_H__5495DE91_FC66_48AD_B74C_D7720D7D039A__INCLUDED_)
#define AFX_TESTDLG_H__5495DE91_FC66_48AD_B74C_D7720D7D039A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestDlg.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CCommTest

struct CommTest
{
	int		m_id;
	CWnd*	m_receiver;
	BOOL*	m_loopAbort;
};


/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

class CTestDlg : public CDialog
{
// Construction
public:
	CTestDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestDlg)
	enum { IDD = IDD_TEST_DLG };
	CString	m_status;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	LRESULT		OnScopeUpdate(WPARAM wParam, LPARAM lParam);
	BOOL		m_loopAbort;
	CommTest	m_commTest;


protected:

	// Generated message map functions
	//{{AFX_MSG(CTestDlg)
	afx_msg void OnLoopAbort();
	afx_msg void OnStartThread();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLG_H__5495DE91_FC66_48AD_B74C_D7720D7D039A__INCLUDED_)

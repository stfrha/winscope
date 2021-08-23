#if !defined(AFX_SETBUFFSIZEDLG_H__2D7BB615_9B19_4E37_91A6_95C3E27C055A__INCLUDED_)
#define AFX_SETBUFFSIZEDLG_H__2D7BB615_9B19_4E37_91A6_95C3E27C055A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetBuffSizeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetBuffSizeDlg dialog

class CSetBuffSizeDlg : public CDialog
{
// Construction
public:
	CSetBuffSizeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetBuffSizeDlg)
	enum { IDD = IDD_BUFFER_SETTING };
	UINT	m_bufferSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetBuffSizeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetBuffSizeDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETBUFFSIZEDLG_H__2D7BB615_9B19_4E37_91A6_95C3E27C055A__INCLUDED_)

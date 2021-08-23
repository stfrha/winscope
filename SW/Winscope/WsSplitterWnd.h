#if !defined(AFX_MYSPLITTERWND_H__138E6707_DE1A_11D2_BD45_0000F8771AA3__INCLUDED_)
#define AFX_MYSPLITTERWND_H__138E6707_DE1A_11D2_BD45_0000F8771AA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySplitterWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWsSplitterWnd window

class CWsSplitterWnd : public CSplitterWnd
{
// Construction
public:
	CWsSplitterWnd();

// Attributes
public:


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWsSplitterWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWsSplitterWnd();
	// Generated message map functions
protected:
	//{{AFX_MSG(CWsSplitterWnd)
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSPLITTERWND_H__138E6707_DE1A_11D2_BD45_0000F8771AA3__INCLUDED_)



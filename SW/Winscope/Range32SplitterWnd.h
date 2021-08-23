#if !defined(AFX_RANGE32SPLITTERWND_H__05D2B875_2BCE_48A2_9270_6AD01B7D3742__INCLUDED_)
#define AFX_RANGE32SPLITTERWND_H__05D2B875_2BCE_48A2_9270_6AD01B7D3742__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Range32SplitterWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRange32SplitterWnd frame with splitter

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CRange32SplitterWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CRange32SplitterWnd)
protected:
	CRange32SplitterWnd();           // protected constructor used by dynamic creation

// Attributes
protected:
	CSplitterWnd    m_wndSplitter;
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRange32SplitterWnd)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRange32SplitterWnd();

	// Generated message map functions
	//{{AFX_MSG(CRange32SplitterWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RANGE32SPLITTERWND_H__05D2B875_2BCE_48A2_9270_6AD01B7D3742__INCLUDED_)

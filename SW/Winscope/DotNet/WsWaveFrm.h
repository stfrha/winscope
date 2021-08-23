// WsWaveFrm.h : interface of the CWsWaveFrm class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WSWAVEFRM_H__173FC038_897C_458A_B53A_249ED771E978__INCLUDED_)
#define AFX_WSWAVEFRM_H__173FC038_897C_458A_B53A_249ED771E978__INCLUDED_

#include "WsSplitterWnd.h"
//class CWsSplitterWnd;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWsWaveFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CWsWaveFrm)
public:
	CWsWaveFrm();

// Attributes
protected:
	CWsSplitterWnd m_wndSplitter;
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWsWaveFrm)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWsWaveFrm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CWsWaveFrm)
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSWAVEFRM_H__173FC038_897C_458A_B53A_249ED771E978__INCLUDED_)

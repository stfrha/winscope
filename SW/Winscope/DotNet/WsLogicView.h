#if !defined(AFX_WSLOGICVIEW_H__5B453A00_94B8_4CAF_BFE6_1B6257C6F8EA__INCLUDED_)
#define AFX_WSLOGICVIEW_H__5B453A00_94B8_4CAF_BFE6_1B6257C6F8EA__INCLUDED_

#include "WsWaveDoc.h"
#include "WsWaveDataView.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WsLogicView.h : header file
//

// Logic View constants
#define LVC_VECTOR_SLOPE_COMP	1
#define LVC_ROW_MARGIN			3


class CWsLogicLabelView;

/////////////////////////////////////////////////////////////////////////////
// CWsLogicView view

class CWsLogicView : public CWsWaveDataView
{
protected:
	CWsLogicView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWsLogicView)

// Attributes
public:
	CWsLogicLabelView*	m_logicLabelView;
	int					m_dragState;


// Operations
public:
	CWsWaveDoc*			GetDocument();
	void				OnChangedSize( void );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWsLogicView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CWsLogicView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CWsLogicView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSLOGICVIEW_H__5B453A00_94B8_4CAF_BFE6_1B6257C6F8EA__INCLUDED_)

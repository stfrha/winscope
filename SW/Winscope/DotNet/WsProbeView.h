// WsProbeView.h : interface of the CWsProbeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WsProbeView_H__62FAF804_0AB6_4F26_BFAF_AB27C3DCB9F4__INCLUDED_)
#define AFX_WsProbeView_H__62FAF804_0AB6_4F26_BFAF_AB27C3DCB9F4__INCLUDED_

#include "WsWaveDoc.h"
#include "WsWaveDataView.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PV_CURSOR_GRAB_DISTANCE	5

// Drag state
#define DS_NONE					0x00
#define DS_DRAG_X_TIME_CURSOR	0x01
#define DS_DRAG_Y_TIME_CURSOR	0x02
#define DS_DRAG_X_VOLT_CURSOR	0x04
#define DS_DRAG_Y_VOLT_CURSOR	0x08

class CWsProbeInfoView;

class CWsProbeView : public CWsWaveDataView
{
protected: // create from serialization only
	CWsProbeView();
	DECLARE_DYNCREATE(CWsProbeView)

// Attributes
public:
	CWsProbeInfoView*	m_probeInfoView;
	long				m_x;
	int					m_timer;
	unsigned int		m_dragState;

// Operations
public:
	CWsWaveDoc*			GetDocument();

	LRESULT				OnThreadAborted(WPARAM wParam, LPARAM lParam);
	LRESULT				OnScopeError(WPARAM wParam, LPARAM lParam);
	LRESULT				OnNewDataAvailable(WPARAM wParam, LPARAM lParam);
	LRESULT				OnScopeDone(WPARAM wParam, LPARAM lParam);
	LRESULT				OnThreadTerminated(WPARAM wParam, LPARAM lParam);
	LRESULT				OnHwNotConfigured(WPARAM wParam, LPARAM lParam);
	LRESULT				OnSweepStateArmed(WPARAM wParam, LPARAM lParam);
	LRESULT				OnSweepStateFull(WPARAM wParam, LPARAM lParam);
	void				OnChangedSize( void );
	int					GetViewportCenter( void );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWsProbeView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWsProbeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWsProbeView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WsProbeView.cpp
inline CWsWaveDoc* CWsProbeView::GetDocument()
   { return (CWsWaveDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WsProbeView_H__62FAF804_0AB6_4F26_BFAF_AB27C3DCB9F4__INCLUDED_)

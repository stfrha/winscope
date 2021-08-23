#if !defined(AFX_WSPROBEINFOVIEW_H__C18E1CC5_8553_456C_969F_D10F39AA5F08__INCLUDED_)
#define AFX_WSPROBEINFOVIEW_H__C18E1CC5_8553_456C_969F_D10F39AA5F08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WsProbeInfoView.h : header file
//

// Probe Info View Drag States
#define	PIV_DS_IDLE			0x00
#define PIV_DS_DRAG_A_GND	0x01
#define PIV_DS_DRAG_B_GND	0x02
#define PIV_DS_DRAG_A_TRIGG	0x03
#define PIV_DS_DRAG_B_TRIGG	0x04

//Cursor frame sizes
#define	CFS_TOP_MOST_BASE	7
#define	CFS_DELTA			64
#define	CFS_HEIGHT			55
#define	CFS_LEFT			3
#define	CFS_WIDTH			87
#define	CFS_ON_BOX_LEFT		76
#define	CFS_ON_BOX_HEIGHT	9
#define	CFS_ON_BOX_WIDTH	9
#define	CFS_CAPTION_LEFT	8
#define	CFS_TOP_TEXT_Y		20
#define	CFS_BOTT_TEXT_Y		40
#define	CFS_TEXT_LEFT_COL	15
#define	CFS_TEXT_RIGHT_COL	60

//Dragable position and trigg level mouse capture window sizes
#define	DMCS_DIFF_Y_CENTER	5
#define	DMCS_DIFF_FROM_RIGHT	10




/////////////////////////////////////////////////////////////////////////////
// CWsProbeInfoView view

class CWsProbeInfoView : public CView
{
protected:
	CWsProbeInfoView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWsProbeInfoView)

// Attributes
public:
	CPoint			m_scrollPosition;
	int				m_dragState;
	BOOL			m_cursorXOnBoxDepressed;
	BOOL			m_cursorYOnBoxDepressed;

// Operations
public:
	void			SetProbeScrollPosition(CPoint p);
	CSplitterWnd*	m_splitterWnd;
	void			PaintFrames(CDC* pDC, COLORREF textColor, int frameBase, 
								   const char* caption, const char* tls, const char* trs, 
								   const char* bls, const char* brs, BOOL on, BOOL depressed);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWsProbeInfoView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CWsProbeInfoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CWsProbeInfoView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSPROBEINFOVIEW_H__C18E1CC5_8553_456C_969F_D10F39AA5F08__INCLUDED_)

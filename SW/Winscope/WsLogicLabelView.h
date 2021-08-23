#if !defined(AFX_WSLOGICLABELVIEW_H__E82C7BC2_E0FB_40F1_BCB6_34C7BD4E35D1__INCLUDED_)
#define AFX_WSLOGICLABELVIEW_H__E82C7BC2_E0FB_40F1_BCB6_34C7BD4E35D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WsLogicLabelView.h : header file
//

class CWsLogicView;

/////////////////////////////////////////////////////////////////////////////
// CWsLogicLabelView view

class CWsLogicLabelView : public CScrollView
{
protected:
	CWsLogicLabelView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWsLogicLabelView)

// Attributes
public:
	CString				m_labelFontName;
	int					m_labelFontSize;
	CFont				m_labelFont;
	CWsLogicView*		m_logicView;

// Operations
public:
	CWsWaveDoc*			GetDocument();

	void				OnChangedSize( void );
	int					GetLabelHeight(CDC* dc);
	CSize				GetMaxSize(CDC* dc);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWsLogicLabelView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CWsLogicLabelView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CWsLogicLabelView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSLOGICLABELVIEW_H__E82C7BC2_E0FB_40F1_BCB6_34C7BD4E35D1__INCLUDED_)

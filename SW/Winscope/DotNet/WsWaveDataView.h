#if !defined(AFX_WSWAVEDATAVIEW_H__3A3C395C_0C65_48A1_A1E1_9DEC615D534B__INCLUDED_)
#define AFX_WSWAVEDATAVIEW_H__3A3C395C_0C65_48A1_A1E1_9DEC615D534B__INCLUDED_

#include "WsWaveDoc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WsWaveDataView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWsWaveDataView view

class CWsWaveDataView : public CScrollView
{
protected:
	CWsWaveDataView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWsWaveDataView)

// Attributes
public:
	CSplitterWnd*		m_splitterWnd;
	long				m_oldScrollPosition;
	int					m_posSetByZoom;

// Operations
public:
	CWsWaveDoc*			GetDocument();

	void				PreScrollPrepare( void );
	void				PostScrollPosition(int mouseX, double oldScaleFactor);
	int					GetViewportSize( void );


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWsWaveDataView)
	protected:
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CWsWaveDataView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CWsWaveDataView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSWAVEDATAVIEW_H__3A3C395C_0C65_48A1_A1E1_9DEC615D534B__INCLUDED_)

#if !defined(AFX_LOGICLABELVIEW2_H__2F52E222_19D5_48BB_9EA6_D32DA2C51FEC__INCLUDED_)
#define AFX_LOGICLABELVIEW2_H__2F52E222_19D5_48BB_9EA6_D32DA2C51FEC__INCLUDED_

#include <afxcview.h>


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogicLabelView2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogicLabelView2 view

class CWsLogicView;

class CLogicLabelView2 : public CTreeView
{
protected:
	CLogicLabelView2();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLogicLabelView2)

// Attributes
public:
	CWsLogicView*		m_logicView;

// Operations
public:
	int					GetLabelHeight(CDC* dc);
	CSize				GetMaxSize(CDC* dc);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogicLabelView2)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLogicLabelView2();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CLogicLabelView2)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGICLABELVIEW2_H__2F52E222_19D5_48BB_9EA6_D32DA2C51FEC__INCLUDED_)

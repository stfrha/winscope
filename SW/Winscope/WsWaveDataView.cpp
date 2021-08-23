// WsWaveDataView.cpp : implementation file
//

#include "stdafx.h"
#include "winscope.h"
#include "WsWaveDataView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWsWaveDataView

IMPLEMENT_DYNCREATE(CWsWaveDataView, CScrollView)

CWsWaveDataView::CWsWaveDataView()
{
}

CWsWaveDataView::~CWsWaveDataView()
{
}


BEGIN_MESSAGE_MAP(CWsWaveDataView, CScrollView)
	//{{AFX_MSG_MAP(CWsWaveDataView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWsWaveDataView diagnostics

#ifdef _DEBUG
void CWsWaveDataView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CWsWaveDataView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

#endif //_DEBUG

CWsWaveDoc* CWsWaveDataView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWsWaveDoc)));
	return (CWsWaveDoc*)m_pDocument;
}


/////////////////////////////////////////////////////////////////////////////
// CWsWaveDataView message handlers

void CWsWaveDataView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
}

void	CWsWaveDataView::PreScrollPrepare( void )
{
	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_oldScrollPosition = GetScrollPosition().x;
}

void	CWsWaveDataView::PostScrollPosition(int mouseX, double oldScaleFactor)
{
	double		newPos;

	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	newPos = ((double) (m_oldScrollPosition + mouseX)) * pDoc->m_viewScaleFactor / oldScaleFactor - (double) mouseX;

	SetScrollPos(SB_HORZ, (long) newPos, TRUE);
}

int	CWsWaveDataView::GetViewportSize( void )
{
	int				cx;
	int				dummy;

	m_splitterWnd->GetColumnInfo(1, cx, dummy);

	return cx;
}




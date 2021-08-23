// WsLogicLabelView.cpp : implementation file
//

#include "stdafx.h"
#include "Winscope.h"
#include "WsWaveDoc.h"
#include "WsLogicView.h"
#include "WsLogicLabelView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWsLogicLabelView

IMPLEMENT_DYNCREATE(CWsLogicLabelView, CScrollView)

CWsLogicLabelView::CWsLogicLabelView()
{
}

CWsLogicLabelView::~CWsLogicLabelView()
{
}


BEGIN_MESSAGE_MAP(CWsLogicLabelView, CScrollView)
	//{{AFX_MSG_MAP(CWsLogicLabelView)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWsLogicLabelView drawing

void CWsLogicLabelView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	OnChangedSize();
}

void	CWsLogicLabelView::OnChangedSize( void )
{
	CClientDC	dc(this);

	SetScrollSizes(MM_TEXT, GetMaxSize(&dc));
	
	// Inform LogicView of scroll size change
}

void CWsLogicLabelView::OnDraw(CDC* pDC)
{
	int	i = 0;
	int height = GetLabelHeight(pDC);

	CWsWaveDoc* pDoc = GetDocument();

	CFont	font;
	font.CreatePointFont(LOGIC_LABEL_FONT_SIZE, LOGIC_LABEL_FONT_NAME);
	CFont*	oldFont = pDC->SelectObject(&font);

	pDC->SetTextColor(LOGIC_COLOR);
	pDC->SetBkColor(PROBE_BK_COLOR);

	POSITION pos = pDoc->m_symbolList.GetHeadPosition();
	while (pos) {
		pDC->TextOut(5, i * height, pDoc->m_symbolList.GetNext(pos)->m_name);
		i++;
	}

	pDC->SelectObject(oldFont);
}

/////////////////////////////////////////////////////////////////////////////
// CWsLogicLabelView diagnostics

#ifdef _DEBUG
void CWsLogicLabelView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CWsLogicLabelView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

#endif //_DEBUG

CWsWaveDoc* CWsLogicLabelView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWsWaveDoc)));
	return (CWsWaveDoc*)m_pDocument;
}

/////////////////////////////////////////////////////////////////////////////
// CWsLogicLabelView message handlers

int	CWsLogicLabelView::GetLabelHeight(CDC* dc)
{
	CSize	size;

	size = dc->GetTextExtent("AWHÅÄ½§");
	return size.cy;
}

CSize	CWsLogicLabelView::GetMaxSize(CDC* dc)
{
	CSize	size;
	int		maxWidth = 0;

	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CFont	font;
	font.CreatePointFont(LOGIC_LABEL_FONT_SIZE, LOGIC_LABEL_FONT_NAME);

	CFont* oldFont = dc->SelectObject(&font);

	POSITION pos = pDoc->m_symbolList.GetHeadPosition();
	while (pos) {
		size = dc->GetTextExtent(pDoc->m_symbolList.GetNext(pos)->m_name);
		if (size.cx > maxWidth) maxWidth = size.cx;
	}

	size.cx = maxWidth + 10;
	size.cy = GetLabelHeight(dc) * pDoc->m_symbolList.GetCount();

	dc->SelectObject(oldFont);


	return size;
}

void CWsLogicLabelView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	OnChangedSize();
	m_logicView->OnChangedSize();
	Invalidate();
}


BOOL CWsLogicLabelView::OnEraseBkgnd(CDC* pDC) 
{
    // Set brush to desired background color.
    CBrush backBrush(PROBE_BK_COLOR);
    // Save old brush.
    CBrush* pOldBrush = pDC->SelectObject(&backBrush);
    CRect rect;
    pDC->GetClipBox(&rect);     // Erase the area needed.
    pDC->PatBlt(rect.left, rect.top, rect.Width(), 
    rect.Height(), PATCOPY);
    pDC->SelectObject(pOldBrush);
    return TRUE;
}

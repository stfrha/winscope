// LogicLabelView2.cpp : implementation file
//

#include "stdafx.h"
#include "winscope.h"
#include "WsWaveDoc.h"
#include "LogicLabelView2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogicLabelView2

IMPLEMENT_DYNCREATE(CLogicLabelView2, CTreeView)

CLogicLabelView2::CLogicLabelView2()
{
}

CLogicLabelView2::~CLogicLabelView2()
{
}


BEGIN_MESSAGE_MAP(CLogicLabelView2, CTreeView)
	//{{AFX_MSG_MAP(CLogicLabelView2)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogicLabelView2 drawing

void CLogicLabelView2::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CLogicLabelView2 diagnostics

#ifdef _DEBUG
void CLogicLabelView2::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLogicLabelView2::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLogicLabelView2 message handlers

int	CLogicLabelView2::GetLabelHeight(CDC* dc)
{
/*	CSize	size;

	size = dc->GetTextExtent("AWHÅÄ½§");
	return size.cy;
	*/
	CTreeCtrl& tc = GetTreeCtrl();
	return tc.GetItemHeight();
}

CSize	CLogicLabelView2::GetMaxSize(CDC* dc)
{
	CTreeCtrl& tc = GetTreeCtrl();

	return tc.GetItemHeight() * tc.GetCount();

/*
	CSize	size;
	int		maxWidth = 0;

	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CFont	font;
	font.CreatePointFont(LOGIC_LABEL_FONT_SIZE, LOGIC_LABEL_FONT_NAME);

	dc->SelectObject(&font);

	POSITION pos = pDoc->m_symbolList.GetHeadPosition();
	while (pos) {
		size = dc->GetTextExtent(pDoc->m_symbolList.GetNext(pos)->m_name);
		if (size.cx > maxWidth) maxWidth = size.cx;
	}

	size.cx = maxWidth + 10;
	size.cy = GetLabelHeight(dc) * pDoc->m_symbolList.GetCount();
	return size;

  */
	return CSize(100, 100);
}


void CLogicLabelView2::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
/*
	CTreeCtrl& tc = GetTreeCtrl();

	tc.InsertItem("Hello World", 0, 0);
	*/
}

void CLogicLabelView2::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
	CTreeCtrl& tc = GetTreeCtrl();
	tc.SetBkColor(PROBE_BK_COLOR);
	tc.SetTextColor(LOGIC_LABEL_TEXT_COLOR);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	tc.InsertItem("Hello World, with a long name", 0, 0);
	
}

// Range32SplitterWnd.cpp : implementation file
//

#include "stdafx.h"
#include "winscope.h"
#include "Range32SplitterWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRange32SplitterWnd

IMPLEMENT_DYNCREATE(CRange32SplitterWnd, CMDIChildWnd)

CRange32SplitterWnd::CRange32SplitterWnd()
{
}

CRange32SplitterWnd::~CRange32SplitterWnd()
{
}

BOOL CRange32SplitterWnd::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	return m_wndSplitter.Create(this,
		2, 2,       // TODO: adjust the number of rows, columns
		CSize(10, 10),  // TODO: adjust the minimum pane size
		pContext);
}


BEGIN_MESSAGE_MAP(CRange32SplitterWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CRange32SplitterWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRange32SplitterWnd message handlers

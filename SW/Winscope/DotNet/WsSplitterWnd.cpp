// MySplitterWnd.cpp : implementation file
//

#include "stdafx.h"
#include "WsSplitterWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWsSplitterWnd

CWsSplitterWnd::CWsSplitterWnd()
{
}

CWsSplitterWnd::~CWsSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CWsSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CWsSplitterWnd)
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWsSplitterWnd message handlers





BOOL CWsSplitterWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{

	return 0;
	
//	return CSplitterWnd::OnMouseWheel(nFlags, zDelta, pt);
}

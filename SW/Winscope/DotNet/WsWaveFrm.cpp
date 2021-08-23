// WsWaveFrm.cpp : implementation of the CWsWaveFrm class
//

#include "stdafx.h"
#include "Winscope.h"

#include "WsSplitterWnd.h"
#include "WsLogicView.h"
#include "WsProbeView.h"
#include "WsLogicLabelView.h"
#include "WsProbeInfoView.h"
#include "WsWaveFrm.h"
//#include "WsSplitterWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWsWaveFrm

IMPLEMENT_DYNCREATE(CWsWaveFrm, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CWsWaveFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CWsWaveFrm)
	ON_WM_DESTROY()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWsWaveFrm construction/destruction

CWsWaveFrm::CWsWaveFrm()
{
	// TODO: add member initialization code here
	
}

CWsWaveFrm::~CWsWaveFrm()
{
}

BOOL CWsWaveFrm::OnCreateClient( LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{

	if (!m_wndSplitter.CreateStatic(this, 2, 2, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(0, 1,
		pContext->m_pNewViewClass, CSize(130, 256), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(1, 1,
		RUNTIME_CLASS(CWsLogicView), CSize(130, 200), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(0, 0,
		RUNTIME_CLASS(CWsProbeInfoView), CSize(150, 256), pContext))
	{
		TRACE0("Failed to create third pane\n");
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(1, 0,
		RUNTIME_CLASS(CWsLogicLabelView), CSize(150, 200), pContext))
	{
		TRACE0("Failed to create fourth pane\n");
		return FALSE;
	}

	((CWsProbeView*) m_wndSplitter.GetPane(0, 1))->m_probeInfoView = ((CWsProbeInfoView*) m_wndSplitter.GetPane(0, 0));
	((CWsLogicLabelView*) m_wndSplitter.GetPane(1, 0))->m_logicView = ((CWsLogicView*) m_wndSplitter.GetPane(1, 1));
	((CWsLogicView*) m_wndSplitter.GetPane(1, 1))->m_logicLabelView = ((CWsLogicLabelView*) m_wndSplitter.GetPane(1, 0));


	((CWsProbeInfoView*) m_wndSplitter.GetPane(0, 0))->m_splitterWnd = &m_wndSplitter;
//	((CWsLogicLabelView*) m_wndSplitter.GetPane(1, 0))->m_splitterWnd = &m_wndSplitter;
	((CWsProbeView*) m_wndSplitter.GetPane(0, 1))->m_splitterWnd = &m_wndSplitter;
	((CWsLogicView*) m_wndSplitter.GetPane(1, 1))->m_splitterWnd = &m_wndSplitter;

	SetActiveView((CView*)m_wndSplitter.GetPane(1,0));

	return TRUE;
}

BOOL CWsWaveFrm::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWsWaveFrm diagnostics

#ifdef _DEBUG
void CWsWaveFrm::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CWsWaveFrm::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWsWaveFrm message handlers

void CWsWaveFrm::OnDestroy() 
{
	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
}


BOOL CWsWaveFrm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
//	MessageBeep(0xFFFFFFFF);
	
	return CMDIChildWnd::OnMouseWheel(nFlags, zDelta, pt);
}

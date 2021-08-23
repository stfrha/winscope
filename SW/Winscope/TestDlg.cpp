// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscope.h"
#include "TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Thread callback function is global, I dont think it has to be tough.

UINT scopeThreadCallback( LPVOID pParam );


/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog


CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
	m_status = _T("");
	//}}AFX_DATA_INIT
}


void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	DDX_Text(pDX, IDC_STATUS, m_status);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_BN_CLICKED(IDC_LOOP_ABORT, OnLoopAbort)
	ON_BN_CLICKED(IDC_START_THREAD, OnStartThread)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_SCOPE_UPDATE, OnScopeUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

void CTestDlg::OnLoopAbort() 
{
	m_loopAbort = TRUE;	
}

void CTestDlg::OnStartThread() 
{
	m_commTest.m_id = 4711;
	m_commTest.m_loopAbort = &m_loopAbort;
	m_commTest.m_receiver = this;

	m_loopAbort = FALSE;
	m_status = "Starting thread...";
	UpdateData(FALSE);
	AfxBeginThread(scopeThreadCallback, &m_commTest);
	m_status = "Thread started";
	UpdateData(FALSE);
}

LRESULT CTestDlg::OnScopeUpdate(WPARAM wParam, LPARAM lParam)
{
	m_status = (char*) lParam;
	UpdateData(FALSE);
	return 0;
}




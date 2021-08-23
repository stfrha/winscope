// SetBuffSizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscope.h"
#include "SetBuffSizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetBuffSizeDlg dialog


CSetBuffSizeDlg::CSetBuffSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetBuffSizeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetBuffSizeDlg)
	m_bufferSize = 0;
	//}}AFX_DATA_INIT
}


void CSetBuffSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetBuffSizeDlg)
	DDX_Text(pDX, IDC_BUFFER_SIZE, m_bufferSize);
	DDV_MinMaxUInt(pDX, m_bufferSize, 0, 131071);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetBuffSizeDlg, CDialog)
	//{{AFX_MSG_MAP(CSetBuffSizeDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetBuffSizeDlg message handlers

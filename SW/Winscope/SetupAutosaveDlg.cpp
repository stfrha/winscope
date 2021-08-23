// SetupAutosaveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Winscope.h"
#include "SetupAutosaveDlg.h"
#include ".\setupautosavedlg.h"


// CSetupAutosaveDlg dialog

IMPLEMENT_DYNAMIC(CSetupAutosaveDlg, CDialog)
CSetupAutosaveDlg::CSetupAutosaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupAutosaveDlg::IDD, pParent)
	, m_autosaveActive(FALSE)
	, m_filenameBase(_T(""))
	, m_autosaveExport(FALSE)
	, m_nextIndex(0)
{
}

CSetupAutosaveDlg::~CSetupAutosaveDlg()
{
}

void CSetupAutosaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_AUTOSAVE_ACTIVE, m_autosaveActive);
	DDX_Text(pDX, IDC_FILENAME_BASE, m_filenameBase);
	DDX_Check(pDX, IDC_AUTOEXPORT_CSV, m_autosaveExport);
	DDX_Control(pDX, IDC_AUTOEXPORT_CSV, m_autoexportButton);
	DDX_Text(pDX, IDC_NEXT_INDEX, m_nextIndex);
}


BEGIN_MESSAGE_MAP(CSetupAutosaveDlg, CDialog)
	ON_BN_CLICKED(IDC_AUTOSAVE_ACTIVE, OnBnClickedAutosaveActive)
END_MESSAGE_MAP()


// CSetupAutosaveDlg message handlers

void CSetupAutosaveDlg::OnBnClickedAutosaveActive()
{
	UpdateData(TRUE);
	if (m_autosaveActive) {
		m_autoexportButton.EnableWindow(TRUE);
	} else {
		m_autoexportButton.EnableWindow(FALSE);
		m_autoexportButton.SetCheck(BST_UNCHECKED);
	}
}

BOOL CSetupAutosaveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	if (!m_autosaveActive) {
		m_autoexportButton.EnableWindow(FALSE);
		m_autoexportButton.SetCheck(BST_UNCHECKED);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

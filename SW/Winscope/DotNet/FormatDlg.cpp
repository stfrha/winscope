// FormatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscope.h"
#include "PodLabel.h"
#include "ScopeDialog.h"
#include "WsWaveDoc.h"
#include "FormatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormatDlg property page

IMPLEMENT_DYNCREATE(CFormatDlg, CPropertyPage)

CFormatDlg::CFormatDlg() : CPropertyPage(CFormatDlg::IDD)
{
	//{{AFX_DATA_INIT(CFormatDlg)
	m_labelName = _T("");
	m_podPActive = FALSE;
	m_podQActive = FALSE;
	m_prbAActive = FALSE;
	m_prbBActive = FALSE;
	m_lp0 = FALSE;
	m_lp1 = FALSE;
	m_lp2 = FALSE;
	m_lp3 = FALSE;
	m_lp4 = FALSE;
	m_lp5 = FALSE;
	m_lp6 = FALSE;
	m_lp7 = FALSE;
	m_lq0 = FALSE;
	m_lq1 = FALSE;
	m_lq2 = FALSE;
	m_lq3 = FALSE;
	m_lq4 = FALSE;
	m_lq5 = FALSE;
	m_lq6 = FALSE;
	m_lq7 = FALSE;
	m_viewRadix = -1;
	//}}AFX_DATA_INIT
	m_initialized = FALSE;
}

CFormatDlg::~CFormatDlg()
{
}

void CFormatDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormatDlg)
	DDX_Control(pDX, IDC_REMOVE_LABEL, m_removeLabelButt);
	DDX_Control(pDX, IDC_MODIFY_LABEL, m_modifyLabelButt);
	DDX_Control(pDX, IDC_ADD_LABEL, m_addLabelButt);
	DDX_Control(pDX, IDC_LABEL_LIST, m_labelList);
	DDX_Text(pDX, IDC_LABEL_NAME, m_labelName);
	DDX_Check(pDX, IDC_POD_P_ACTIVE, m_podPActive);
	DDX_Check(pDX, IDC_POD_Q_ACTIVE, m_podQActive);
	DDX_Check(pDX, IDC_PRB_A_ACTIVE, m_prbAActive);
	DDX_Check(pDX, IDC_PRB_B_ACTIVE, m_prbBActive);
	DDX_Check(pDX, IDC_SYMBIT_P0, m_lp0);
	DDX_Check(pDX, IDC_SYMBIT_P1, m_lp1);
	DDX_Check(pDX, IDC_SYMBIT_P2, m_lp2);
	DDX_Check(pDX, IDC_SYMBIT_P3, m_lp3);
	DDX_Check(pDX, IDC_SYMBIT_P4, m_lp4);
	DDX_Check(pDX, IDC_SYMBIT_P5, m_lp5);
	DDX_Check(pDX, IDC_SYMBIT_P6, m_lp6);
	DDX_Check(pDX, IDC_SYMBIT_P7, m_lp7);
	DDX_Check(pDX, IDC_SYMBIT_Q0, m_lq0);
	DDX_Check(pDX, IDC_SYMBIT_Q1, m_lq1);
	DDX_Check(pDX, IDC_SYMBIT_Q2, m_lq2);
	DDX_Check(pDX, IDC_SYMBIT_Q3, m_lq3);
	DDX_Check(pDX, IDC_SYMBIT_Q4, m_lq4);
	DDX_Check(pDX, IDC_SYMBIT_Q5, m_lq5);
	DDX_Check(pDX, IDC_SYMBIT_Q6, m_lq6);
	DDX_Check(pDX, IDC_SYMBIT_Q7, m_lq7);
	DDX_CBIndex(pDX, IDC_VIEW_RADIX, m_viewRadix);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormatDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CFormatDlg)
	ON_NOTIFY(NM_CLICK, IDC_LABEL_LIST, OnClickLabelList)
	ON_BN_CLICKED(IDC_ADD_LABEL, OnAddLabel)
	ON_BN_CLICKED(IDC_MODIFY_LABEL, OnModifyLabel)
	ON_BN_CLICKED(IDC_REMOVE_LABEL, OnRemoveLabel)
	ON_EN_CHANGE(IDC_LABEL_NAME, OnChangeLabelName)
	ON_BN_CLICKED(IDC_PRB_A_ACTIVE, OnPrbAActive)
	ON_BN_CLICKED(IDC_PRB_B_ACTIVE, OnPrbBActive)
	ON_BN_CLICKED(IDC_POD_P_ACTIVE, OnPodPActive)
	ON_BN_CLICKED(IDC_POD_Q_ACTIVE, OnPodQActive)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormatDlg message handlers

BOOL CFormatDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	CRect rectWnd;
	GetWindowRect(rectWnd);
	SetWindowPos(NULL, 0, 0,
		rectWnd.Width() - 100,
		rectWnd.Height(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);


	m_font.CreatePointFont(80, "Courier New");
	m_labelList.SetFont(&m_font, TRUE);

	m_labelList.InsertColumn(0, "Label", LVCFMT_LEFT, 60, 0);
	m_labelList.InsertColumn(1, "Pod P", LVCFMT_LEFT, 60, 1);
	m_labelList.InsertColumn(2, "Pod Q", LVCFMT_LEFT, 60, 2);

	m_podPActive = m_scopeDialog->m_doc->m_docSetting.m_prbPodCntrl.bits.PODP_ACTIVE;
	m_podQActive = m_scopeDialog->m_doc->m_docSetting.m_prbPodCntrl.bits.PODQ_ACTIVE;
	m_prbAActive = m_scopeDialog->m_doc->m_docSetting.m_prbPodCntrl.bits.PRBA_ACTIVE;
	m_prbBActive = m_scopeDialog->m_doc->m_docSetting.m_prbPodCntrl.bits.PRBB_ACTIVE;

	m_labelName = "";

	UpdateData(FALSE);

	m_initialized = TRUE;

	m_addLabelButt.EnableWindow(FALSE);
	m_modifyLabelButt.EnableWindow(FALSE);
	m_removeLabelButt.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFormatDlg::OnChangeLabelName() 
{
	UpdateData(TRUE);
	if (m_labelName == "") {
		m_addLabelButt.EnableWindow(FALSE);
	} else {
		m_addLabelButt.EnableWindow(TRUE);
	}
}

void CFormatDlg::OnClickLabelList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString		msg;
	CPodLabel*	tmpLabel;

	POSITION pos = m_labelList.GetFirstSelectedItemPosition();
	if (pos) {
//		msg.Format("Selected item: %d", m_labelList.GetNextSelectedItem(pos));
//		MessageBox(msg, "Hello");

		
		tmpLabel = m_scopeDialog->m_doc->m_docSetting.GetSymbol(m_labelList.GetItemText(m_labelList.GetNextSelectedItem(pos), 0));

		if (!tmpLabel) {
			MessageBox("ERROR: Selected symbol does not exist in data list", "Error", MB_OK | MB_ICONEXCLAMATION);
			return;
		}
		
		m_labelName = tmpLabel->m_name;
		m_lp0 = FALSE;
		m_lp1 = FALSE;
		m_lp2 = FALSE;
		m_lp3 = FALSE;
		m_lp4 = FALSE;
		m_lp5 = FALSE;
		m_lp6 = FALSE;
		m_lp7 = FALSE;
		m_lq0 = FALSE;
		m_lq1 = FALSE;
		m_lq2 = FALSE;
		m_lq3 = FALSE;
		m_lq4 = FALSE;
		m_lq5 = FALSE;
		m_lq6 = FALSE;
		m_lq7 = FALSE;
		if (tmpLabel->m_validPtrnP & 0x01) m_lp0 = TRUE;
		if (tmpLabel->m_validPtrnP & 0x02) m_lp1 = TRUE;
		if (tmpLabel->m_validPtrnP & 0x04) m_lp2 = TRUE;
		if (tmpLabel->m_validPtrnP & 0x08) m_lp3 = TRUE;
		if (tmpLabel->m_validPtrnP & 0x10) m_lp4 = TRUE;
		if (tmpLabel->m_validPtrnP & 0x20) m_lp5 = TRUE;
		if (tmpLabel->m_validPtrnP & 0x40) m_lp6 = TRUE;
		if (tmpLabel->m_validPtrnP & 0x80) m_lp7 = TRUE;
		if (tmpLabel->m_validPtrnQ & 0x01) m_lq0 = TRUE;
		if (tmpLabel->m_validPtrnQ & 0x02) m_lq1 = TRUE;
		if (tmpLabel->m_validPtrnQ & 0x04) m_lq2 = TRUE;
		if (tmpLabel->m_validPtrnQ & 0x08) m_lq3 = TRUE;
		if (tmpLabel->m_validPtrnQ & 0x10) m_lq4 = TRUE;
		if (tmpLabel->m_validPtrnQ & 0x20) m_lq5 = TRUE;
		if (tmpLabel->m_validPtrnQ & 0x40) m_lq6 = TRUE;
		if (tmpLabel->m_validPtrnQ & 0x80) m_lq7 = TRUE;
		m_viewRadix = tmpLabel->m_viewRadix;
		UpdateData(FALSE);
		m_modifyLabelButt.EnableWindow(TRUE);
		m_removeLabelButt.EnableWindow(TRUE);
	} else {
		m_modifyLabelButt.EnableWindow(FALSE);
		m_removeLabelButt.EnableWindow(FALSE);
	}
	
	*pResult = 0;
}

/*
POSITION	CFormatDlg::SearchListForName(CString searchName)
{
	POSITION oldPos;
	POSITION pos = m_labelDataList.GetHeadPosition();
	while (pos) {
		oldPos = pos;
		if (searchName == m_labelDataList.GetNext(pos)->m_name) return oldPos;
	}
	return NULL;
}
*/
void CFormatDlg::OnAddLabel() 
{
	unsigned char	p = 0, q = 0;
	CString			ps = "--------";
	CString			qs = "--------";
	CPodLabel*		m_newLabel;

	UpdateData(TRUE);
	if (m_labelName == "") {
		MessageBox("Write a label name", "Error", MB_ICONEXCLAMATION);
		return;
	}
	if (m_scopeDialog->m_doc->m_docSetting.GetSymbolPos(m_labelName)) {
		MessageBox("Label already defined", "Error", MB_ICONEXCLAMATION);
		return;
	}

	if (m_lp0) { p += 0x01; ps.SetAt(7, 'X'); }
	if (m_lp1) { p += 0x02; ps.SetAt(6, 'X'); }
	if (m_lp2) { p += 0x04; ps.SetAt(5, 'X'); }
	if (m_lp3) { p += 0x08; ps.SetAt(4, 'X'); }
	if (m_lp4) { p += 0x10; ps.SetAt(3, 'X'); }
	if (m_lp5) { p += 0x20; ps.SetAt(2, 'X'); }
	if (m_lp6) { p += 0x40; ps.SetAt(1, 'X'); }
	if (m_lp7) { p += 0x80; ps.SetAt(0, 'X'); }

	if (m_lq0) { q += 0x01; qs.SetAt(7, 'X'); }
	if (m_lq1) { q += 0x02; qs.SetAt(6, 'X'); }
	if (m_lq2) { q += 0x04; qs.SetAt(5, 'X'); }
	if (m_lq3) { q += 0x08; qs.SetAt(4, 'X'); }
	if (m_lq4) { q += 0x10; qs.SetAt(3, 'X'); }
	if (m_lq5) { q += 0x20; qs.SetAt(2, 'X'); }
	if (m_lq6) { q += 0x40; qs.SetAt(1, 'X'); }
	if (m_lq7) { q += 0x80; qs.SetAt(0, 'X'); }

	m_labelList.InsertItem(m_scopeDialog->m_doc->m_docSetting.m_symbolList->GetCount(), m_labelName);
	m_labelList.SetItemText(m_scopeDialog->m_doc->m_docSetting.m_symbolList->GetCount(), 1, ps);
	m_labelList.SetItemText(m_scopeDialog->m_doc->m_docSetting.m_symbolList->GetCount(), 2, qs);
	m_newLabel = (CPodLabel*) new  CPodLabel(m_labelName, p, q);
	m_newLabel->m_viewRadix = m_viewRadix;
	m_scopeDialog->m_doc->m_docSetting.m_symbolList->AddTail(m_newLabel);
	UpdateData(FALSE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CFormatDlg::OnModifyLabel() 
{
	POSITION	pos;
	CPodLabel*	tmpLabel;
	int			item;
	unsigned char	p = 0, q = 0;
	CString			ps = "--------";
	CString			qs = "--------";

	UpdateData(TRUE);
	if (m_labelName == "") {
		MessageBox("Write a label name", "Error", MB_ICONEXCLAMATION);
		return;
	}

	tmpLabel = m_scopeDialog->m_doc->m_docSetting.GetSymbol(m_labelName);
	if (!tmpLabel) {
		MessageBox("Label does not exist", "Error", MB_ICONEXCLAMATION);
		return;
	}


	if (m_lp0) { p += 0x01; ps.SetAt(7, 'X'); }
	if (m_lp1) { p += 0x02; ps.SetAt(6, 'X'); }
	if (m_lp2) { p += 0x04; ps.SetAt(5, 'X'); }
	if (m_lp3) { p += 0x08; ps.SetAt(4, 'X'); }
	if (m_lp4) { p += 0x10; ps.SetAt(3, 'X'); }
	if (m_lp5) { p += 0x20; ps.SetAt(2, 'X'); }
	if (m_lp6) { p += 0x40; ps.SetAt(1, 'X'); }
	if (m_lp7) { p += 0x80; ps.SetAt(0, 'X'); }

	if (m_lq0) { q += 0x01; qs.SetAt(7, 'X'); }
	if (m_lq1) { q += 0x02; qs.SetAt(6, 'X'); }
	if (m_lq2) { q += 0x04; qs.SetAt(5, 'X'); }
	if (m_lq3) { q += 0x08; qs.SetAt(4, 'X'); }
	if (m_lq4) { q += 0x10; qs.SetAt(3, 'X'); }
	if (m_lq5) { q += 0x20; qs.SetAt(2, 'X'); }
	if (m_lq6) { q += 0x40; qs.SetAt(1, 'X'); }
	if (m_lq7) { q += 0x80; qs.SetAt(0, 'X'); }

	tmpLabel->m_validPtrnP = p;
	tmpLabel->m_validPtrnQ = q;

	tmpLabel->m_viewRadix = m_viewRadix;
	pos = m_labelList.GetFirstSelectedItemPosition();
	if (pos) {
		item = m_labelList.GetNextSelectedItem(pos);
		m_labelList.SetItemText(item, 1, ps);
		m_labelList.SetItemText(item, 2, qs);
	}
	m_scopeDialog->OnSettingChanged(' ');
}

void CFormatDlg::OnRemoveLabel() 
{
	POSITION	pos;
	CPodLabel*	tmpLabel;

	UpdateData(TRUE);
	if (m_labelName == "") {
		MessageBox("Write a label name", "Error", MB_ICONEXCLAMATION);
		return;
	}

	pos = m_scopeDialog->m_doc->m_docSetting.GetSymbolPos(m_labelName);
	tmpLabel = m_scopeDialog->m_doc->m_docSetting.GetSymbol(m_labelName);
	if (!tmpLabel) {
		MessageBox("Label does not exist", "Error", MB_ICONEXCLAMATION);
		return;
	}

	if (m_scopeDialog->IsSymbolSelectedQ(tmpLabel)) {
		if (MessageBox("The symbol about to be removed is used as a trigger symbol\nDo you still want to erase it?", "Resolve posible conflict", MB_YESNO | MB_ICONQUESTION) == IDNO) return;
		else m_scopeDialog->KillingSymbol(tmpLabel);
	}

	m_scopeDialog->m_doc->m_docSetting.m_symbolList->RemoveAt(pos);

	pos = m_labelList.GetFirstSelectedItemPosition();
	if (pos) {
		m_labelList.DeleteItem(m_labelList.GetNextSelectedItem(pos));
	}
	if (!m_labelList.GetFirstSelectedItemPosition()) {
		m_modifyLabelButt.EnableWindow(FALSE);
		m_removeLabelButt.EnableWindow(FALSE);
	}
	m_scopeDialog->OnSettingChanged(' ');
}


void CFormatDlg::OnPrbAActive() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CFormatDlg::OnPrbBActive() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CFormatDlg::OnPodPActive() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CFormatDlg::OnPodQActive() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

BOOL CFormatDlg::OnSetActive() 
{
	CPodLabel*	tmpSymbol;
	CString		ps;
	CString		qs;
	int			i = 0, j;

	if (m_scopeDialog->m_doc->m_docSetting.m_symbolList) {

		m_labelList.DeleteAllItems();

		POSITION pos = m_scopeDialog->m_doc->m_docSetting.m_symbolList->GetHeadPosition();
		while (pos) {
			ps = "--------";
			qs = "--------";
			tmpSymbol = m_scopeDialog->m_doc->m_docSetting.m_symbolList->GetNext(pos);
			for (j=0 ; j<8 ; j++) {
				if (tmpSymbol->m_validPtrnP & 1<<j) ps.SetAt(j, 'X');
				if (tmpSymbol->m_validPtrnQ & 1<<j) qs.SetAt(j, 'X');
			}
			m_labelList.InsertItem(i, tmpSymbol->m_name);
			m_labelList.SetItemText(i, 1, ps);
			m_labelList.SetItemText(i, 2, qs);
			i++;
		}
	}

	UpdateSettingsView();

	return CPropertyPage::OnSetActive();
}

void	CFormatDlg::UpdateSettingsView( void )
{
	m_podPActive = m_scopeDialog->m_doc->m_docSetting.m_prbPodCntrl.bits.PODP_ACTIVE;
	m_podQActive = m_scopeDialog->m_doc->m_docSetting.m_prbPodCntrl.bits.PODQ_ACTIVE;
	m_prbAActive = m_scopeDialog->m_doc->m_docSetting.m_prbPodCntrl.bits.PRBA_ACTIVE;
	m_prbBActive = m_scopeDialog->m_doc->m_docSetting.m_prbPodCntrl.bits.PRBB_ACTIVE;
}

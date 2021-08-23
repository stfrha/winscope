// TriggerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscope.h"
#include "ScopeDialog.h"
#include "WsWaveDoc.h"
#include "TriggerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTriggerDlg property page

IMPLEMENT_DYNCREATE(CTriggerDlg, CPropertyPage)

CTriggerDlg::CTriggerDlg() : CPropertyPage(CTriggerDlg::IDD)
{
	//{{AFX_DATA_INIT(CTriggerDlg)
	m_triggerFunction = -1;
	m_prbTriggSource = -1;
	m_prbTriggSlope = -1;
	m_ptrnRadix = -1;
	m_symbolEdge = -1;
	m_triggPtrn = _T("");
	m_extTriggSlope = -1;
	m_triggLevelB = 0;
	m_triggLevelA = 0;
	//}}AFX_DATA_INIT
	m_ptrnSymbolName = "";
	m_edgeSymbolName = "";
	m_initialized = FALSE;
	m_reqTrgLvlVA = 0.0;
	m_reqTrgLvlVB = 0.0;
}

CTriggerDlg::~CTriggerDlg()
{
}

void CTriggerDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTriggerDlg)
	DDX_Control(pDX, IDC_PRB_TRG_LVL_B_VOLT, m_triggLevelBVolt);
	DDX_Control(pDX, IDC_PRB_TRG_LVL_A_VOLT, m_triggLevelAVolt);
	DDX_Control(pDX, IDC_TRIGG_PTRN, m_triggPtrnEdit);
	DDX_Control(pDX, IDC_EDGE_SYMBOL, m_edgeSymbolComboBox);
	DDX_Control(pDX, IDC_PTRN_SYMBOL, m_ptrnSymbolComboBox);
	DDX_Control(pDX, IDC_LVL_B_SCROLLBAR, m_scrollbarB);
	DDX_Control(pDX, IDC_LVL_A_SCROLLBAR, m_scrollbarA);
	DDX_CBIndex(pDX, IDC_TRIGG_FUNCTION, m_triggerFunction);
	DDX_Radio(pDX, IDC_PRB_TRIGG_SRC, m_prbTriggSource);
	DDX_Radio(pDX, IDC_PRB_TRIGG_SLOPE, m_prbTriggSlope);
	DDX_CBIndex(pDX, IDC_PTRN_RADIX, m_ptrnRadix);
	DDX_Radio(pDX, IDC_SYMBOL_EDGE, m_symbolEdge);
	DDX_Text(pDX, IDC_TRIGG_PTRN, m_triggPtrn);
	DDX_Radio(pDX, IDC_EXT_PRB_TRIGG_SLOPE, m_extTriggSlope);
	DDX_Text(pDX, IDC_PRB_TRG_LVL_B, m_triggLevelB);
	DDV_MinMaxInt(pDX, m_triggLevelB, 0, 255);
	DDX_Text(pDX, IDC_PRB_TRG_LVL_A, m_triggLevelA);
	DDV_MinMaxInt(pDX, m_triggLevelA, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTriggerDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CTriggerDlg)
	ON_CBN_SELCHANGE(IDC_PTRN_SYMBOL, OnSelchangePtrnSymbol)
	ON_EN_CHANGE(IDC_TRIGG_PTRN, OnChangeTriggPtrn)
	ON_EN_KILLFOCUS(IDC_TRIGG_PTRN, OnKillfocusTriggPtrn)
	ON_CBN_SELCHANGE(IDC_PTRN_RADIX, OnSelchangePtrnRadix)
	ON_CBN_SELCHANGE(IDC_EDGE_SYMBOL, OnSelchangeEdgeSymbol)
	ON_EN_CHANGE(IDC_PRB_TRG_LVL_A, OnChangePrbTrgLvlA)
	ON_EN_CHANGE(IDC_PRB_TRG_LVL_B, OnChangePrbTrgLvlB)
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_TRIGG_FUNCTION, OnSelchangeTriggFunction)
	ON_EN_KILLFOCUS(IDC_PRB_TRG_LVL_A, OnKillfocusPrbTrgLvlA)
	ON_EN_KILLFOCUS(IDC_PRB_TRG_LVL_B, OnKillfocusPrbTrgLvlB)
	ON_BN_CLICKED(IDC_PRB_TRIGG_SRC, OnPrbTriggSrc)
	ON_BN_CLICKED(IDC_PRB_TRIGG_SRC_B, OnPrbTriggSrcB)
	ON_BN_CLICKED(IDC_PRB_TRIGG_SLOPE, OnPrbTriggSlope)
	ON_BN_CLICKED(IDC_PRB_TRIGG_SLOPE_NEG, OnPrbTriggSlopeNeg)
	ON_BN_CLICKED(IDC_EXT_PRB_TRIGG_SLOPE, OnExtPrbTriggSlope)
	ON_BN_CLICKED(IDC_EXT_PRB_TRIGG_SLOPE_NEG, OnExtPrbTriggSlopeNeg)
	ON_BN_CLICKED(IDC_SYMBOL_EDGE, OnSymbolEdge)
	ON_BN_CLICKED(IDC_SYMBOL_EDGE_NEG, OnSymbolEdgeNeg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTriggerDlg message handlers

BOOL CTriggerDlg::OnSetActive() 
{
	BOOL	ret;

	m_ptrnSymbolComboBox.ResetContent();
	m_edgeSymbolComboBox.ResetContent();
	m_scopeDialog->RegisterAllSymbols();

	ret = CPropertyPage::OnSetActive();

	if (m_scopeDialog->m_doc->m_ptrnSelSymbol) {
		if (m_ptrnSymbolComboBox.SelectString(-1, m_scopeDialog->m_doc->m_ptrnSelSymbol->m_name) == CB_ERR) {
			MessageBox("Did not find pattern symbol", "ERROR");
		} else {
			m_ptrnRadix = m_scopeDialog->m_doc->m_ptrnSelSymbol->m_triggerRadix;
			CreateTriggPatternString();
			UpdateData(FALSE);
		}
	}
	if (m_scopeDialog->m_doc->m_edgeSelSymbol) {
		if (m_edgeSymbolComboBox.SelectString(-1, m_scopeDialog->m_doc->m_edgeSelSymbol->m_name) == CB_ERR) {
			MessageBox("Did not find edge symbol", "ERROR");
		}
	}

/*
	int oldTLA = m_triggLevelA;

	m_triggLevelA = (int) (128.0 * m_reqTrgLvlVA / m_scopeDialog->m_doc->m_docSetting.GetScreenVoltageRange('A') + 128.0);
	if (m_triggLevelA > 255) m_triggLevelA = 255;
	if (m_triggLevelA < 1) m_triggLevelA = 1;

	int oldTLB = m_triggLevelB;

	m_triggLevelB = (int) (128.0 * m_reqTrgLvlVB / m_scopeDialog->m_doc->m_docSetting.GetScreenVoltageRange('B') + 128.0);
	if (m_triggLevelB > 255) m_triggLevelB = 255;
	if (m_triggLevelB < 1) m_triggLevelB = 1;

	if ((oldTLA != m_triggLevelA) || (oldTLB != m_triggLevelB)) {
		m_scopeDialog->OnSettingChanged(' ');
	}

	
	m_scrollbarA.SetScrollPos(255 - m_triggLevelA);
	m_scrollbarB.SetScrollPos(255 - m_triggLevelB);

	UpdateVoltTriggLevel();

	UpdateData(FALSE);

*/
	return ret;
}

void CTriggerDlg::OnSelchangePtrnSymbol() 
{
	int		selIndex;
	CString	selName;

	selIndex = m_ptrnSymbolComboBox.GetCurSel();
	if (selIndex == CB_ERR) return;
	m_ptrnSymbolComboBox.GetLBText(selIndex, selName);
	m_scopeDialog->m_doc->m_ptrnSelSymbol = m_scopeDialog->m_doc->m_docSetting.GetSymbol(selName);
	m_ptrnRadix = m_scopeDialog->m_doc->m_ptrnSelSymbol->m_triggerRadix;
	CreateTriggPatternString();
	UpdateData(FALSE);
	m_scopeDialog->OnSettingChanged(' ');

	//Ändra valideringsgränser till m_triggPtrn
}

void CTriggerDlg::OnChangeTriggPtrn() 
{
	//Kolla att senaste tecknet är inom rätt grupp
}

void CTriggerDlg::OnKillfocusTriggPtrn() 
{
	//Kolla att strängen innehåller rätt antal bitar
	CString oldStr = m_triggPtrn;
	UpdateData(TRUE);
	if (oldStr != m_triggPtrn) {
		if (m_triggPtrn == "") {
			m_scopeDialog->OnSettingChanged(' ');
			return;
		}
		if (CreateTriggPatternValues()) {
			if (m_scopeDialog->m_doc->m_ptrnSelSymbol) {
				MessageBox("Trigger pattern value too high\n(Can not be greater then weight of symbol)", "Error", MB_OK | MB_ICONEXCLAMATION);
				m_triggPtrnEdit.SetFocus();
				m_triggPtrnEdit.SetSel(0, -1);
			}
		}
		m_scopeDialog->OnSettingChanged(' ');
	}
}


void CTriggerDlg::OnSelchangePtrnRadix() 
{
	//Ändra patternsträng till ny radix
	UpdateData(TRUE);
	m_scopeDialog->m_doc->m_ptrnSelSymbol->m_triggerRadix = m_ptrnRadix;
	CreateTriggPatternString();
	m_scopeDialog->OnSettingChanged(' ');
}

/*CString CTriggerDlg::CreateBinString(int p, int q) 
{
}

void	CTriggerDlg::ConvertFromBinString(CString str, int* p, int* q) 
{

}
*/

void CTriggerDlg::OnSelchangeEdgeSymbol() 
{
	int		selIndex;
	CString	selName;

	selIndex = m_edgeSymbolComboBox.GetCurSel();
	if (selIndex == CB_ERR) {
		m_scopeDialog->OnSettingChanged(' ');
		return;
	}
	m_edgeSymbolComboBox.GetLBText(selIndex, selName);
	m_scopeDialog->m_doc->m_edgeSelSymbol = m_scopeDialog->m_doc->m_docSetting.GetSymbol(selName);
	m_scopeDialog->OnSettingChanged(' ');
}

void CTriggerDlg::CreateTriggPatternString( void )
{
	int		rep = 0;
	int		i;
	
	for (i=7 ; i>=0 ; i--) {
		if ((m_scopeDialog->m_doc->m_ptrnSelSymbol->m_validPtrnP >> i) & 0x01) {
			rep = rep << 1;
			rep += (m_scopeDialog->m_doc->m_ptrnSelSymbol->m_triggPtrnP >> i) & 0x01;
		}
	}

	for (i=7 ; i>=0 ; i--) {
		if ((m_scopeDialog->m_doc->m_ptrnSelSymbol->m_validPtrnQ >> i) & 0x01) {
			rep = rep << 1;
			rep += (m_scopeDialog->m_doc->m_ptrnSelSymbol->m_triggPtrnQ >> i) & 0x01;
		}
	}

	switch (m_scopeDialog->m_doc->m_ptrnSelSymbol->m_triggerRadix) {
	case 0:						//Hex
		m_triggPtrn.Format("%X", rep);
		break;
	case 1:
		m_triggPtrn = BinSprint(rep);
		break;
	case 2:
		m_triggPtrn.Format("%d", rep);
		break;
	}

	UpdateData(FALSE);
}

CString	CTriggerDlg::BinSprint(int v)
{
	int		i;
	int		j = 0;
	CString	str;

	str = "";

	for (i=32768 ; (i>1 && !(v & i)) ; i /= 2);
	for ( ; i>=1  ; i /= 2) {
		if (v & i) str += "1";
		else str += "0";
	}

	return str;
}

void	CTriggerDlg::BinSscanf(const CString str, int* v)
{
	int	i;

	*v = 0;

	for (i = 0 ; i < str.GetLength() ; i++) {
		*v = *v << 1;
		if (str[i] == '1') (*v)++;
	}
}


BOOL CTriggerDlg::CreateTriggPatternValues( void )
{
	int		rep = 0;
	int		i;
	int		weight;

	UpdateData(TRUE);
	
	switch (m_ptrnRadix) {
	case 0:						//Hex
		sscanf(m_triggPtrn, "%X", &rep);
		break;
	case 1:
		BinSscanf(m_triggPtrn, &rep);
		break;
	case 2:
		sscanf(m_triggPtrn, "%d", &rep);
		break;
	}

	if (rep > (1 << m_scopeDialog->m_doc->m_ptrnSelSymbol->GetWeight()) - 1) return TRUE;

	m_scopeDialog->m_doc->m_ptrnSelSymbol->m_triggPtrnP = 0;
	m_scopeDialog->m_doc->m_ptrnSelSymbol->m_triggPtrnQ = 0;
	weight = m_scopeDialog->m_doc->m_ptrnSelSymbol->GetWeight();

	for (i=7 ; i>=0 ; i--) {
		if ((m_scopeDialog->m_doc->m_ptrnSelSymbol->m_validPtrnP >> i) & 0x01) {
			m_scopeDialog->m_doc->m_ptrnSelSymbol->m_triggPtrnP += ((rep >> (weight-1)) & 0x01) << i;
			weight--;
		}
	}

	for (i=7 ; i>=0 ; i--) {
		if ((m_scopeDialog->m_doc->m_ptrnSelSymbol->m_validPtrnQ >> i) & 0x01) {
			m_scopeDialog->m_doc->m_ptrnSelSymbol->m_triggPtrnQ += ((rep >> (weight-1)) & 0x01) << i;
			weight--;
		}
	}

	UpdateData(FALSE);
	return FALSE;
}

void CTriggerDlg::UpdateTriggLevel(char probe, int newTriggLevel) 
{
	int		min, max;
	m_scrollbarA.GetScrollRange(&min, &max);

	switch (probe) {
	case 'A':
	case 'a':
		m_triggLevelA = newTriggLevel;
		m_scrollbarA.SetScrollPos(max - m_triggLevelA);
		m_reqTrgLvlVA = (((double) m_triggLevelA) - 128.0) / 128.0 * m_scopeDialog->m_doc->m_docSetting.GetScreenVoltageRange('A');
		break;
	case 'B':
	case 'b':
		m_triggLevelB = newTriggLevel;
		m_scrollbarB.SetScrollPos(max - m_triggLevelB);
		m_reqTrgLvlVA = (((double) m_triggLevelA) - 128.0) / 128.0 * m_scopeDialog->m_doc->m_docSetting.GetScreenVoltageRange('A');
		break;
	}

	m_scopeDialog->OnSettingChanged(' ');
	UpdateVoltTriggLevel();
}


void CTriggerDlg::OnChangePrbTrgLvlA() 
{
	CString	text;
	int		min, max;

	m_scrollbarA.GetScrollRange(&min, &max);

	UpdateData(TRUE);

/*
	text.Format("%.1f", (((double) m_triggLevelA) - 128.0) / 128.0 * 2.0);

	m_triggLevelAVolt.SetWindowText(text);
*/
	m_scrollbarA.SetScrollPos(max - m_triggLevelA);
	m_scopeDialog->OnSettingChanged(' ');

	m_reqTrgLvlVA = (((double) m_triggLevelA) - 128.0) / 128.0 * m_scopeDialog->m_doc->m_docSetting.GetScreenVoltageRange('A');
	
	UpdateVoltTriggLevel();
}

void CTriggerDlg::OnChangePrbTrgLvlB() 
{
	CString	text;
	int		min, max;

	m_scrollbarB.GetScrollRange(&min, &max);

	UpdateData(TRUE);
/*
	text.Format("%.1f", (((double) m_triggLevelB) - 128.0) / 128.0 * 2.0);

	m_triggLevelBVolt.SetWindowText(text);
*/
	m_scrollbarB.SetScrollPos(max - m_triggLevelB);
	m_scopeDialog->OnSettingChanged(' ');

	m_reqTrgLvlVB = (((double) m_triggLevelB) - 128.0) / 128.0 * m_scopeDialog->m_doc->m_docSetting.GetScreenVoltageRange('B');
	
	UpdateVoltTriggLevel();
}

void CTriggerDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString	text;
	int		min, max;


	if (pScrollBar == &m_scrollbarA) {
		m_scrollbarA.GetScrollRange(&min, &max);
		switch (nSBCode) {
		case SB_BOTTOM :
			m_triggLevelA = 0;
			break;
		case SB_LINEDOWN :
			m_triggLevelA--;
			break;
		case SB_PAGEDOWN :
			m_triggLevelA -= 10;
			break;
		case SB_LINEUP :
			m_triggLevelA++;
			break;
		case SB_PAGEUP :
			m_triggLevelA += 10;
			break;
		case SB_TOP :
			m_triggLevelA = 255;
			break;
		case SB_THUMBPOSITION :
			m_triggLevelA = max - nPos;
			break;
//		case SB_THUMBTRACK :
//			m_triggLevelA = max - nPos;
//			break;
		default:
			return;
		}
		m_scrollbarA.SetScrollPos(max - m_triggLevelA);

		UpdateData(FALSE);

		m_reqTrgLvlVA = (((double) m_triggLevelA) - 128.0) / 128.0 * m_scopeDialog->m_doc->m_docSetting.GetScreenVoltageRange('A');
	}
	
	if (pScrollBar == &m_scrollbarB) {
		m_scrollbarB.GetScrollRange(&min, &max);
		switch (nSBCode) {
		case SB_BOTTOM :
			m_triggLevelB = 0;
			break;
		case SB_LINEDOWN :
			m_triggLevelB--;
			break;
		case SB_PAGEDOWN :
			m_triggLevelB -= 10;
			break;
		case SB_LINEUP :
			m_triggLevelB++;
			break;
		case SB_PAGEUP :
			m_triggLevelB += 10;
			break;
		case SB_TOP :
			m_triggLevelB = 255;
			break;
		case SB_THUMBPOSITION :
			m_triggLevelB = max - nPos;
			break;
//		case SB_THUMBTRACK :
//			m_triggLevelB = max - nPos;
//			break;
		default:
			return;
		}
		m_scrollbarB.SetScrollPos(max - m_triggLevelB);

		UpdateData(FALSE);

		m_reqTrgLvlVB = (((double) m_triggLevelB) - 128.0) / 128.0 * m_scopeDialog->m_doc->m_docSetting.GetScreenVoltageRange('B');
	}
	m_scopeDialog->OnSettingChanged(' ');
	UpdateVoltTriggLevel();
//	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CTriggerDlg::OnKillActive() 
{
	if (m_ptrnSymbolComboBox.GetCurSel() >= 0) {
		m_ptrnSymbolComboBox.GetLBText(m_ptrnSymbolComboBox.GetCurSel(), m_ptrnSymbolName);
	} else {
		m_ptrnSymbolName = "";
	}

	if (m_edgeSymbolComboBox.GetCurSel() >= 0) {
		m_edgeSymbolComboBox.GetLBText(m_edgeSymbolComboBox.GetCurSel(), m_edgeSymbolName);
	} else {
		m_edgeSymbolName = "";
	}

	return CPropertyPage::OnKillActive();
}

void CTriggerDlg::OnSelchangeTriggFunction() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CTriggerDlg::OnKillfocusPrbTrgLvlA() 
{
	int	oldLevel = m_triggLevelA;
	UpdateData(TRUE);
	if (m_triggLevelA != oldLevel) {
		m_scopeDialog->OnSettingChanged(' ');
	}
}

void CTriggerDlg::OnKillfocusPrbTrgLvlB() 
{
	int	oldLevel = m_triggLevelB;
	UpdateData(TRUE);
	if (m_triggLevelA != oldLevel) {
		m_scopeDialog->OnSettingChanged(' ');
	}
}

void CTriggerDlg::OnPrbTriggSrc() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CTriggerDlg::OnPrbTriggSrcB() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CTriggerDlg::OnPrbTriggSlope() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CTriggerDlg::OnPrbTriggSlopeNeg() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CTriggerDlg::OnExtPrbTriggSlope() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CTriggerDlg::OnExtPrbTriggSlopeNeg() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CTriggerDlg::OnSymbolEdge() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CTriggerDlg::OnSymbolEdgeNeg() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

BOOL CTriggerDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CRect rectWnd;
	GetWindowRect(rectWnd);
	SetWindowPos(NULL, 0, 0,
		rectWnd.Width() - 100,
		rectWnd.Height(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	m_scrollbarA.SetScrollRange(0, 255);
	m_scrollbarB.SetScrollRange(0, 255);

	UpdateSettingsView();

	m_initialized = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void	CTriggerDlg::UpdateVoltTriggLevel( void )
{
	CString	text;

	text.Format("%.2f", (((double) m_triggLevelA) - 128.0) / 128.0 * m_scopeDialog->m_doc->m_docSetting.GetScreenVoltageRange('A'));
	m_triggLevelAVolt.SetWindowText(text);

	text.Format("%.2f", (((double) m_triggLevelB) - 128.0) / 128.0 * m_scopeDialog->m_doc->m_docSetting.GetScreenVoltageRange('B'));
	m_triggLevelBVolt.SetWindowText(text);
}


void	CTriggerDlg::UpdateSettingsView( void )
{
	m_extTriggSlope = m_scopeDialog->m_doc->m_docSetting.m_triggSetup.bits.EXT_EDGE_SLOPE;
	m_prbTriggSlope = m_scopeDialog->m_doc->m_docSetting.m_triggSetup.bits.PRB_TRG_SLOPE;
	m_prbTriggSource = m_scopeDialog->m_doc->m_docSetting.m_triggSetup.bits.PRB_TRG_SOURCE;

	if ((m_scopeDialog->m_doc->m_docSetting.m_triggSetup.field & 0x1F) == 0x00) m_triggerFunction = 0;
	else if (m_scopeDialog->m_doc->m_docSetting.m_triggSetup.field & 0x01) m_triggerFunction = 1;
	else if ((m_scopeDialog->m_doc->m_docSetting.m_triggSetup.field & 0x1F) == 0x02) m_triggerFunction = 2;
	else if ((m_scopeDialog->m_doc->m_docSetting.m_triggSetup.field & 0x1F) == 0x10) m_triggerFunction = 3;
	else if ((m_scopeDialog->m_doc->m_docSetting.m_triggSetup.field & 0x1F) == 0x08) m_triggerFunction = 4;
	else if ((m_scopeDialog->m_doc->m_docSetting.m_triggSetup.field & 0x1F) == 0x04) m_triggerFunction = 5;
	else if ((m_scopeDialog->m_doc->m_docSetting.m_triggSetup.field & 0x1F) == 0x06) m_triggerFunction = 6;
	else if ((m_scopeDialog->m_doc->m_docSetting.m_triggSetup.field & 0x1F) == 0x0C) m_triggerFunction = 7;
	else if ((m_scopeDialog->m_doc->m_docSetting.m_triggSetup.field & 0x1F) == 0x14) m_triggerFunction = 8;
	else m_triggerFunction = -1;

	m_triggLevelA = m_scopeDialog->m_doc->m_docSetting.m_prbATriggLevel;
	m_triggLevelB = m_scopeDialog->m_doc->m_docSetting.m_prbBTriggLevel;

	m_scrollbarA.SetScrollPos(255 - m_triggLevelA);
	m_scrollbarB.SetScrollPos(255 - m_triggLevelB);

/* Nothing can change contents of symbol combo boxes other then from Format tab,
   The update of the combo boxes is therefore done in OnSetActive
	if (m_scopeDialog->m_doc->m_ptrnSelSymbol) {
		if (m_ptrnSymbolComboBox.SelectString(-1, m_scopeDialog->m_doc->m_ptrnSelSymbol->m_name) == CB_ERR) {
			MessageBox("Did not find pattern symbol", "ERROR");
		} else {
			m_ptrnRadix = m_scopeDialog->m_doc->m_ptrnSelSymbol->m_triggerRadix;
			CreateTriggPatternString();
		}
	}
	if (m_scopeDialog->m_doc->m_edgeSelSymbol) {
		if (m_edgeSymbolComboBox.SelectString(-1, m_scopeDialog->m_doc->m_edgeSelSymbol->m_name) == CB_ERR) {
			MessageBox("Did not find pattern symbol", "ERROR");
		} else {
			m_symbolEdge = m_scopeDialog->m_doc->m_docSetting.m_podPEdgeSlope;

		}
	}
*/
	UpdateVoltTriggLevel();

	UpdateData(FALSE);
}
	







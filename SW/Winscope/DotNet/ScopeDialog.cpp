// ScopeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "winscope.h"
#include "FormatDlg.h"
#include "TriggerDlg.h"
#include "MainFrm.h"
#include "WsSetting.h"
#include "WsWaveDoc.h"
#include "ScopeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScopeDialog

IMPLEMENT_DYNAMIC(CScopeDialog, CPropertySheet)

CScopeDialog::CScopeDialog(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CScopeDialog::CScopeDialog(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_samplingDlg);
	AddPage(&m_formatDlg);
	AddPage(&m_triggerDlg);
	m_samplingDlg.m_scopeDialog = this;
	m_triggerDlg.m_scopeDialog = this;
	m_formatDlg.m_scopeDialog = this;
}

CScopeDialog::~CScopeDialog()
{
}

BEGIN_MESSAGE_MAP(CScopeDialog, CPropertySheet)
	//{{AFX_MSG_MAP(CScopeDialog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void	CScopeDialog::RegisterSymbol(CPodLabel* symbol)
{
	m_triggerDlg.m_ptrnSymbolComboBox.AddString(symbol->m_name);
	if (symbol->GetWeight() == 1) {
		m_triggerDlg.m_edgeSymbolComboBox.AddString(symbol->m_name);
	}
}

BOOL	CScopeDialog::IsSymbolSelectedQ(CPodLabel* symbol)
{
	if (m_doc->m_ptrnSelSymbol == symbol) return TRUE;
	if (m_doc->m_edgeSelSymbol == symbol) return TRUE;
	return FALSE;
}

void		CScopeDialog::RegisterAllSymbols( void )
{
	if (!m_doc->m_docSetting.m_symbolList) {
		MessageBox("No symbol list in scope", "Error", MB_OK);
		return;
	}
	POSITION pos = m_doc->m_docSetting.m_symbolList->GetHeadPosition();
	while (pos) {
		RegisterSymbol(m_doc->m_docSetting.m_symbolList->GetNext(pos));
	}
}

void		CScopeDialog::KillingSymbol(CPodLabel* symbol)
{
	if (symbol == m_doc->m_ptrnSelSymbol) {
		m_triggerDlg.m_ptrnRadix = -1;
		m_triggerDlg.m_triggPtrn = "";
		m_doc->m_ptrnSelSymbol = NULL;
		m_triggerDlg.UpdateData(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CScopeDialog message handlers


BOOL CScopeDialog::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
/*	
	CRect rectWnd;
	GetWindowRect(rectWnd);
	SetWindowPos(NULL, 0, 0,
		rectWnd.Width() + 100,
		rectWnd.Height(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	GetTabControl()->GetWindowRect(rectWnd);
	GetTabControl()->SetWindowPos(NULL, 0, 0,
		rectWnd.Width() - 50,
		rectWnd.Height(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
*/
	
	CRect r;

	GetParentFrame()->GetWindowRect(&r);
	int	x = r.left + 6;
	int y = r.top + 68;
	SetWindowPos(&wndBottom, x, y, x+180, y+450, SWP_NOSIZE | SWP_NOZORDER);

	return bResult;
}

/*
void	CScopeDialog::SetScopeOwner(CWsWaveDoc* doc)
{
	m_scopeOwner = doc;

	if (!m_scopeOwner) {
		return;		// If doc release ownership return here
	}

	m_scopeSetting.m_symbolList = doc->m_docSetting.m_symbolList;	// Pointer to symbolLisrt must be passed even
																	// if data in doc settings isn't valid


	if (!doc->m_validData) return;	// If doc has no data, it should inherit this dialog data
									// and no change needs to be done here. The heritens is performed
									// by the doc itself.

	//Update dialog contents to new document settings
	unsigned long r = doc->m_docSetting.m_clockReset;
	unsigned long t = doc->m_docSetting.m_clockToggle;
	if (r == 0 && t == 0) m_samplingDlg.m_tPerDiv = 17;
	else if (r == 1 && t == 0) m_samplingDlg.m_tPerDiv = 16;
	else if (r == 4 && t == 1) m_samplingDlg.m_tPerDiv = 15;
	else if (r == 9 && t == 4) m_samplingDlg.m_tPerDiv = 14;
	else if (r == 19 && t == 9) m_samplingDlg.m_tPerDiv = 13;
	else if (r == 49 && t == 24) m_samplingDlg.m_tPerDiv = 12;
	else if (r == 99 && t == 49) m_samplingDlg.m_tPerDiv = 11;
	else if (r == 199 && t == 99) m_samplingDlg.m_tPerDiv = 10;
	else if (r == 499 && t == 249) m_samplingDlg.m_tPerDiv = 9;
	else if (r == 999 && t == 499) m_samplingDlg.m_tPerDiv = 8;
	else if (r == 1999 && t == 999) m_samplingDlg.m_tPerDiv = 7;
	else if (r == 4999 && t == 2499) m_samplingDlg.m_tPerDiv = 6;
	else if (r == 9999 && t == 4999) m_samplingDlg.m_tPerDiv = 5;
	else if (r == 19999 && t == 9999) m_samplingDlg.m_tPerDiv = 4;
	else if (r == 49999 && t == 24999) m_samplingDlg.m_tPerDiv = 3;
	else if (r == 99999 && t == 49999) m_samplingDlg.m_tPerDiv = 2;
	else if (r == 199999 && t == 99999) m_samplingDlg.m_tPerDiv = 1;
	else if (r == 499999 && t == 249999) m_samplingDlg.m_tPerDiv = 0;
	else {
		MessageBox("m_clockReset or m_clockToggle is corrupt", "Corrupt data");
		m_samplingDlg.m_tPerDiv = 0;
	}

	if (doc->m_docSetting.m_prbGain.minifields.PRBA == 0xF) m_samplingDlg.m_aVperDiv = 0;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBA == 0xB) m_samplingDlg.m_aVperDiv = 1;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBA == 0x3) m_samplingDlg.m_aVperDiv = 2;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBA == 0xE) m_samplingDlg.m_aVperDiv = 3;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBA == 0xA) m_samplingDlg.m_aVperDiv = 4;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBA == 0x2) m_samplingDlg.m_aVperDiv = 5;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBA == 0xD) m_samplingDlg.m_aVperDiv = 6;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBA == 0x9) m_samplingDlg.m_aVperDiv = 7;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBA == 0x1) m_samplingDlg.m_aVperDiv = 8;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBA == 0xC) m_samplingDlg.m_aVperDiv = 9;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBA == 0x8) m_samplingDlg.m_aVperDiv = 10;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBA == 0x0) m_samplingDlg.m_aVperDiv = 11;
	else {
		MessageBox("Probe A gain is corrupt", "Corrupt data");
		m_samplingDlg.m_aVperDiv = 0;
	}

	if (doc->m_docSetting.m_prbGain.minifields.PRBB == 0xF) m_samplingDlg.m_bVperDiv = 0;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBB == 0xB) m_samplingDlg.m_bVperDiv = 1;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBB == 0x3) m_samplingDlg.m_bVperDiv = 2;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBB == 0xE) m_samplingDlg.m_bVperDiv = 3;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBB == 0xA) m_samplingDlg.m_bVperDiv = 4;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBB == 0x2) m_samplingDlg.m_bVperDiv = 5;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBB == 0xD) m_samplingDlg.m_bVperDiv = 6;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBB == 0x9) m_samplingDlg.m_bVperDiv = 7;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBB == 0x1) m_samplingDlg.m_bVperDiv = 8;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBB == 0xC) m_samplingDlg.m_bVperDiv = 9;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBB == 0x8) m_samplingDlg.m_bVperDiv = 10;
	else if (doc->m_docSetting.m_prbGain.minifields.PRBB == 0x0) m_samplingDlg.m_bVperDiv = 11;
	else {
		MessageBox("Probe B gain is corrupt", "Corrupt data");
		m_samplingDlg.m_bVperDiv = 0;
	}

	switch (doc->m_docSetting.m_prbPodCntrl.bits.PRBA) {
	case 0x03:
		m_samplingDlg.m_aInput = 0x00;					//DC
		break;
	case 0x02:
		m_samplingDlg.m_aInput = 0x01;					//AC
		break;
	case 0x00:
		m_samplingDlg.m_aInput = 0x02;					//GND
		break;
	};

	switch (doc->m_docSetting.m_prbPodCntrl.bits.PRBB) {
	case 0x03:
		m_samplingDlg.m_bInput = 0x00;					//DC
		break;
	case 0x02:
		m_samplingDlg.m_bInput = 0x01;					//AC
		break;
	case 0x00:
		m_samplingDlg.m_bInput = 0x02;					//GND
		break;
	};

	m_samplingDlg.m_bufSize = doc->m_docSetting.m_bufferSize;
	m_samplingDlg.m_bufferSizeSlide.SetPos(m_samplingDlg.m_bufSize);
	m_samplingDlg.m_triggDelay = doc->m_docSetting.m_triggDelay;
	m_samplingDlg.m_triggDelaySlide.SetPos(m_samplingDlg.m_triggDelay);

	m_formatDlg.m_podPActive = doc->m_docSetting.m_prbPodCntrl.bits.PODP_ACTIVE;
	m_formatDlg.m_podQActive = doc->m_docSetting.m_prbPodCntrl.bits.PODQ_ACTIVE;
	m_formatDlg.m_prbAActive = doc->m_docSetting.m_prbPodCntrl.bits.PRBA_ACTIVE;
	m_formatDlg.m_prbBActive = doc->m_docSetting.m_prbPodCntrl.bits.PRBB_ACTIVE;

	m_formatDlg.m_labelName = "";

	m_triggerDlg.m_extTriggSlope = doc->m_docSetting.m_triggSetup.bits.EXT_EDGE_SLOPE;
	m_triggerDlg.m_prbTriggSlope = doc->m_docSetting.m_triggSetup.bits.PRB_TRG_SLOPE;
	m_triggerDlg.m_prbTriggSource = doc->m_docSetting.m_triggSetup.bits.PRB_TRG_SOURCE;

	if ((doc->m_docSetting.m_triggSetup.field & 0x1F) == 0x00) m_triggerDlg.m_triggerFunction = 0;
	else if (doc->m_docSetting.m_triggSetup.field & 0x01) m_triggerDlg.m_triggerFunction = 1;
	else if ((doc->m_docSetting.m_triggSetup.field & 0x1F) == 0x02) m_triggerDlg.m_triggerFunction = 2;
	else if ((doc->m_docSetting.m_triggSetup.field & 0x1F) == 0x10) m_triggerDlg.m_triggerFunction = 3;
	else if ((doc->m_docSetting.m_triggSetup.field & 0x1F) == 0x08) m_triggerDlg.m_triggerFunction = 4;
	else if ((doc->m_docSetting.m_triggSetup.field & 0x1F) == 0x04) m_triggerDlg.m_triggerFunction = 5;
	else if ((doc->m_docSetting.m_triggSetup.field & 0x1F) == 0x06) m_triggerDlg.m_triggerFunction = 6;
	else if ((doc->m_docSetting.m_triggSetup.field & 0x1F) == 0x0C) m_triggerDlg.m_triggerFunction = 7;
	else if ((doc->m_docSetting.m_triggSetup.field & 0x1F) == 0x14) m_triggerDlg.m_triggerFunction = 8;
	else m_triggerDlg.m_triggerFunction = -1;


	m_triggerDlg.m_triggLevelA = doc->m_docSetting.m_prbATriggLevel;
	m_triggerDlg.m_triggLevelB = doc->m_docSetting.m_prbBTriggLevel;

	if (doc->m_ptrnSelSymbol) {
		if (m_triggerDlg.m_ptrnSymbolComboBox.SelectString(-1, doc->m_ptrnSelSymbol->m_name) == CB_ERR) {
			MessageBox("Did not find pattern symbol", "ERROR");
		} else {
			m_triggerDlg.m_ptrnRadix = doc->m_ptrnSelSymbol->m_preferedRadix;
			m_triggerDlg.CreateTriggPatternString();
		}
	}
	if (doc->m_edgeSelSymbol) {
		if (m_triggerDlg.m_edgeSymbolComboBox.SelectString(-1, doc->m_edgeSelSymbol->m_name) == CB_ERR) {
			MessageBox("Did not find pattern symbol", "ERROR");
		} else {
			m_triggerDlg.m_symbolEdge = doc->m_docSetting.m_podPEdgeSlope;

		}
	}
	m_samplingDlg.UpdateData(FALSE);
//	m_formatDlg.UpdateData(FALSE);
//	m_triggerDlg.UpdateData(FALSE);
}

*/

void	CScopeDialog::OnSettingChanged(char ignoreAorBTrigg)
// If ignoreAorBTrigg is 'A' trigg level for A probe will not be updated acc. triggerDlg.
// Eqv. for 'B'-probe. For any other character, the triggerDlg settings will be used.
// This gives "change of VperDiv" the chance to preserve trigg voltage level
{
	UpdateData(TRUE);

	if (m_samplingDlg.m_initialized) {

		if (m_doc->m_docSetting.m_bufferSize != m_samplingDlg.m_bufSize) {
			m_doc->OnChangedBufferSize(m_samplingDlg.m_bufSize);
		}

		switch (m_samplingDlg.m_tPerDiv) {
		case 17 :
			m_doc->m_docSetting.m_clockReset = 0;
			m_doc->m_docSetting.m_clockToggle = 0;
			break;
		case 16 :
			m_doc->m_docSetting.m_clockReset = 1;
			m_doc->m_docSetting.m_clockToggle = 0;
			break;
		case 15 :
			m_doc->m_docSetting.m_clockReset = 4;
			m_doc->m_docSetting.m_clockToggle = 1;
			break;
		case 14 :
			m_doc->m_docSetting.m_clockReset = 9;
			m_doc->m_docSetting.m_clockToggle = 4;
			break;
		case 13 :
			m_doc->m_docSetting.m_clockReset = 19;
			m_doc->m_docSetting.m_clockToggle = 9;
			break;
		case 12 :
			m_doc->m_docSetting.m_clockReset = 49;
			m_doc->m_docSetting.m_clockToggle = 24;
			break;
		case 11 :
			m_doc->m_docSetting.m_clockReset = 99;
			m_doc->m_docSetting.m_clockToggle = 49;
			break;
		case 10 :
			m_doc->m_docSetting.m_clockReset = 199;
			m_doc->m_docSetting.m_clockToggle = 99;
			break;
		case 9 :
			m_doc->m_docSetting.m_clockReset = 499;
			m_doc->m_docSetting.m_clockToggle = 249;
			break;
		case 8 :
			m_doc->m_docSetting.m_clockReset = 999;
			m_doc->m_docSetting.m_clockToggle = 499;
			break;
		case 7 :
			m_doc->m_docSetting.m_clockReset = 1999;
			m_doc->m_docSetting.m_clockToggle = 999;
			break;
		case 6 :
			m_doc->m_docSetting.m_clockReset = 4999;
			m_doc->m_docSetting.m_clockToggle = 2499;
			break;
		case 5 :
			m_doc->m_docSetting.m_clockReset = 9999;
			m_doc->m_docSetting.m_clockToggle = 4999;
			break;
		case 4 :
			m_doc->m_docSetting.m_clockReset = 19999;
			m_doc->m_docSetting.m_clockToggle = 9999;
			break;
		case 3 :
			m_doc->m_docSetting.m_clockReset = 49999;
			m_doc->m_docSetting.m_clockToggle = 24999;
			break;
		case 2 :
			m_doc->m_docSetting.m_clockReset = 99999;
			m_doc->m_docSetting.m_clockToggle = 49999;
			break;
		case 1 :
			m_doc->m_docSetting.m_clockReset = 199999;
			m_doc->m_docSetting.m_clockToggle = 99999;
			break;
		case 0 :
			m_doc->m_docSetting.m_clockReset = 499999;
			m_doc->m_docSetting.m_clockToggle = 249999;
			break;
		}

		switch (m_samplingDlg.m_aVperDiv) {
		case 0 :
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP1 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT1 = 1;
			break;
		case 1 :
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP1 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT1 = 1;
			break;
		case 2 :
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP1 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT1 = 1;
			break;
		case 3 :
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP1 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT1 = 1;
			break;
		case 4 :
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP1 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT1 = 1;
			break;
		case 5 :
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP1 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT1 = 1;
			break;
		case 6 :
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP1 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT1 = 0;
			break;
		case 7 :
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP1 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT1 = 0;
			break;
		case 8 :
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP1 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT1 = 0;
			break;
		case 9 :
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP1 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT1 = 0;
			break;
		case 10 :
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP1 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT1 = 0;
			break;
		case 11 :
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_AMP1 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBA_ATT1 = 0;
			break;
		}

		switch (m_samplingDlg.m_bVperDiv) {
		case 0 :
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP1 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT1 = 1;
			break;
		case 1 :
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP1 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT1 = 1;
			break;
		case 2 :
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP1 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT1 = 1;
			break;
		case 3 :
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP1 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT1 = 1;
			break;
		case 4 :
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP1 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT1 = 1;
			break;
		case 5 :
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP1 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT1 = 1;
			break;
		case 6 :
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP1 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT1 = 0;
			break;
		case 7 :
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP1 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT1 = 0;
			break;
		case 8 :
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP1 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT1 = 0;
			break;
		case 9 :
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP0 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP1 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT1 = 0;
			break;
		case 10 :
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP1 = 1;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT1 = 0;
			break;
		case 11 :
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_AMP1 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT0 = 0;
			m_doc->m_docSetting.m_prbGain.bits.PRBB_ATT1 = 0;
			break;
		}
		switch (m_samplingDlg.m_aInput) {
		case 0x00:
			m_doc->m_docSetting.m_prbPodCntrl.bits.PRBA = 0x03;	//DC
			break;
		case 0x01:
			m_doc->m_docSetting.m_prbPodCntrl.bits.PRBA = 0x02;	//AC
			break;
		case 0x02:
			m_doc->m_docSetting.m_prbPodCntrl.bits.PRBA = 0x00;	//GND
			break;
		};

		switch (m_samplingDlg.m_bInput) {
		case 0x00:
			m_doc->m_docSetting.m_prbPodCntrl.bits.PRBB = 0x03;	//DC
			break;
		case 0x01:
			m_doc->m_docSetting.m_prbPodCntrl.bits.PRBB = 0x02;	//AC
			break;
		case 0x02:
			m_doc->m_docSetting.m_prbPodCntrl.bits.PRBB = 0x00;	//GND
			break;
		};
		m_doc->m_docSetting.m_control.bits.CLOCK_SELECT = m_samplingDlg.m_clockSelection;
		m_doc->m_docSetting.m_triggDelay = m_samplingDlg.m_triggDelay;
		m_doc->m_docSetting.m_control.bits.SWEEP_MODE = m_samplingDlg.m_sweepMode;
	}

	if (m_formatDlg.m_initialized) {
	
		m_doc->m_docSetting.m_prbPodCntrl.bits.PODP_ACTIVE = m_formatDlg.m_podPActive;
		m_doc->m_docSetting.m_prbPodCntrl.bits.PODQ_ACTIVE = m_formatDlg.m_podQActive;
		m_doc->m_docSetting.m_prbPodCntrl.bits.PRBA_ACTIVE = m_formatDlg.m_prbAActive;
		m_doc->m_docSetting.m_prbPodCntrl.bits.PRBB_ACTIVE = m_formatDlg.m_prbBActive;

	}

	if (m_triggerDlg.m_initialized) {

		m_doc->m_docSetting.m_triggSetup.bits.EXT_EDGE_SLOPE = m_triggerDlg.m_extTriggSlope;
		m_doc->m_docSetting.m_triggSetup.bits.PRB_TRG_SLOPE = m_triggerDlg.m_prbTriggSlope;
		m_doc->m_docSetting.m_triggSetup.bits.PRB_TRG_SOURCE = m_triggerDlg.m_prbTriggSource;
		switch (m_triggerDlg.m_triggerFunction) {
		case 0:
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_ALWAYS = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_EXT_EDGE = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_EDGE = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_PTRN = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_PRB_EDGE = 0;
			break;
		case 1:
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_ALWAYS = 1;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_EXT_EDGE = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_EDGE = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_PTRN = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_PRB_EDGE = 0;
			break;
		case 2:
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_ALWAYS = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_EXT_EDGE = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_EDGE = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_PTRN = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_PRB_EDGE = 1;
			break;
		case 3:
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_ALWAYS = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_EXT_EDGE = 1;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_EDGE = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_PTRN = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_PRB_EDGE = 0;
			break;
		case 4:
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_ALWAYS = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_EXT_EDGE = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_EDGE = 1;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_PTRN = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_PRB_EDGE = 0;
			break;
		case 5:
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_ALWAYS = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_EXT_EDGE = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_EDGE = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_PTRN = 1;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_PRB_EDGE = 0;
			break;
		case 6:
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_ALWAYS = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_EXT_EDGE = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_EDGE = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_PTRN = 1;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_PRB_EDGE = 1;
			break;
		case 7:
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_ALWAYS = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_EXT_EDGE = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_EDGE = 1;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_PTRN = 1;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_PRB_EDGE = 0;
			break;
		case 8:
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_ALWAYS = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_EXT_EDGE = 1;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_EDGE = 0;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_POD_PTRN = 1;
			m_doc->m_docSetting.m_triggSetup.bits.TRIGG_PRB_EDGE = 0;
			break;
		}
		if (ignoreAorBTrigg != 'A') {
			m_doc->m_docSetting.m_prbATriggLevel = m_triggerDlg.m_triggLevelA;
		}
		if (ignoreAorBTrigg != 'B') {
			m_doc->m_docSetting.m_prbBTriggLevel = m_triggerDlg.m_triggLevelB;
		}
	}
		

	if (m_doc->m_ptrnSelSymbol != NULL) {
		m_doc->m_docSetting.m_podPPtrnValid = m_doc->m_ptrnSelSymbol->m_validPtrnP;
		m_doc->m_docSetting.m_podQPtrnValid = m_doc->m_ptrnSelSymbol->m_validPtrnQ;
		m_doc->m_docSetting.m_podPTriggPtrn = m_doc->m_ptrnSelSymbol->m_triggPtrnP;
		m_doc->m_docSetting.m_podQTriggPtrn = m_doc->m_ptrnSelSymbol->m_triggPtrnQ;
	} else {
		m_doc->m_docSetting.m_podPPtrnValid = 0x00;
		m_doc->m_docSetting.m_podQPtrnValid = 0x00;
		m_doc->m_docSetting.m_podPTriggPtrn = 0x00;
		m_doc->m_docSetting.m_podQTriggPtrn = 0x00;
	}

	m_doc->m_docSetting.m_podPEdgeSlope = 0x00;
	m_doc->m_docSetting.m_podQEdgeSlope = 0x00;
	if (m_doc->m_edgeSelSymbol != NULL) {
		m_doc->m_docSetting.m_podPEdgeValid = m_doc->m_edgeSelSymbol->m_validPtrnP;
		m_doc->m_docSetting.m_podQEdgeValid = m_doc->m_edgeSelSymbol->m_validPtrnQ;
		if (m_triggerDlg.m_symbolEdge == WS_POS_EDGE) {
			m_doc->m_docSetting.m_podPEdgeSlope = 0xFF;						// Only one bit will be valid
			m_doc->m_docSetting.m_podQEdgeSlope = 0xFF;						// - " -
		}
	} else {
		m_doc->m_docSetting.m_podPEdgeValid = 0x00;
		m_doc->m_docSetting.m_podQEdgeValid = 0x00;
	}

//	m_doc->m_docSetting.m_symbolList = m_formatDlg.m_labelDataList;

	m_doc->UserChangedSetting(SM_SCOPE_DIALOG);
//	m_doc->ScopeSettingChanged();
}

void	CScopeDialog::UpdateSettingsView( void )
{
	if (m_samplingDlg.m_initialized) m_samplingDlg.UpdateSettingsView();
	if (m_formatDlg.m_initialized) m_formatDlg.UpdateSettingsView();
	if (m_triggerDlg.m_initialized) m_triggerDlg.UpdateSettingsView();
}



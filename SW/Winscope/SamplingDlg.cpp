// SamplingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscope.h"
#include "ScopeDialog.h"
#include "WsWaveDoc.h"
#include "SetBuffSizeDlg.h"
#include "SamplingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSamplingDlg dialog

IMPLEMENT_DYNCREATE(CSamplingDlg, CPropertyPage)

CSamplingDlg::CSamplingDlg() : CPropertyPage(CSamplingDlg::IDD)
{
	//{{AFX_DATA_INIT(CSamplingDlg)
	m_aInput = 0;
	m_aVperDiv = 0;
	m_bInput = 0;
	m_bVperDiv = 0;
	m_bufSize = 0;
	m_clockSelection = 0;
	m_tPerDiv = 0;
	m_triggDelay = 0;
	m_sweepMode = -1;
	//}}AFX_DATA_INIT
	m_initialized = FALSE;
}

CSamplingDlg::~CSamplingDlg()
{
}

void CSamplingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSamplingDlg)
	DDX_Control(pDX, IDC_BUF_SIZE_SLIDER, m_bufferSizeSlide);
	DDX_Control(pDX, IDC_TRIG_DELY_SLIDER, m_triggDelaySlide);
	DDX_Radio(pDX, IDC_A_INPUT, m_aInput);
	DDX_CBIndex(pDX, IDC_A_VPERDIV, m_aVperDiv);
	DDX_Radio(pDX, IDC_B_INPUT, m_bInput);
	DDX_CBIndex(pDX, IDC_B_VPERDIV, m_bVperDiv);
	DDX_Text(pDX, IDC_BUFFER_SIZE, m_bufSize);
	DDX_Radio(pDX, IDC_CLOCK_SELECTION, m_clockSelection);
	DDX_CBIndex(pDX, IDC_TPERDIV, m_tPerDiv);
	DDX_Text(pDX, IDC_TRIGGER_DELAY, m_triggDelay);
	DDX_Radio(pDX, IDC_SM_NORMAL, m_sweepMode);
	//}}AFX_DATA_MAP
	DDV_MinMaxUInt(pDX, m_bufSize, 0, SC_MAX_BUFER_SIZE);
	DDV_MinMaxUInt(pDX, m_triggDelay, 0, SC_MAX_BUFER_SIZE);
}


BEGIN_MESSAGE_MAP(CSamplingDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CSamplingDlg)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_BUF_SIZE_SLIDER, OnReleasedcaptureBufSizeSlider)
	ON_EN_KILLFOCUS(IDC_BUFFER_SIZE, OnKillfocusBufferSize)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_TRIG_DELY_SLIDER, OnReleasedcaptureTrigDelySlider)
	ON_EN_KILLFOCUS(IDC_TRIGGER_DELAY, OnKillfocusTriggerDelay)
	ON_CBN_SELCHANGE(IDC_A_VPERDIV, OnSelchangeAVperdiv)
	ON_CBN_SELCHANGE(IDC_B_VPERDIV, OnSelchangeBVperdiv)
	ON_CBN_SELCHANGE(IDC_TPERDIV, OnSelchangeTperdiv)
	ON_BN_CLICKED(IDC_A_INPUT, OnAInput)
	ON_BN_CLICKED(IDC_A_I_AC, OnAIAc)
	ON_BN_CLICKED(IDC_A_I_GND, OnAIGnd)
	ON_BN_CLICKED(IDC_B_INPUT, OnBInput)
	ON_BN_CLICKED(IDC_B_I_AC, OnBIAc)
	ON_BN_CLICKED(IDC_B_I_GND, OnBIGnd)
	ON_BN_CLICKED(IDC_CLOCK_SELECTION, OnClockSelection)
	ON_BN_CLICKED(IDC_CLOCK_SEL_EXT, OnClockSelExt)
	ON_BN_CLICKED(IDC_SM_NORMAL, OnSmNormal)
	ON_BN_CLICKED(IDC_SM_AUTO, OnSmAuto)
	ON_BN_CLICKED(IDC_SWEEP_MODE, OnSweepMode)
	ON_BN_CLICKED(IDC_NON_SWEEP_SETTINGS, OnNonSweepSettings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSamplingDlg message handlers

BOOL CSamplingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect rectWnd;
	GetWindowRect(rectWnd);
	SetWindowPos(NULL, 0, 0,
		rectWnd.Width() - 100,
		rectWnd.Height(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	UpdateSettingsView();

	m_initialized = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSamplingDlg::OnReleasedcaptureBufSizeSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_bufSize = m_bufferSizeSlide.GetPos();
	UpdateData(FALSE);

	m_triggDelaySlide.SetRange(0, m_bufSize, TRUE);
	
	*pResult = 0;

	m_scopeDialog->OnSettingChanged(' ');
}

void CSamplingDlg::OnKillfocusBufferSize() 
{
	UINT	oldSize = m_bufSize;
	UpdateData(TRUE);
	if (m_bufSize != oldSize) {
		m_triggDelaySlide.SetRange(0, m_bufSize, TRUE);
		m_bufferSizeSlide.SetPos(m_bufSize);
		m_scopeDialog->OnSettingChanged(' ');
	}
}

void CSamplingDlg::OnReleasedcaptureTrigDelySlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_triggDelay = m_triggDelaySlide.GetPos();
	UpdateData(FALSE);
	
	*pResult = 0;
	m_scopeDialog->OnSettingChanged(' ');
}

void CSamplingDlg::OnKillfocusTriggerDelay() 
{
	UINT	oldDelay = m_triggDelay;
	UpdateData(TRUE);
	if (m_triggDelay != oldDelay) {
		m_triggDelaySlide.SetPos(m_triggDelay);
		m_scopeDialog->OnSettingChanged(' ');
	}
}

void CSamplingDlg::OnSelchangeAVperdiv() 
{
	// Try to preserve trigg level
	double	oldRange = m_scopeDialog->m_doc->m_docSetting.GetScreenVoltageRange('A');
	double	newRange;
	double	triggLevelVoltage;
	int		newTriggLevel;

	UpdateData(TRUE);

	switch (m_aVperDiv) {
	case 0 :
		newRange = 200.0;
		break;
	case 1 :
		newRange = 80.0;
		break;
	case 2 :
		newRange = 40.0;
		break;
	case 3 :
		newRange = 20.0;
		break;
	case 4 :
		newRange = 8.0;
		break;
	case 5 :
		newRange = 4.0;
		break;
	case 6 :
		newRange = 2.0;
		break;
	case 7 :
		newRange = 0.80;
		break;
	case 8 :
		newRange = 0.40;
		break;
	case 9 :
		newRange = 0.20;
		break;
	case 10 :
		newRange = 0.08;
		break;
	case 11 :
		newRange = 0.04;
		break;
	}

	triggLevelVoltage = (((double) m_scopeDialog->m_doc->m_docSetting.m_prbATriggLevel) - 128.0) / 128.0 * oldRange;
	newTriggLevel = (int) (triggLevelVoltage / newRange * 128.0 + 128.0);
	if (newTriggLevel > 255) newTriggLevel = 255;
	if (newTriggLevel < 0) newTriggLevel = 0;
	m_scopeDialog->m_doc->m_docSetting.m_prbATriggLevel = (unsigned char) newTriggLevel;
	m_scopeDialog->OnSettingChanged('A');
}

void CSamplingDlg::OnSelchangeBVperdiv() 
{
	// Try to preserve trigg level
	double	oldRange = m_scopeDialog->m_doc->m_docSetting.GetScreenVoltageRange('B');
	double	newRange;
	double	triggLevelVoltage;
	int		newTriggLevel;

	UpdateData(TRUE);

	switch (m_bVperDiv) {
	case 0 :
		newRange = 200.0;
		break;
	case 1 :
		newRange = 80.0;
		break;
	case 2 :
		newRange = 40.0;
		break;
	case 3 :
		newRange = 20.0;
		break;
	case 4 :
		newRange = 8.0;
		break;
	case 5 :
		newRange = 4.0;
		break;
	case 6 :
		newRange = 2.0;
		break;
	case 7 :
		newRange = 0.80;
		break;
	case 8 :
		newRange = 0.40;
		break;
	case 9 :
		newRange = 0.20;
		break;
	case 10 :
		newRange = 0.08;
		break;
	case 11 :
		newRange = 0.04;
		break;
	}

	triggLevelVoltage = (((double) m_scopeDialog->m_doc->m_docSetting.m_prbBTriggLevel) - 128.0) / 128.0 * oldRange;
	newTriggLevel = (int) (triggLevelVoltage / newRange * 128.0 + 128.0);
	if (newTriggLevel > 255) newTriggLevel = 255;
	if (newTriggLevel < 0) newTriggLevel = 0;
	m_scopeDialog->m_doc->m_docSetting.m_prbBTriggLevel = (unsigned char) newTriggLevel;
	m_scopeDialog->OnSettingChanged('B');
}

void CSamplingDlg::OnSelchangeTperdiv() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CSamplingDlg::OnAInput() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CSamplingDlg::OnAIAc() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CSamplingDlg::OnAIGnd() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CSamplingDlg::OnBInput() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CSamplingDlg::OnBIAc() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CSamplingDlg::OnBIGnd() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CSamplingDlg::OnClockSelection() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CSamplingDlg::OnClockSelExt() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CSamplingDlg::OnSmNormal() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CSamplingDlg::OnSmAuto() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CSamplingDlg::OnSweepMode() 
{
	UpdateData(TRUE);
	m_scopeDialog->OnSettingChanged(' ');
}

void CSamplingDlg::OnNonSweepSettings() 
{
	CSetBuffSizeDlg	dlg;

	if (!m_scopeDialog->m_doc->m_sweepActive) {		// Not allowed if sweep is active
		dlg.m_bufferSize = m_bufSize;
		if (dlg.DoModal() == IDOK) {
			m_bufSize = dlg.m_bufferSize;
			UpdateData(FALSE);
			m_triggDelaySlide.SetRange(0, m_bufSize, TRUE);
			m_scopeDialog->OnSettingChanged(' ');
		}
	} else {
		MessageBox("Changeing buffer size can not be\ndone during an active sweep", "Illegal", MB_ICONEXCLAMATION | MB_OK);
	}
}

void	CSamplingDlg::UpdateSettingsView( void )
//Update dialog contents to new document settings
{
	unsigned long r = m_scopeDialog->m_doc->m_docSetting.m_clockReset;
	unsigned long t = m_scopeDialog->m_doc->m_docSetting.m_clockToggle;
	if (r == 0 && t == 0) m_tPerDiv = 17;
	else if (r == 1 && t == 0) m_tPerDiv = 16;
	else if (r == 4 && t == 1) m_tPerDiv = 15;
	else if (r == 9 && t == 4) m_tPerDiv = 14;
	else if (r == 19 && t == 9) m_tPerDiv = 13;
	else if (r == 49 && t == 24) m_tPerDiv = 12;
	else if (r == 99 && t == 49) m_tPerDiv = 11;
	else if (r == 199 && t == 99) m_tPerDiv = 10;
	else if (r == 499 && t == 249) m_tPerDiv = 9;
	else if (r == 999 && t == 499) m_tPerDiv = 8;
	else if (r == 1999 && t == 999) m_tPerDiv = 7;
	else if (r == 4999 && t == 2499) m_tPerDiv = 6;
	else if (r == 9999 && t == 4999) m_tPerDiv = 5;
	else if (r == 19999 && t == 9999) m_tPerDiv = 4;
	else if (r == 49999 && t == 24999) m_tPerDiv = 3;
	else if (r == 99999 && t == 49999) m_tPerDiv = 2;
	else if (r == 199999 && t == 99999) m_tPerDiv = 1;
	else if (r == 499999 && t == 249999) m_tPerDiv = 0;
	else {
		MessageBox("m_clockReset or m_clockToggle is corrupt", "Corrupt data");
		m_tPerDiv = 0;
	}

	if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBA == 0xF) m_aVperDiv = 0;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBA == 0xB) m_aVperDiv = 1;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBA == 0x3) m_aVperDiv = 2;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBA == 0xE) m_aVperDiv = 3;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBA == 0xA) m_aVperDiv = 4;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBA == 0x2) m_aVperDiv = 5;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBA == 0xD) m_aVperDiv = 6;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBA == 0x9) m_aVperDiv = 7;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBA == 0x1) m_aVperDiv = 8;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBA == 0xC) m_aVperDiv = 9;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBA == 0x8) m_aVperDiv = 10;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBA == 0x0) m_aVperDiv = 11;
	else {
		MessageBox("Probe A gain is corrupt", "Corrupt data");
		m_aVperDiv = 0;
	}

	if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBB == 0xF) m_bVperDiv = 0;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBB == 0xB) m_bVperDiv = 1;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBB == 0x3) m_bVperDiv = 2;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBB == 0xE) m_bVperDiv = 3;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBB == 0xA) m_bVperDiv = 4;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBB == 0x2) m_bVperDiv = 5;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBB == 0xD) m_bVperDiv = 6;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBB == 0x9) m_bVperDiv = 7;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBB == 0x1) m_bVperDiv = 8;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBB == 0xC) m_bVperDiv = 9;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBB == 0x8) m_bVperDiv = 10;
	else if (m_scopeDialog->m_doc->m_docSetting.m_prbGain.minifields.PRBB == 0x0) m_bVperDiv = 11;
	else {
		MessageBox("Probe B gain is corrupt", "Corrupt data");
		m_bVperDiv = 0;
	}

	switch (m_scopeDialog->m_doc->m_docSetting.m_prbPodCntrl.bits.PRBA) {
	case 0x03:
		m_aInput = 0x00;					//DC
		break;
	case 0x02:
		m_aInput = 0x01;					//AC
		break;
	case 0x00:
		m_aInput = 0x02;					//GND
		break;
	};

	switch (m_scopeDialog->m_doc->m_docSetting.m_prbPodCntrl.bits.PRBB) {
	case 0x03:
		m_bInput = 0x00;					//DC
		break;
	case 0x02:
		m_bInput = 0x01;					//AC
		break;
	case 0x00:
		m_bInput = 0x02;					//GND
		break;
	};

	m_bufSize = m_scopeDialog->m_doc->m_docSetting.m_bufferSize;
	m_bufferSizeSlide.SetRange(0, SC_MAX_BUFER_SIZE);
	m_bufferSizeSlide.SetPos(m_bufSize);
	m_triggDelay = m_scopeDialog->m_doc->m_docSetting.m_triggDelay;
	m_triggDelaySlide.SetRange(0, m_bufSize, TRUE);
	m_triggDelaySlide.SetPos(m_triggDelay);

	m_sweepMode = m_scopeDialog->m_doc->m_docSetting.m_control.bits.SWEEP_MODE;

	UpdateData(FALSE);
}




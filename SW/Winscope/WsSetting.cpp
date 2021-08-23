// WsSetting.cpp: implementation of the CWsSetting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "winscope.h"
#include "RegisterIF.h"
#include "WsSetting.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CWsSetting, CObject, 1);

CWsSetting::CWsSetting()
{
	m_bufferSize = 1024;
	m_clockReset = 499;
	m_clockToggle = 249;
	m_control.field = 0;
	m_control.bits.CLOCK_SELECT = 0;
	m_control.bits.IRQ_EN = 1;
	m_control.bits.SWEEP_MODE = TC_SM_SINGLE;
	m_leds = 0xff;
	m_podPEdgeSlope = 0;
	m_podQEdgeSlope = 0;
	m_podPEdgeValid = 0;
	m_podQEdgeValid = 0;
	m_podPPtrnValid = 0;
	m_podQPtrnValid = 0;
	m_podPTriggPtrn = 0;
	m_podQTriggPtrn = 0;
	m_prbATriggLevel = 128;
	m_prbBTriggLevel = 128;
	m_prbGain.bits.PRBA_AMP0 = 0;
	m_prbGain.bits.PRBA_AMP1 = 0;
	m_prbGain.bits.PRBA_ATT0 = 0;
	m_prbGain.bits.PRBA_ATT1 = 1;
	m_prbGain.bits.PRBB_AMP0 = 0;
	m_prbGain.bits.PRBB_AMP1 = 0;
	m_prbGain.bits.PRBB_ATT0 = 0;
	m_prbGain.bits.PRBB_ATT1 = 1;
	m_prbPodCntrl.bits.PODP_ACTIVE = 1;
	m_prbPodCntrl.bits.PODQ_ACTIVE = 1;
	m_prbPodCntrl.bits.PRBA_ACTIVE = 1;
	m_prbPodCntrl.bits.PRBB_ACTIVE = 1;
	m_prbPodCntrl.bits.PRBA = 0;
	m_prbPodCntrl.bits.PRBB = 0;
	m_triggDelay = 512;
	m_triggSetup.field = 0;
}

CWsSetting::~CWsSetting()
{

}


CWsSetting& CWsSetting::operator=(const CWsSetting& s)
{
	m_triggSetup = s.m_triggSetup;
	m_prbATriggLevel = s.m_prbATriggLevel;
	m_prbBTriggLevel = s.m_prbBTriggLevel;
	m_podPTriggPtrn = s.m_podPTriggPtrn;
	m_podQTriggPtrn = s.m_podQTriggPtrn;
	m_podPPtrnValid = s.m_podPPtrnValid;
	m_podQPtrnValid = s.m_podQPtrnValid;
	m_podPEdgeSlope = s.m_podPEdgeSlope;
	m_podQEdgeSlope = s.m_podQEdgeSlope;
	m_podPEdgeValid = s.m_podPEdgeValid;
	m_podQEdgeValid = s.m_podQEdgeValid;
	m_triggDelay = s.m_triggDelay;
	m_bufferSize = s.m_bufferSize;
	m_clockToggle = s.m_clockToggle;
	m_clockReset = s.m_clockReset;
	m_control = s.m_control;
	m_prbPodCntrl = s.m_prbPodCntrl;
	m_prbGain = s.m_prbGain;
	m_leds = s.m_leds;
	m_symbolList = s.m_symbolList;

	return *this;
}


void CWsSetting::Serialize( CArchive& ar )
// Member variable m_symbolList is not serialized.
{
	if (ar.IsStoring())	{
		ar << m_triggSetup.field;
		ar << m_prbATriggLevel;
		ar << m_prbBTriggLevel;
		ar << m_podPTriggPtrn;
		ar << m_podQTriggPtrn;
		ar << m_podPPtrnValid;
		ar << m_podQPtrnValid;
		ar << m_podPEdgeSlope;
		ar << m_podQEdgeSlope;
		ar << m_podPEdgeValid;
		ar << m_podQEdgeValid;
		ar << m_triggDelay;
		ar << m_bufferSize;
		ar << m_clockToggle;
		ar << m_clockReset;
		ar << m_control.field;
		ar << m_prbPodCntrl.field;
		ar << m_prbGain.field;
		ar << m_leds;
	} else {
		ar >> m_triggSetup.field;
		ar >> m_prbATriggLevel;
		ar >> m_prbBTriggLevel;
		ar >> m_podPTriggPtrn;
		ar >> m_podQTriggPtrn;
		ar >> m_podPPtrnValid;
		ar >> m_podQPtrnValid;
		ar >> m_podPEdgeSlope;
		ar >> m_podQEdgeSlope;
		ar >> m_podPEdgeValid;
		ar >> m_podQEdgeValid;
		ar >> m_triggDelay;
		ar >> m_bufferSize;
		ar >> m_clockToggle;
		ar >> m_clockReset;
		ar >> m_control.field;
		ar >> m_prbPodCntrl.field;
		ar >> m_prbGain.field;
		ar >> m_leds;
	}
}

POSITION	CWsSetting::GetSymbolPos(CString searchName)
{
	POSITION oldPos;
	POSITION pos = m_symbolList->GetHeadPosition();
	while (pos) {
		oldPos = pos;
		if (searchName == m_symbolList->GetNext(pos)->m_name) return oldPos;
	}
	return NULL;
}

CPodLabel*	CWsSetting::GetSymbol(CString searchName)
{
	CPodLabel*	tmpSymbol;
	POSITION pos = m_symbolList->GetHeadPosition();
	while (pos) {
		tmpSymbol = m_symbolList->GetNext(pos);
		if (searchName == tmpSymbol->m_name) return tmpSymbol;
	}
	return NULL;
}

double	CWsSetting::GetScreenVoltageRange(char AorB)
//Parameter is 'A' or 'B' resp. channel
{
	unsigned char	field;

	if (AorB == 'A') {
		field = m_prbGain.minifields.PRBA;
	} else if (AorB == 'B') {
		field = m_prbGain.minifields.PRBB;
	} else {
		return 0.00000001;
	}

	switch (field) {
	case 0xF:
		return 200.0;
	case 0xB:
		return 80.0;
	case 0x3:
		return 40.0;
	case 0xE:
		return 20.0;
	case 0xA:
		return 8.0;
	case 0x2:
		return 4.0;
	case 0xD:
		return 2.0;
	case 0x9:
		return 0.80;
	case 0x1:
		return 0.40;
	case 0xC:
		return 0.20;
	case 0x8:
		return 0.08;
	case 0x0:
		return 0.04;
	}
	return 0.00000001;
}


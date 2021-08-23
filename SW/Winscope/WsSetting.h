// WsSetting.h: interface for the CWsSetting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WSSETTING_H__31903C1F_0979_4C09_B1B9_18D5AF80A130__INCLUDED_)
#define AFX_WSSETTING_H__31903C1F_0979_4C09_B1B9_18D5AF80A130__INCLUDED_

#include <afxtempl.h>
#include "PodLabel.h"
#include "RegisterIF.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWsSetting : public CObject  
{
public:
	CWsSetting();
	DECLARE_SERIAL( CWsSetting )
	virtual ~CWsSetting();
	CWsSetting& operator=(const CWsSetting& s);
	virtual void Serialize( CArchive& ar );
	

	TRIGG_SETUP_TYPE	m_triggSetup;
	unsigned char		m_prbATriggLevel;
	unsigned char		m_prbBTriggLevel;
	unsigned char		m_podPTriggPtrn;
	unsigned char		m_podQTriggPtrn;
	unsigned char		m_podPPtrnValid;
	unsigned char		m_podQPtrnValid;
	unsigned char		m_podPEdgeSlope;
	unsigned char		m_podQEdgeSlope;
	unsigned char		m_podPEdgeValid;
	unsigned char		m_podQEdgeValid;
	unsigned long		m_triggDelay;
	unsigned long		m_bufferSize;
	unsigned long		m_clockToggle;
	unsigned long		m_clockReset;
	CONTROL_TYPE		m_control;
	PRB_POD_CTRL_TYPE	m_prbPodCntrl;
	PRB_GAIN_TYPE		m_prbGain;
	unsigned char		m_leds;
	CTypedPtrList<CObList, CPodLabel*>*	m_symbolList;


	POSITION			GetSymbolPos(CString searchName);
	CPodLabel*			GetSymbol(CString searchName);
	double				GetScreenVoltageRange(char AorB);

};

#endif // !defined(AFX_WSSETTING_H__31903C1F_0979_4C09_B1B9_18D5AF80A130__INCLUDED_)

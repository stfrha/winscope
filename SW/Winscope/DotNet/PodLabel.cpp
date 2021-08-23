// PodLabel.cpp: implementation of the CPodLabel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "winscope.h"
#include "PodLabel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL( CPodLabel, CObject, 2 )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPodLabel::CPodLabel(CString name, unsigned char p, unsigned char q)
{
	m_name = name;
	m_validPtrnP = p;
	m_validPtrnQ = q;
	m_triggerRadix = 0;
	m_triggPtrnP = 0;
	m_triggPtrnQ = 0;
	m_viewRadix = 0;
}

CPodLabel::CPodLabel()
{
	m_triggerRadix = 0;
	m_triggPtrnP = 0;
	m_triggPtrnQ = 0;
	m_viewRadix = 0;
}

CPodLabel::~CPodLabel()
{

}

void CPodLabel::Serialize( CArchive& ar )
{
	if (ar.IsStoring())
	{
		ar << m_name;
		ar << m_triggerRadix;
		ar << m_triggPtrnP;
		ar << m_triggPtrnQ;
		ar << m_validPtrnP;
		ar << m_validPtrnQ;
		ar << m_viewRadix;
	}
	else
	{
		ar >> m_name;
		ar >> m_triggerRadix;
		ar >> m_triggPtrnP;
		ar >> m_triggPtrnQ;
		ar >> m_validPtrnP;
		ar >> m_validPtrnQ;
		ar >> m_viewRadix;
	}
}

int	CPodLabel::GetWeight( void )
{
	int	weight = 0;
	int	i;

	for (i=0 ; i<8 ; i++) {
		if (m_validPtrnP & (1<<i)) weight++;
		if (m_validPtrnQ & (1<<i)) weight++;
	}
	return weight;
}


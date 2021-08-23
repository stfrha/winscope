// WaveData.h: interface for the CWaveData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEDATA_H__9DA8919B_1FC0_4FA7_8C3D_38BD85A9AD32__INCLUDED_)
#define AFX_WAVEDATA_H__9DA8919B_1FC0_4FA7_8C3D_38BD85A9AD32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWaveData : public CObject  
{
public:
	char*	m_probeAData;
	char*	m_probeBData;
	char*	m_podPData;
	char*	m_podQData;
	long	m_triggDelay;
	long	m_bufferSize;

	CWaveData();
	virtual ~CWaveData();

};

#endif // !defined(AFX_WAVEDATA_H__9DA8919B_1FC0_4FA7_8C3D_38BD85A9AD32__INCLUDED_)

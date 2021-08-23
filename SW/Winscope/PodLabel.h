// PodLabel.h: interface for the CPodLabel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PODLABEL_H__6E96711B_21B0_45C8_8DB3_A24665C2A9F0__INCLUDED_)
#define AFX_PODLABEL_H__6E96711B_21B0_45C8_8DB3_A24665C2A9F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// PodLabel Radix
#define	PLR_HEX				0x00
#define	PLR_BIN				0x01
#define	PLR_SIGNED			0x02
#define	PLR_UNSIGNED		0x03


class CPodLabel : public CObject  
{
public:
	CPodLabel();
	DECLARE_SERIAL( CPodLabel )
	CPodLabel(CString name, unsigned char p, unsigned char q);
	virtual ~CPodLabel();
	virtual void Serialize( CArchive& ar );

	int				GetWeight( void );

	CString			m_name;
	unsigned char	m_validPtrnP;
	unsigned char	m_validPtrnQ;
	unsigned char	m_triggPtrnP;
	unsigned char	m_triggPtrnQ;
	int				m_triggerRadix;
	int				m_viewRadix;
};

#endif // !defined(AFX_PODLABEL_H__6E96711B_21B0_45C8_8DB3_A24665C2A9F0__INCLUDED_)

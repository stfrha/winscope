// BitFileReader.h: interface for the CBitFileReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BITFILEREADER_H__85D3D71D_C0F2_4E1E_9511_E26CDA2824CD__INCLUDED_)
#define AFX_BITFILEREADER_H__85D3D71D_C0F2_4E1E_9511_E26CDA2824CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBitFileReader  
{
public:
	CBitFileReader();
	virtual			~CBitFileReader();
	BOOL			OpenBitFile(CString path);
	void			CloseBitFile( void );
	unsigned char	GetNextByte( void );
private:
	FILE*			m_fp;
	CString			m_path;
};

#endif // !defined(AFX_BITFILEREADER_H__85D3D71D_C0F2_4E1E_9511_E26CDA2824CD__INCLUDED_)

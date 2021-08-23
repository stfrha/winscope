// BitFileReader.cpp: implementation of the CBitFileReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BitFileReader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBitFileReader::CBitFileReader()
{
	m_fp = NULL;
}

CBitFileReader::~CBitFileReader()
{
}

BOOL	CBitFileReader::OpenBitFile(CString path)
{
	int		c;

	m_path = path;

	m_fp = fopen(m_path, "rb");
	if (!m_fp) return FALSE;
	do {
		c = fgetc(m_fp);
	} while (c != 0xff);
	ungetc(c, m_fp);
	return TRUE;
}

void	CBitFileReader::CloseBitFile( void )
{
	if (m_fp) fclose(m_fp);
	m_fp = NULL;
}

unsigned char	CBitFileReader::GetNextByte( void )
{
	char c, d;

	c = fgetc(m_fp);

	if (feof(m_fp)) MessageBox(NULL, "Error: end of file reached", "Error", MB_OK);

	d = 0;

	if (c & 0x01) d = (char) 0x80;
	if (c & 0x02) d += 0x40;
	if (c & 0x04) d += 0x20;
	if (c & 0x08) d += 0x10;
	if (c & 0x10) d += 0x08;
	if (c & 0x20) d += 0x04;
	if (c & 0x40) d += 0x02;
	if (c & 0x80) d += 0x01;
	return d;
}

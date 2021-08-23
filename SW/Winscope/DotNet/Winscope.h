// Winscope.h : main header file for the WINSCOPE application
//

#if !defined(AFX_WINSCOPE_H__103E2D15_705C_4557_AF66_15D4B93AAD2E__INCLUDED_)
#define AFX_WINSCOPE_H__103E2D15_705C_4557_AF66_15D4B93AAD2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

// Scope constants

#define SC_MAX_BUFER_SIZE	131071
#define WS_POS_EDGE			0x01
#define WS_NEG_EDGE			0x00







/////////////////////////////////////////////////////////////////////////////
// CWinscopeApp:
// See Winscope.cpp for the implementation of this class
//

class CWinscopeApp : public CWinApp
{
public:
	CWinscopeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinscopeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CWinscopeApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINSCOPE_H__103E2D15_705C_4557_AF66_15D4B93AAD2E__INCLUDED_)

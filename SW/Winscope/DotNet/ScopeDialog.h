#if !defined(AFX_SCOPEDIALOG_H__0C46D17C_286D_46F1_A0CD_63ED0665FAFA__INCLUDED_)
#define AFX_SCOPEDIALOG_H__0C46D17C_286D_46F1_A0CD_63ED0665FAFA__INCLUDED_

#include "SamplingDlg.h"	// Added by ClassView
#include "FormatDlg.h"	// Added by ClassView
#include "TriggerDlg.h"	// Added by ClassView
#include "PodLabel.h"
//#include "WsWaveDoc.h"
#include "ScopeThread.h"
#include "WsSetting.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScopeDialog.h : header file
//

class CWsWaveDoc;

/////////////////////////////////////////////////////////////////////////////
// CScopeDialog

class CScopeDialog : public CPropertySheet
{
	DECLARE_DYNAMIC(CScopeDialog)

// Construction
public:
	CScopeDialog(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CScopeDialog(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CWsWaveDoc*	m_doc;


// Operations
public:
	void		RegisterSymbol(CPodLabel* symbol);
	BOOL		IsSymbolSelectedQ(CPodLabel* symbol);
//	CPodLabel*	GetSymbolPointer(CString name);
	void		RegisterAllSymbols( void );
	void		KillingSymbol(CPodLabel* symbol);
//	void		SetScopeOwner(CWsWaveDoc* doc);
	void		OnSettingChanged(char ignoreAorBTrigg);
	void		UpdateSettingsView( void );
//	void		SetSettings(WS_SETTING* setting, CTypedPtrList<CObList, CPodLabel*>*	m_labelDataList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScopeDialog)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	CTriggerDlg		m_triggerDlg;
	CFormatDlg		m_formatDlg;
	CSamplingDlg	m_samplingDlg;
	virtual ~CScopeDialog();

	// Generated message map functions
protected:
	//{{AFX_MSG(CScopeDialog)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCOPEDIALOG_H__0C46D17C_286D_46F1_A0CD_63ED0665FAFA__INCLUDED_)

#if !defined(AFX_FORMATDLG_H__9F57DF71_EAEF_43DE_A180_AC3111E52503__INCLUDED_)
#define AFX_FORMATDLG_H__9F57DF71_EAEF_43DE_A180_AC3111E52503__INCLUDED_

#include <afxtempl.h>
//#include "ScopeDialog.h"
#include "PodLabel.h"

class CScopeDialog;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormatDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CFormatDlg dialog

class CFormatDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CFormatDlg)

// Construction
public:
	CFormatDlg();
	~CFormatDlg();

//	CTypedPtrList<CObList, CPodLabel*>*	m_labelDataList;

	CScopeDialog*	m_scopeDialog;

// Dialog Data
	//{{AFX_DATA(CFormatDlg)
	enum { IDD = IDD_FORMAT };
	CButton	m_removeLabelButt;
	CButton	m_modifyLabelButt;
	CButton	m_addLabelButt;
	CListCtrl	m_labelList;
	CString	m_labelName;
	BOOL	m_podPActive;
	BOOL	m_podQActive;
	BOOL	m_prbAActive;
	BOOL	m_prbBActive;
	BOOL	m_lp0;
	BOOL	m_lp1;
	BOOL	m_lp2;
	BOOL	m_lp3;
	BOOL	m_lp4;
	BOOL	m_lp5;
	BOOL	m_lp6;
	BOOL	m_lp7;
	BOOL	m_lq0;
	BOOL	m_lq1;
	BOOL	m_lq2;
	BOOL	m_lq3;
	BOOL	m_lq4;
	BOOL	m_lq5;
	BOOL	m_lq6;
	BOOL	m_lq7;
	int		m_viewRadix;
	//}}AFX_DATA

	BOOL	m_initialized;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFormatDlg)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void		UpdateSettingsView( void );
	
protected:

	CFont	m_font;


	// Generated message map functions
	//{{AFX_MSG(CFormatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickLabelList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddLabel();
	afx_msg void OnModifyLabel();
	afx_msg void OnRemoveLabel();
	afx_msg void OnChangeLabelName();
	afx_msg void OnPrbAActive();
	afx_msg void OnPrbBActive();
	afx_msg void OnPodPActive();
	afx_msg void OnPodQActive();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMATDLG_H__9F57DF71_EAEF_43DE_A180_AC3111E52503__INCLUDED_)

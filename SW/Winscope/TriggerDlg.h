#if !defined(AFX_TRIGGERDLG_H__7D66FE93_8422_449C_B05F_BB0AC0837504__INCLUDED_)
#define AFX_TRIGGERDLG_H__7D66FE93_8422_449C_B05F_BB0AC0837504__INCLUDED_

//#include "ScopeDialog.h"
#include "PodLabel.h"

class CScopeDialog;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TriggerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTriggerDlg dialog

class CTriggerDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CTriggerDlg)

// Construction
public:
	CTriggerDlg();
	~CTriggerDlg();
	void			CreateTriggPatternString( void );
	BOOL			CreateTriggPatternValues( void );
	CString			BinSprint(int v);
	void			BinSscanf(const CString str, int* v);
	void			UpdateVoltTriggLevel( void );
	void			UpdateTriggLevel(char probe, int newTriggLevel);
	void			UpdateSettingsView( void );
	
	


	CScopeDialog*	m_scopeDialog;

	CString			m_ptrnSymbolName;
	CString			m_edgeSymbolName;
	double			m_reqTrgLvlVA;
	double			m_reqTrgLvlVB;


// Dialog Data
	//{{AFX_DATA(CTriggerDlg)
	enum { IDD = IDD_TRIGGER };
	CEdit	m_triggLevelBVolt;
	CEdit	m_triggLevelAVolt;
	CEdit	m_triggPtrnEdit;
	CComboBox	m_edgeSymbolComboBox;
	CComboBox	m_ptrnSymbolComboBox;
	CScrollBar	m_scrollbarB;
	CScrollBar	m_scrollbarA;
	int		m_triggerFunction;
	int		m_prbTriggSource;
	int		m_prbTriggSlope;
	int		m_ptrnRadix;
	int		m_symbolEdge;
	CString	m_triggPtrn;
	int		m_extTriggSlope;
	int		m_triggLevelB;
	int		m_triggLevelA;
	//}}AFX_DATA

	BOOL	m_initialized;

	
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTriggerDlg)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTriggerDlg)
	afx_msg void OnSelchangePtrnSymbol();
	afx_msg void OnChangeTriggPtrn();
	afx_msg void OnKillfocusTriggPtrn();
	afx_msg void OnSelchangePtrnRadix();
	afx_msg void OnSelchangeEdgeSymbol();
	afx_msg void OnChangePrbTrgLvlA();
	afx_msg void OnChangePrbTrgLvlB();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeTriggFunction();
	afx_msg void OnKillfocusPrbTrgLvlA();
	afx_msg void OnKillfocusPrbTrgLvlB();
	afx_msg void OnPrbTriggSrc();
	afx_msg void OnPrbTriggSrcB();
	afx_msg void OnPrbTriggSlope();
	afx_msg void OnPrbTriggSlopeNeg();
	afx_msg void OnExtPrbTriggSlope();
	afx_msg void OnExtPrbTriggSlopeNeg();
	afx_msg void OnSymbolEdge();
	afx_msg void OnSymbolEdgeNeg();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRIGGERDLG_H__7D66FE93_8422_449C_B05F_BB0AC0837504__INCLUDED_)

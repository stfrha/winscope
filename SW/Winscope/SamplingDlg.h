#if !defined(AFX_SAMPLINGDLG_H__E5829084_6F2D_4595_BE69_8B4C572BB673__INCLUDED_)
#define AFX_SAMPLINGDLG_H__E5829084_6F2D_4595_BE69_8B4C572BB673__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SamplingDlg.h : header file
//

//#include "ScopeDialog.h"

class CScopeDialog;

/////////////////////////////////////////////////////////////////////////////
// CSamplingDlg dialog

class CSamplingDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CSamplingDlg)

// Construction
public:
	CSamplingDlg();
	~CSamplingDlg();

	CScopeDialog*	m_scopeDialog;

// Dialog Data
	//{{AFX_DATA(CSamplingDlg)
	enum { IDD = IDD_SAMPLING };
	CSliderCtrl	m_bufferSizeSlide;
	CSliderCtrl	m_triggDelaySlide;
	int		m_aInput;
	int		m_aVperDiv;
	int		m_bInput;
	int		m_bVperDiv;
	UINT	m_bufSize;
	int		m_clockSelection;
	int		m_tPerDiv;
	UINT	m_triggDelay;
	int		m_sweepMode;
	//}}AFX_DATA

	BOOL	m_initialized;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSamplingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void		UpdateSettingsView( void );

	
protected:

	// Generated message map functions
	//{{AFX_MSG(CSamplingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnReleasedcaptureBufSizeSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusBufferSize();
	afx_msg void OnReleasedcaptureTrigDelySlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusTriggerDelay();
	afx_msg void OnSelchangeAVperdiv();
	afx_msg void OnSelchangeBVperdiv();
	afx_msg void OnSelchangeTperdiv();
	afx_msg void OnAInput();
	afx_msg void OnAIAc();
	afx_msg void OnAIGnd();
	afx_msg void OnBInput();
	afx_msg void OnBIAc();
	afx_msg void OnBIGnd();
	afx_msg void OnClockSelection();
	afx_msg void OnClockSelExt();
	afx_msg void OnSmNormal();
	afx_msg void OnSmAuto();
	afx_msg void OnSweepMode();
	afx_msg void OnNonSweepSettings();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMPLINGDLG_H__E5829084_6F2D_4595_BE69_8B4C572BB673__INCLUDED_)

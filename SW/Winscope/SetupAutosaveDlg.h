#pragma once
#include "afxwin.h"


// CSetupAutosaveDlg dialog

class CSetupAutosaveDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupAutosaveDlg)

public:
	CSetupAutosaveDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupAutosaveDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_AUTOSAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_autosaveActive;
	CString m_filenameBase;
	BOOL m_autosaveExport;
	afx_msg void OnBnClickedAutosaveActive();
	CButton m_autoexportButton;
	virtual BOOL OnInitDialog();
	int m_nextIndex;
};

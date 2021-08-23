// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__1E36A7A5_0BFC_4533_BFFE_91AD6CF0D852__INCLUDED_)
#define AFX_MAINFRM_H__1E36A7A5_0BFC_4533_BFFE_91AD6CF0D852__INCLUDED_

#include "TestDlg.h"
#include "ScopeDialog.h"	// Added by ClassView
#include "WsSetting.h"
#include "ScopeThread.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FPGA_CONFIG_FILE_NAME	m_hwConfigFile

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	int					m_postAbortRerunThis;
	void*				m_postAbortParameter;
	CString				m_hwConfigFile;

	CWsSetting			m_scopeSettings;

	unsigned char		m_cpuVersionString[64];
	WS_ID_VER			m_idVersions;

	WS_STATE			m_scopeState;
	CString				m_debugString;

	unsigned char*		m_prbAData;
	unsigned char*		m_prbBData;
	unsigned char*		m_podPData;
	unsigned char*		m_podQData;
	WS_COMM_PACKET		m_commPacket;
	WS_COMM_PACKET*		m_pCommPacket;

	CWsWaveDoc*			m_scopeOwner;

	BOOL				m_threadAborted;

	int					m_mouseX;
	int					m_mouseY;

// Operations
public:
	LRESULT				OnScopeSaidHello(WPARAM wParam, LPARAM lParam);
	LRESULT				OnScopeError(WPARAM wParam, LPARAM lParam);
	LRESULT				OnScopeFwIdVersions(WPARAM wParam, LPARAM lParam);
	LRESULT				OnScopeDone(WPARAM wParam, LPARAM lParam);
	LRESULT				OnThreadTerminated(WPARAM wParam, LPARAM lParam);
	LRESULT				OnHwNotConfigured(WPARAM wParam, LPARAM lParam);
	void				ConfigureHw( void ) { OnHwConfigurehardware(); }
	void				ClaimScope(CWsWaveDoc* doc);
	void				ReleaseScope(CWsWaveDoc* doc);
	void				ScopeSettingChanged(CWsSetting* scopeSetting);
	LRESULT				OnThreadAborted(WPARAM wParam, LPARAM lParam);
	afx_msg void		OnUpdateScopeStatePane(CCmdUI *pCmdUI);
	afx_msg void		OnUpdateScopeDebugStatePane(CCmdUI *pCmdUI);
	afx_msg void		OnUpdateDebugMousePoint(CCmdUI *pCmdUI);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHwConfigurehardware();
	afx_msg void OnHwGetfwversions();
	afx_msg void OnHwStartinfiniteloopinthread();
	afx_msg void OnUpdateHwConfigurehardware(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHwGetfwversions(CCmdUI* pCmdUI);
	afx_msg void OnHwSelectconfigurationfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__1E36A7A5_0BFC_4533_BFFE_91AD6CF0D852__INCLUDED_)

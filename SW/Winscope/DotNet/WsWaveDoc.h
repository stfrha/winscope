// WsWaveDoc.h : interface of the CWsWaveDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WSWAVEDOC_H__D8402AF7_54E2_4427_8F9B_3E83765E1CCB__INCLUDED_)
#define AFX_WSWAVEDOC_H__D8402AF7_54E2_4427_8F9B_3E83765E1CCB__INCLUDED_

#include "ScopeThread.h"
#include "WaveData.h"	// Added by ClassView
#include "ScopeDialog.h"
#include "WsSetting.h"

#define	RERUN_NORERUN				0x00
#define	RERUN_SCOPESETTINGCHANGED	0x01
#define	RERUN_SWEEP_ABORTED			0x02

#define BASE_SAMPLE_FREQ			30000000.0

#define CVR_A_PROBE					0x01
#define CVR_B_PROBE					0x02

// Appereance
// Colors COLORREF 0x00bbggrr
#define PROBE_BK_COLOR				0x00800000
#define PROBE_GRID_COLOR			0x00808080
#define PROBE_TRIGG_ARROW_COLOR		0x00ffffff
#define PROBE_A_COLOR				0x00ffff80
#define PROBE_B_COLOR				0x0080ffff
#define LOGIC_COLOR					0x0080ff00
#define LOGIC_LABEL_TEXT_COLOR		0x00ffffff
#define CURSOR_X_COLOR				0x004080ff
#define CURSOR_Y_COLOR				0x00ff00ff
#define CURSOR_FRAME_COLOR			0x00ffc6c6
#define CURSOR_FRAME_CHECK_COLOR	0x000000ff
#define CURSOR_FRM_CK_DEPR_COLOR	0x000000c0

// Fonts
#define	LOGIC_LABEL_FONT_NAME		"Arial"
#define LOGIC_LABEL_FONT_SIZE		120	
#define TRIGG_LEVEL_FONT_NAME		"Arial"
#define TRIGG_LEVEL_FONT_SIZE		80
#define CURSOR_FONT_NAME			"Arial"
#define CURSOR_FONT_SIZE			100

// Zoom factor
#define	DELTA_ZOOM_FACTOR			1.3

//Setting Modifiers
#define	SM_NONE						0x00
#define SM_SCOPE_DIALOG				0x01
#define SM_PROBE_INFO_VIEW			0x02
#define	SM_LOGIC_LABEL_VIEW			0x03
#define SM_PROBE_VIEW				0x04
#define SM_LOGIC_VIEW				0x05

#define AUTO_TRIGGER_WAIT_DELAY		500		// 500 ms

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWsWaveDoc : public CDocument
{
protected: // create from serialization only
	CWsWaveDoc();
	DECLARE_DYNCREATE(CWsWaveDoc)

// Attributes
public:
	BOOL				m_threadAborted;
	BOOL				m_sweepActive;
	BOOL				m_rerunSweep;
	int					m_postAbortRerunThis;
	void*				m_postAbortParameter;
	unsigned char*		m_probeA;
	unsigned char*		m_probeB;
	unsigned char*		m_podP;
	unsigned char*		m_podQ;
	BOOL				m_drawWithDots;
	BOOL				m_noErase;
	BOOL				m_grid;
	double				m_viewScaleFactor;

	BOOL				m_validData;
	WS_COMM_PACKET*		m_pCommPacket;
	CWsSetting			m_docSetting;
	CTypedPtrList<CObList, CPodLabel*>	m_symbolList;
	CPodLabel*			m_ptrnSelSymbol;
	CPodLabel*			m_edgeSelSymbol;
	CScopeDialog*		m_scopeDialog;
	int					m_probeAPosOffset;
	int					m_probeBPosOffset;

	double				m_xCursorU;
	double				m_xCursorT;
	double				m_yCursorU;
	double				m_yCursorT;
	BOOL				m_xCursorOn;
	BOOL				m_yCursorOn;
	int					m_cursorVoltageRelation;

// Operations
public:
	void				SetSTCommPacketPointer(WS_COMM_PACKET*	pCommPacket);
	void				UserChangedSetting(int settingModifier);
	void				ScopeSettingChanged( void );
	LRESULT				OnThreadAborted(WPARAM wParam, LPARAM lParam);
	LRESULT				OnScopeError(WPARAM wParam, LPARAM lParam);
	LRESULT				OnNewDataAvailable(WPARAM wParam, LPARAM lParam);
	LRESULT				OnScopeDone(WPARAM wParam, LPARAM lParam);
	LRESULT				OnThreadTerminated(WPARAM wParam, LPARAM lParam);
	LRESULT				OnHwNotConfigured(WPARAM wParam, LPARAM lParam);
	void				ConnectCommPacketToBuffers( void );
	void				ReallocateBuffers(unsigned long newSize);
	void				OnChangedBufferSize(unsigned long newSize);
	void				PostAbortStopscope( void );
	void				UpdateProbeViews( void );
	void				AutoTriggerTimedOut( void );
	void				ToggleCursor(char cursor);

	int					GetProbeOffset(char probe);
	void				GetFormatedVoltage(double value, CString* voltage, CString* preFix);
	void				GetFormatedTime(double value, CString* time, CString* preFix);
	void				GetFormatedFreq(double value, CString* time, CString* preFix);

	void				MWZoom(BOOL zoomIn, int mouseX );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWsWaveDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
//	CWaveData m_waveData;
	virtual ~CWsWaveDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWsWaveDoc)
	afx_msg void OnScopeClaimscopeownership();
	afx_msg void OnUpdateScopeClaimscopeownership(CCmdUI* pCmdUI);
	afx_msg void OnScopeRunscope();
	afx_msg void OnUpdateScopeRunscope(CCmdUI* pCmdUI);
	afx_msg void OnScopeStopscope();
	afx_msg void OnUpdateScopeStopscope(CCmdUI* pCmdUI);
	afx_msg void OnScopeDrawwithdots();
	afx_msg void OnUpdateScopeDrawwithdots(CCmdUI* pCmdUI);
	afx_msg void OnScopeNoerase();
	afx_msg void OnUpdateScopeNoerase(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnViewFitall();
	afx_msg void OnViewNozoom();
	afx_msg void OnViewCursorx();
	afx_msg void OnUpdateViewCursorx(CCmdUI* pCmdUI);
	afx_msg void OnViewCursory();
	afx_msg void OnUpdateViewCursory(CCmdUI* pCmdUI);
	afx_msg void OnViewVoltageCursorA();
	afx_msg void OnUpdateViewVoltageCursorA(CCmdUI* pCmdUI);
	afx_msg void OnViewVoltageCursorB();
	afx_msg void OnUpdateViewVoltageCursorB(CCmdUI* pCmdUI);
	afx_msg void OnFileExporttocsv();
	afx_msg void OnUpdateFileExporttocsv(CCmdUI* pCmdUI);
	afx_msg void OnFileExporttometafile();
	afx_msg void OnUpdateFileExporttometafile(CCmdUI* pCmdUI);
	afx_msg void OnViewDisplaygrid();
	afx_msg void OnUpdateViewDisplaygrid(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSWAVEDOC_H__D8402AF7_54E2_4427_8F9B_3E83765E1CCB__INCLUDED_)

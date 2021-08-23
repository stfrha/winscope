// WsWaveDoc.cpp : implementation of the CWsWaveDoc class
//

#include "stdafx.h"
#include "Winscope.h"
#include "MainFrm.h"
#include "WsSetting.h"
#include "WsProbeView.h"
#include "WsProbeInfoView.h"
#include "WsWaveDoc.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWsWaveDoc

IMPLEMENT_DYNCREATE(CWsWaveDoc, CDocument)

BEGIN_MESSAGE_MAP(CWsWaveDoc, CDocument)
	//{{AFX_MSG_MAP(CWsWaveDoc)
	ON_COMMAND(ID_SCOPE_CLAIMSCOPEOWNERSHIP, OnScopeClaimscopeownership)
	ON_UPDATE_COMMAND_UI(ID_SCOPE_CLAIMSCOPEOWNERSHIP, OnUpdateScopeClaimscopeownership)
	ON_COMMAND(ID_SCOPE_RUNSCOPE, OnScopeRunscope)
	ON_UPDATE_COMMAND_UI(ID_SCOPE_RUNSCOPE, OnUpdateScopeRunscope)
	ON_COMMAND(ID_SCOPE_STOPSCOPE, OnScopeStopscope)
	ON_UPDATE_COMMAND_UI(ID_SCOPE_STOPSCOPE, OnUpdateScopeStopscope)
	ON_COMMAND(ID_SCOPE_DRAWWITHDOTS, OnScopeDrawwithdots)
	ON_UPDATE_COMMAND_UI(ID_SCOPE_DRAWWITHDOTS, OnUpdateScopeDrawwithdots)
	ON_COMMAND(ID_SCOPE_NOERASE, OnScopeNoerase)
	ON_UPDATE_COMMAND_UI(ID_SCOPE_NOERASE, OnUpdateScopeNoerase)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_COMMAND(ID_VIEW_FITALL, OnViewFitall)
	ON_COMMAND(ID_VIEW_NOZOOM, OnViewNozoom)
	ON_COMMAND(ID_VIEW_CURSORX, OnViewCursorx)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CURSORX, OnUpdateViewCursorx)
	ON_COMMAND(ID_VIEW_CURSORY, OnViewCursory)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CURSORY, OnUpdateViewCursory)
	ON_COMMAND(ID_VIEW_VOLTAGECURSORA, OnViewVoltageCursorA)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VOLTAGECURSORA, OnUpdateViewVoltageCursorA)
	ON_COMMAND(ID_VIEW_VOLTAGECURSORB, OnViewVoltageCursorB)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VOLTAGECURSORB, OnUpdateViewVoltageCursorB)
	ON_COMMAND(ID_FILE_EXPORTTOCSV, OnFileExporttocsv)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTTOCSV, OnUpdateFileExporttocsv)
	ON_COMMAND(ID_FILE_EXPORTTOMETAFILE, OnFileExporttometafile)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTTOMETAFILE, OnUpdateFileExporttometafile)
	ON_COMMAND(ID_VIEW_DISPLAYGRID, OnViewDisplaygrid)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DISPLAYGRID, OnUpdateViewDisplaygrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWsWaveDoc construction/destruction

CWsWaveDoc::CWsWaveDoc()
{
	// TODO: add one-time construction code here
	m_pCommPacket = NULL;
	m_docSetting.m_symbolList = &m_symbolList;
	m_validData = FALSE;
	m_ptrnSelSymbol = NULL;
	m_edgeSelSymbol = NULL;
	m_scopeDialog = NULL;
	m_sweepActive = FALSE;
	m_rerunSweep = FALSE;
	m_threadAborted = FALSE;
	m_probeA = NULL;
	m_probeB = NULL;
	m_podP = NULL;
	m_podQ = NULL;
	m_drawWithDots = FALSE;
	m_noErase = FALSE;
	m_grid = TRUE;
	m_viewScaleFactor = 1.0;
	m_probeAPosOffset = 0;
	m_probeBPosOffset = 0;
	m_xCursorU = 0.952;
	m_xCursorT = 0.0;
	m_yCursorU = 0.0;
	m_yCursorT = 0.0;
	m_xCursorOn = FALSE;
	m_yCursorOn = FALSE;
	m_cursorVoltageRelation = CVR_A_PROBE;
}

CWsWaveDoc::~CWsWaveDoc()
{
}

BOOL CWsWaveDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// Allocate memory for probe and pod data
	ReallocateBuffers(m_docSetting.m_bufferSize);

	m_scopeDialog = new CScopeDialog("Winscope Control Window", AfxGetMainWnd());
	m_scopeDialog->m_doc = this;

	m_pCommPacket = NULL;

	return TRUE;
}

BOOL CWsWaveDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	m_scopeDialog = new CScopeDialog("Winscope Control Window", AfxGetMainWnd());
	m_scopeDialog->m_doc = this;

	m_pCommPacket = NULL;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWsWaveDoc serialization

void CWsWaveDoc::Serialize(CArchive& ar)
{
	unsigned long	i;

	m_symbolList.Serialize(ar);
	m_docSetting.Serialize(ar);

	if (ar.IsStoring())	{

		ar << m_xCursorOn;
		ar << m_xCursorT;
		ar << m_xCursorU;

		ar << m_yCursorOn;
		ar << m_yCursorT;
		ar << m_yCursorU;

		ar << m_cursorVoltageRelation;

		if (m_ptrnSelSymbol) {
			ar << m_ptrnSelSymbol->m_name;
		} else {
			ar << CString("");
		}
		if (m_edgeSelSymbol) {
			ar << m_edgeSelSymbol->m_name;
		} else {
			ar << CString("");
		}
		ar << m_validData;
		if (m_probeA && m_probeB && m_podP && m_podQ) {
			for (i=0 ; i < m_docSetting.m_bufferSize ; i++) {
				ar << m_probeA[i];
				ar << m_probeB[i];
				ar << m_podP[i];
				ar << m_podQ[i];
			}
		}
	} else {
		// Allocate memory for probe and pod data
		ReallocateBuffers(m_docSetting.m_bufferSize);

		ar >> m_xCursorOn;
		ar >> m_xCursorT;
		ar >> m_xCursorU;

		ar >> m_yCursorOn;
		ar >> m_yCursorT;
		ar >> m_yCursorU;

		ar >> m_cursorVoltageRelation;

		CString	name;
		ar >> name;
		m_ptrnSelSymbol = m_docSetting.GetSymbol(name);
		ar >> name;
		m_edgeSelSymbol = m_docSetting.GetSymbol(name);
		ar >> m_validData;
		if (m_probeA && m_probeB && m_podP && m_podQ) {
			for (i=0 ; i < m_docSetting.m_bufferSize ; i++) {
				ar >> m_probeA[i];
				ar >> m_probeB[i];
				ar >> m_podP[i];
				ar >> m_podQ[i];
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CWsWaveDoc diagnostics

#ifdef _DEBUG
void CWsWaveDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWsWaveDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWsWaveDoc commands

void CWsWaveDoc::OnScopeClaimscopeownership() 
{
	CMainFrame*	mf = (CMainFrame*) AfxGetMainWnd();

	if (m_pCommPacket == NULL) {

		mf->ClaimScope(this);

		if (m_pCommPacket) {						// Check if I now own scope
			if (!m_validData) {
				// Get Default data
				m_validData = TRUE;
			}
			m_pCommPacket->settings = &m_docSetting;
			POSITION pos = this->GetFirstViewPosition();
			m_pCommPacket->msgReceiver = this->GetNextView(pos);

			ConnectCommPacketToBuffers();

			m_scopeDialog->Create((CWnd*) mf, WS_SYSMENU | WS_POPUP | WS_CAPTION | WS_VISIBLE, WS_EX_TOOLWINDOW );
			if (m_pCommPacket->threadBusy) {			// Is thread busy?
				return;
			}
			*m_pCommPacket->debugString = "Hello";
			m_pCommPacket->commandType = WSC_NO_COMMAND;
			AfxBeginThread(scopeThreadCallback, m_pCommPacket);
			ScopeSettingChanged();
		}
	} else {
		m_pCommPacket->commandType = WSC_TERMINATE_THREAD;
		mf->ReleaseScope(this);
		m_scopeDialog->DestroyWindow();
	}
}

void CWsWaveDoc::OnUpdateScopeClaimscopeownership(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_pCommPacket != NULL);

	CMainFrame*	mf = (CMainFrame*) AfxGetMainWnd();

/*
	Hämta villkor för att något annat dokument fått ownership

	pCmdUI->Enable(mf->m_scopeDialog->m_scopeOwner == NULL || m_pCommPacket != NULL);

*/
	
}

void	CWsWaveDoc::SetSTCommPacketPointer(WS_COMM_PACKET*	pCommPacket)
{
	m_pCommPacket = pCommPacket;
}

void	CWsWaveDoc::UserChangedSetting(int settingModifier)
{
	SetModifiedFlag(TRUE);
	ScopeSettingChanged();
	UpdateAllViews(NULL);						// Should test settingModifier and select only apropriate updates
	if (settingModifier != SM_SCOPE_DIALOG) {
		if (m_scopeDialog) {
			m_scopeDialog->UpdateSettingsView();
		}
	}
}

void	CWsWaveDoc::ScopeSettingChanged( void )
{
	if (m_pCommPacket == NULL) return;			// Does MainFrame own scope now??
	if (m_pCommPacket->threadBusy) {			// Is thread busy?
//		MessageBox(NULL, "Setting changed and thread busy", "Hello", MB_OK);
		if (m_sweepActive) m_rerunSweep = TRUE;
		m_threadAborted = FALSE;
		m_postAbortRerunThis = RERUN_SCOPESETTINGCHANGED;
		m_pCommPacket->loopAbort = TRUE;		// Yes, order thread to die and wait for temination
		return;
	}
//	MessageBox(NULL, "Reattempting to write setting", "Hello", MB_OK);
	m_pCommPacket->commandType = WSC_WRITE_SETTING;
}

LRESULT	CWsWaveDoc::OnThreadAborted(WPARAM wParam, LPARAM lParam)
{
//	MessageBox(NULL, "Thread aborted", "Hello", MB_OK);

	m_threadAborted = TRUE;
	m_pCommPacket->loopAbort = FALSE;
	switch (m_postAbortRerunThis) {
	case RERUN_NORERUN:
		// Do absolutely nothing
		break;
	case RERUN_SCOPESETTINGCHANGED:
		ScopeSettingChanged();
		break;
	case RERUN_SWEEP_ABORTED:
		PostAbortStopscope();
		break;
	};
	return 0;
}

LRESULT CWsWaveDoc::OnScopeDone(WPARAM wParam, LPARAM lParam)
{
	if (m_rerunSweep) {
		m_rerunSweep = FALSE;
		OnScopeRunscope();
	}
	return 0;
}

LRESULT CWsWaveDoc::OnScopeError(WPARAM wParam, LPARAM lParam)
{
	MessageBox(NULL, (char*) lParam, "Winscope Error (in doc)", MB_OK | MB_ICONEXCLAMATION);
	m_pCommPacket->commandType = WSC_TERMINATE_THREAD;
	return 0;
}

LRESULT	CWsWaveDoc::OnNewDataAvailable(WPARAM wParam, LPARAM lParam)
{
	// UpdateView to new data
	UpdateAllViews(NULL);
	m_pCommPacket->dataValid = FALSE;
	SetModifiedFlag(TRUE);
	switch (m_docSetting.m_control.bits.SWEEP_MODE) {
	case TC_SM_NORMAL:
		break;
	case TC_SM_AUTO:
		break;
	case TC_SM_SINGLE:
		m_sweepActive = FALSE;
		break;
	};
	return 0;
}

LRESULT	CWsWaveDoc::OnHwNotConfigured(WPARAM wParam, LPARAM lParam)
{
	m_pCommPacket->commandType = WSC_TERMINATE_THREAD;
	MessageBox(NULL, "HW is not configured\nPlease configure now (Hw->Configure HW)", "Winscope Error", MB_OK | MB_ICONEXCLAMATION);
	return 0;
}

LRESULT	CWsWaveDoc::OnThreadTerminated(WPARAM wParam, LPARAM lParam)
{
//	MessageBox(NULL, "Thread terminated", "Info", MB_OK);
	return 0;
}

void CWsWaveDoc::OnScopeRunscope() 
{
//	if (m_sweepActive) return;		// A sweep is already performing
//		MessageBox(NULL, "Starting a sweep", "Hello", MB_OK);
	switch (m_docSetting.m_control.bits.SWEEP_MODE) {
	case TC_SM_NORMAL:
		if (m_pCommPacket == NULL) return;			// Does MainFrame own scope now??
		if (m_pCommPacket->threadBusy) {			// Is thread busy?
			MessageBox(NULL, "Scope busy", "Error", MB_OK);
			return;
		}
		m_sweepActive = TRUE;
		m_pCommPacket->commandType = WSC_SWEEP_NORMAL;
		break;
	case TC_SM_AUTO:
		if (m_pCommPacket == NULL) return;			// Does MainFrame own scope now??
		if (m_pCommPacket->threadBusy) {			// Is thread busy?
			MessageBox(NULL, "Scope busy", "Error", MB_OK);
			return;
		}
		m_sweepActive = TRUE;
		m_pCommPacket->commandType = WSC_SWEEP_AUTO;
		break;
	case TC_SM_SINGLE:
		if (m_pCommPacket == NULL) return;			// Does MainFrame own scope now??
		if (m_pCommPacket->threadBusy) {			// Is thread busy?
			MessageBox(NULL, "Scope busy", "Error", MB_OK);
			return;
		}
		m_sweepActive = TRUE;

		m_pCommPacket->commandType = WSC_SWEEP_SINGLE;
		break;
	};
}

void CWsWaveDoc::OnUpdateScopeRunscope(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_pCommPacket != NULL) && (!m_sweepActive));
}

void CWsWaveDoc::OnScopeStopscope( void ) 
{
	m_threadAborted = FALSE;
	m_postAbortRerunThis = RERUN_SWEEP_ABORTED;
	m_pCommPacket->loopAbort = TRUE;
}

void CWsWaveDoc::PostAbortStopscope( void ) 
{
//	Sleep(100);
	m_sweepActive = FALSE;
//	MessageBox(NULL, "Sweep aborted", "HEllo", MB_OK);
/*
	while (m_pCommPacket->threadBusy);
	m_pCommPacket->commandType = WSC_RESET_SWEEP;
*/
}


void CWsWaveDoc::OnUpdateScopeStopscope(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_pCommPacket != NULL) && m_sweepActive);
}

void	CWsWaveDoc::ReallocateBuffers(unsigned long newSize)
{
	unsigned long	i;

	m_docSetting.m_bufferSize = newSize;

	if (m_probeA) free(m_probeA);
	m_probeA = (unsigned char*) malloc(newSize);
	if (m_probeB) free(m_probeB);
	m_probeB = (unsigned char*) malloc(newSize);
	if (m_podP) free(m_podP);
	m_podP = (unsigned char*) malloc(newSize);
	if (m_podQ) free(m_podQ);
	m_podQ = (unsigned char*) malloc(newSize);
	if (!m_probeA || !m_probeB || !m_podP || !m_podQ) {
		MessageBox(NULL, "Out of memory", "Error", MB_OK | MB_ICONEXCLAMATION);
	}

	for (i=0 ; i < newSize ; i++) {
		m_probeA[i] = 128;
		m_probeB[i] = 128;
		m_podP[i] = 0;
		m_podQ[i] = 0;
	}

}

void	CWsWaveDoc::OnChangedBufferSize(unsigned long newSize)
{
	ReallocateBuffers(newSize);

	ConnectCommPacketToBuffers();

	UpdateAllViews(NULL);
	SetModifiedFlag(TRUE);
}

void	CWsWaveDoc::ConnectCommPacketToBuffers( void )
{
	if (m_pCommPacket) {
		m_pCommPacket->prbAData = m_probeA;
		m_pCommPacket->prbBData = m_probeB;
		m_pCommPacket->podPData = m_podP;
		m_pCommPacket->podQData = m_podQ;
	}
}


void CWsWaveDoc::DeleteContents() 
{
	CMainFrame*	mf = (CMainFrame*) AfxGetMainWnd();

	if (m_probeA) free(m_probeA);
	m_probeA = NULL;
	if (m_probeB) free(m_probeB);
	m_probeB = NULL;
	if (m_podP) free(m_podP);
	m_podP = NULL;
	if (m_podQ) free(m_podQ);
	m_podQ = NULL;
	
	CDocument::DeleteContents();
}

void CWsWaveDoc::OnCloseDocument() 
{

	CMainFrame*	mf = (CMainFrame*) AfxGetMainWnd();

	if (m_probeA) free(m_probeA);
	m_probeA = NULL;
	if (m_probeB) free(m_probeB);
	m_probeB = NULL;
	if (m_podP) free(m_podP);
	m_podP = NULL;
	if (m_podQ) free(m_podQ);
	m_podQ = NULL;

	if (m_pCommPacket) {						// Check if I now own scope
		m_pCommPacket->commandType = WSC_TERMINATE_THREAD;
		Sleep(100);
		mf->ReleaseScope(this);
	}

	if (m_scopeDialog) {
		m_scopeDialog->DestroyWindow();
		delete m_scopeDialog;
	}

	CDocument::OnCloseDocument();
}

void CWsWaveDoc::OnScopeDrawwithdots() 
{
	if (m_drawWithDots) {
		m_drawWithDots = FALSE;
		UpdateAllViews(NULL);
	} else {
		m_drawWithDots = TRUE;
		UpdateAllViews(NULL);
	}
}

void CWsWaveDoc::OnUpdateScopeDrawwithdots(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_drawWithDots);
}

void CWsWaveDoc::OnScopeNoerase() 
{
	if (m_noErase) {
		m_noErase = FALSE;
	} else {
		m_noErase = TRUE;
	}
}

void CWsWaveDoc::OnUpdateScopeNoerase(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_noErase);
}

void CWsWaveDoc::OnViewZoomin() 
{
	CView*				tmpView;
	int					centerX;

	POSITION pos = GetFirstViewPosition();
	while (pos) {
		tmpView = GetNextView(pos);
		if (tmpView->IsKindOf(RUNTIME_CLASS(CWsProbeView))) {
			centerX = ((CWsProbeView*) tmpView)->GetViewportCenter();
		}
	}

	MWZoom(TRUE, centerX);
}

void CWsWaveDoc::OnViewZoomout() 
{
	CView*				tmpView;
	int					centerX;

	POSITION pos = GetFirstViewPosition();
	while (pos) {
		tmpView = GetNextView(pos);
		if (tmpView->IsKindOf(RUNTIME_CLASS(CWsProbeView))) {
			centerX = ((CWsProbeView*) tmpView)->GetViewportCenter();
		}
	}

	MWZoom(FALSE, centerX);
}

void CWsWaveDoc::OnViewFitall() 
{
	CView*		tmpView;
	int			cx;

	POSITION pos = GetFirstViewPosition();
	while (pos) {
		tmpView = GetNextView(pos);
		if (tmpView->IsKindOf(RUNTIME_CLASS(CWsWaveDataView))) {
			cx = ((CWsWaveDataView*) tmpView)->GetViewportSize();
		}
	}
	m_viewScaleFactor = (double) cx / (double) m_docSetting.m_bufferSize;
	UpdateAllViews(NULL);

}

void CWsWaveDoc::OnViewNozoom() 
{
	m_viewScaleFactor = 1.0;
	UpdateAllViews(NULL);
}

void	CWsWaveDoc::MWZoom(BOOL zoomIn, int mouseX )
{
	CView*				tmpView;
	double				oldScaleFactor = m_viewScaleFactor;

	POSITION pos = GetFirstViewPosition();
	while (pos) {
		tmpView = GetNextView(pos);
		if (tmpView->IsKindOf(RUNTIME_CLASS(CWsWaveDataView))) {
			((CWsWaveDataView*) tmpView)->PreScrollPrepare();
		}
	}

	if (zoomIn) {
		m_viewScaleFactor *= DELTA_ZOOM_FACTOR;
	} else {
		m_viewScaleFactor /= DELTA_ZOOM_FACTOR;
	}
	UpdateAllViews(NULL);

	pos = GetFirstViewPosition();
	while (pos) {
		tmpView = GetNextView(pos);
		if (tmpView->IsKindOf(RUNTIME_CLASS(CWsWaveDataView))) {
			((CWsWaveDataView*) tmpView)->PostScrollPosition(mouseX, oldScaleFactor);
		}
	}
}

void	CWsWaveDoc::UpdateProbeViews( void )
{
	CView*				tmpView;

	POSITION pos = GetFirstViewPosition();
	while (pos) {
		tmpView = GetNextView(pos);
		if ((tmpView->IsKindOf(RUNTIME_CLASS(CWsProbeView))) || 
			(tmpView->IsKindOf(RUNTIME_CLASS(CWsProbeInfoView)))) {
			tmpView->Invalidate(TRUE);
			tmpView->UpdateWindow();
		}
	}

}

void	CWsWaveDoc::AutoTriggerTimedOut( void )
{
	m_pCommPacket->waitTriggerAbort = TRUE;
}


void	CWsWaveDoc::ToggleCursor(char cursor)
{
	if ('X' == cursor) OnViewCursorx();
	else if ('Y' == cursor) OnViewCursory();
	else {
		MessageBox(NULL, "Illegal parameter value to ToggleCursor", "Error", MB_ICONEXCLAMATION | MB_OK);
	}
}

void CWsWaveDoc::OnViewCursorx() 
{
	if (m_xCursorOn) m_xCursorOn = FALSE;
	else m_xCursorOn = TRUE;
	UpdateAllViews(NULL);
}

void CWsWaveDoc::OnUpdateViewCursorx(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_xCursorOn);
}

void CWsWaveDoc::OnViewCursory() 
{
	if (m_yCursorOn) m_yCursorOn = FALSE;
	else m_yCursorOn = TRUE;
	UpdateAllViews(NULL);
}

void CWsWaveDoc::OnUpdateViewCursory(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_yCursorOn);
}

void CWsWaveDoc::GetFormatedVoltage(double value, CString* voltage, CString* preFix)
{
	if (fabs(value) < 1.0e-3) {
		voltage->Format("%.2f", value * 1.0e6);
		*preFix = "uV";
	} else if (fabs(value) < 1.0) {
		voltage->Format("%.2f", value * 1000.0);
		*preFix = "mV";
	} else {
		voltage->Format("%.2f", value);
		*preFix = "V";
	}
}

void CWsWaveDoc::GetFormatedTime(double value, CString* time, CString* preFix)
{
	if (value < 1.0e-12) {
		time->Format("%.2f", value * 1.0e15);
		*preFix = "fs";
	} else if (value < 1.0e-9) {
		time->Format("%.2f", value * 1.0e12);
		*preFix = "ps";
	} else if (value < 1.0e-6) {
		time->Format("%.2f", value * 1.0e9);
		*preFix = "ns";
	} else if (value < 1.0e-3) {
		time->Format("%.2f", value * 1.0e6);
		*preFix = "us";
	} else if (value < 1.0) {
		time->Format("%.2f", value * 1.0e3);
		*preFix = "ms";
	} else {
		time->Format("%.2f", value);
		*preFix = "s";
	}
}


void CWsWaveDoc::GetFormatedFreq(double value, CString* time, CString* preFix)
{
	if (value < 1.0e-3) {
		time->Format("%.2f", value * 1.0e6);
		*preFix = "uHz";
	} else if (value < 1.0) {
		time->Format("%.2f", value * 1000.0);
		*preFix = "mHz";
	} else if (value < 1000.0) {
		time->Format("%.2f", value);
		*preFix = "Hz";
	} else if (value < 1.0e6) {
		time->Format("%.2f", value / 1000.0);
		*preFix = "kHz";
	} else if (value < 1.0e9) {
		time->Format("%.2f", value / 1.0e6);
		*preFix = "MHz";
	} else if (value < 1.0e12) {
		time->Format("%.2f", value / 1.0e9);
		*preFix = "GHz";
	}
}

int	CWsWaveDoc::GetProbeOffset(char probe)
{
	if ('A' == probe) {
		return m_probeAPosOffset;
	} else {
		return m_probeBPosOffset;
	}
}


void CWsWaveDoc::OnViewVoltageCursorA() 
{
	if (CVR_A_PROBE != m_cursorVoltageRelation) {
		m_cursorVoltageRelation = CVR_A_PROBE;
		SetModifiedFlag(TRUE);
		UpdateAllViews(NULL);
	}
}

void CWsWaveDoc::OnUpdateViewVoltageCursorA(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(CVR_A_PROBE == m_cursorVoltageRelation);
	pCmdUI->Enable(m_xCursorOn || m_yCursorOn);
}

void CWsWaveDoc::OnViewVoltageCursorB() 
{
	if (CVR_B_PROBE != m_cursorVoltageRelation) {
		m_cursorVoltageRelation = CVR_B_PROBE;
		SetModifiedFlag(TRUE);
		UpdateAllViews(NULL);
	}
}

void CWsWaveDoc::OnUpdateViewVoltageCursorB(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(CVR_B_PROBE == m_cursorVoltageRelation);
	pCmdUI->Enable(m_xCursorOn || m_yCursorOn);
}

void CWsWaveDoc::OnFileExporttocsv() 
{
	FILE*			of;
	unsigned long	i;
	CFileDialog		fd(FALSE, ".csv", GetTitle().Left(GetTitle().GetLength()-5));

	if (fd.DoModal() == IDOK) {
		of = fopen(fd.GetPathName(), "wt");
		if (!of) {
			MessageBox(NULL, "Unable to open file for output", "Error opening file", MB_OK | MB_ICONEXCLAMATION);
			return;
		}
		for (i=0 ; i<m_docSetting.m_bufferSize ; i++) {
			fprintf(of, "%03d\t%03d\n", *(m_probeA + i), *(m_probeB + i));
		}
		fclose(of);
	}
}

void CWsWaveDoc::OnUpdateFileExporttocsv(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_sweepActive);
}

void CWsWaveDoc::OnFileExporttometafile() 
{
	CView*				tmpView;
	CWsProbeView*		probeView = NULL;

	CFileDialog			fd(FALSE, ".emf", GetTitle(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Enhanced Windows Meta File (*.emf)|*.emf||");

	if (fd.DoModal() == IDOK) {
		POSITION pos = GetFirstViewPosition();
		while (pos) {
			tmpView = GetNextView(pos);
			if (tmpView->IsKindOf(RUNTIME_CLASS(CWsProbeView))) {
				probeView = (CWsProbeView*) tmpView;
			}
		}

		if (!probeView) {
			MessageBox(NULL, "Could not get probe view pointer", "Error", MB_OK);
			return;
		}

		CMetaFileDC	metaFile;
		metaFile.CreateEnhanced(NULL, NULL, NULL, "Winscope\0" + GetTitle().Left(GetTitle().GetLength()-5) + "\0\0");

		probeView->OnDraw(&metaFile);

		HENHMETAFILE mfHandle = metaFile.CloseEnhanced();

		CopyEnhMetaFile(mfHandle, fd.GetPathName());

		DeleteEnhMetaFile(mfHandle);
	}

}

void CWsWaveDoc::OnUpdateFileExporttometafile(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_sweepActive);
}

void CWsWaveDoc::OnViewDisplaygrid() 
{
	if (m_grid) m_grid = FALSE;
	else m_grid = TRUE;
	UpdateAllViews(NULL);
}

void CWsWaveDoc::OnUpdateViewDisplaygrid(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_grid);
}

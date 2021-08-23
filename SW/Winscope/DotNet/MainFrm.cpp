// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Winscope.h"
#include "ScopeThread.h"
#include "WsWaveDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_HW_CONFIGUREHARDWARE, OnHwConfigurehardware)
	ON_COMMAND(ID_HW_GETFWVERSIONS, OnHwGetfwversions)
	ON_COMMAND(ID_HW_STARTINFINITELOOPINTHREAD, OnHwStartinfiniteloopinthread)
	ON_UPDATE_COMMAND_UI(ID_HW_CONFIGUREHARDWARE, OnUpdateHwConfigurehardware)
	ON_UPDATE_COMMAND_UI(ID_HW_GETFWVERSIONS, OnUpdateHwGetfwversions)
	ON_COMMAND(ID_HW_SELECTCONFIGURATIONFILE, OnHwSelectconfigurationfile)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_SAY_HELLO, OnScopeSaidHello)
	ON_MESSAGE(WM_USER_ERROR, OnScopeError)
	ON_MESSAGE(WM_USER_FW_VERSIONS, OnScopeFwIdVersions)
	ON_MESSAGE(WM_USER_THREAD_ABORTED, OnThreadAborted)
	ON_MESSAGE(WM_USER_COMMAND_COMPLETE, OnScopeDone)
	ON_MESSAGE(WM_USER_THREAD_TERMINATED, OnThreadTerminated)
	ON_MESSAGE(WM_USER_HW_NOT_CONFIGURED, OnHwNotConfigured)
	ON_UPDATE_COMMAND_UI(ID_SCOPE_STATE, OnUpdateScopeStatePane)
	ON_UPDATE_COMMAND_UI(ID_SCOPE_DEBUG_STATE, OnUpdateScopeDebugStatePane)
	ON_UPDATE_COMMAND_UI(ID_DG_MOUSE_POINT, OnUpdateDebugMousePoint)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_DG_MOUSE_POINT,
	ID_SCOPE_DEBUG_STATE,
	ID_SCOPE_STATE,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_hwConfigFile = "D:\\Våra dokument\\Fredrik\\WinScope\\VHDL\\MAIN_1_6.bit";

	m_commPacket.commandType = WSC_CONFIG;
	m_commPacket.threadBusy = FALSE;
	m_commPacket.loopAbort = FALSE;
	m_commPacket.updatingBuffers = FALSE;
	m_commPacket.msgReceiver = this;
	m_commPacket.settings = &m_scopeSettings;
	m_commPacket.state = &m_scopeState;
	m_commPacket.dataValid = NULL;
	m_commPacket.prbAData = NULL;
	m_commPacket.prbBData = NULL;
	m_commPacket.podPData = NULL;
	m_commPacket.podQData = NULL;
	m_commPacket.dmaBuffer = NULL;
	m_commPacket.debugString = &m_debugString;

	m_commPacket.configurationFile = FPGA_CONFIG_FILE_NAME;
	m_idVersions.cpuVersionString = m_cpuVersionString;
	m_commPacket.fwIdVersions = &m_idVersions;
	m_scopeOwner = NULL;
	m_pCommPacket = &m_commPacket;
}

CMainFrame::~CMainFrame()
{
	if (NULL != m_commPacket.dmaBuffer) {
		free(m_commPacket.dmaBuffer);
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	
	m_commPacket.dmaBuffer = (unsigned char*) malloc(256*1024*4);
	if (NULL == m_commPacket.dmaBuffer) {
		MessageBox("Could not allocate dma buffer", "Error", MB_OK | MB_ICONEXCLAMATION);
	}


	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}



/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnHwConfigurehardware() 
{
	if (m_pCommPacket == NULL) return;
	if (m_pCommPacket->threadBusy) {
		MessageBox("Thread already busy", "Hello");
		return;
	}
	m_pCommPacket->commandType = WSC_CONFIG;
	m_pCommPacket->msgReceiver = this;
	m_pCommPacket->configurationFile = FPGA_CONFIG_FILE_NAME;
	MessageBox(m_pCommPacket->configurationFile, "Configuration file");
	AfxBeginThread(scopeThreadCallback, m_pCommPacket);
}

void CMainFrame::OnUpdateHwConfigurehardware(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pCommPacket != NULL);
}

LRESULT CMainFrame::OnScopeSaidHello(WPARAM wParam, LPARAM lParam)
{
	MessageBox("Winscope thread said hello", "Hello", MB_ICONINFORMATION | MB_OK);
	return 0;
}

LRESULT CMainFrame::OnScopeError(WPARAM wParam, LPARAM lParam)
{
	m_pCommPacket->commandType = WSC_TERMINATE_THREAD;
	MessageBox((char*) lParam, "Winscope Error", MB_OK | MB_ICONEXCLAMATION);
	return 0;
}

LRESULT	CMainFrame::OnHwNotConfigured(WPARAM wParam, LPARAM lParam)
{
	m_pCommPacket->commandType = WSC_TERMINATE_THREAD;
	MessageBox("HW is not configured\nPlease configure now (Hw->Configure HW)", "Winscope Error", MB_OK | MB_ICONEXCLAMATION);
	return 0;
}

void CMainFrame::OnHwGetfwversions() 
{
	if (m_pCommPacket == NULL) return;
	if (m_pCommPacket->threadBusy) {
		MessageBox("Thread already busy", "Hello");
		return;
	}
	m_pCommPacket->fwIdVersions->cpuVersionString = m_cpuVersionString;
	m_pCommPacket->commandType = WSC_FW_VERSIONS;
	AfxBeginThread(scopeThreadCallback, m_pCommPacket);
}

LRESULT	CMainFrame::OnScopeFwIdVersions(WPARAM wParam, LPARAM lParam)
{
	CString	msg;

	msg.Format("%s\nFPGA ID: %X\nFPGA Version: %d.%d", 
		m_pCommPacket->fwIdVersions->cpuVersionString, 
		m_pCommPacket->fwIdVersions->fpgaId, 
		(m_pCommPacket->fwIdVersions->fpgaVersion >> 4) & 0x0F, 
		m_pCommPacket->fwIdVersions->fpgaVersion & 0x0F);

	MessageBox(msg, "FW Id and versions", MB_ICONINFORMATION | MB_OK);
	return 0;
}

LRESULT CMainFrame::OnScopeDone(WPARAM wParam, LPARAM lParam)
{
	m_pCommPacket->commandType = WSC_TERMINATE_THREAD;
	MessageBox("Command done", "Hello");
	return 0;
}

void CMainFrame::OnUpdateHwGetfwversions(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pCommPacket != NULL);
}

void	CMainFrame::ClaimScope(CWsWaveDoc* doc) 
{
	if (m_scopeOwner != NULL) return ;			// Cant give scope away until owner releases it
	m_pCommPacket = NULL;						// Disables comm channel from main frame
	m_scopeOwner = doc;
	doc->SetSTCommPacketPointer(&m_commPacket);	// Should it be & ?????
}

void	CMainFrame::ReleaseScope(CWsWaveDoc* doc) 
{
	if (doc != m_scopeOwner) return;			// doc does not own scope
	doc->SetSTCommPacketPointer(NULL);			// Disables comm channel from doc
	m_scopeOwner = NULL;
	m_pCommPacket = &m_commPacket;				// Give scope back to main frame
	m_commPacket.loopAbort = FALSE;
	m_commPacket.msgReceiver = this;
	m_commPacket.settings = &m_scopeSettings;
	m_commPacket.state = &m_scopeState;
	m_commPacket.dataValid = NULL;
	m_commPacket.prbAData = NULL;
	m_commPacket.prbBData = NULL;
	m_commPacket.podPData = NULL;
	m_commPacket.podQData = NULL;
}

void	CMainFrame::ScopeSettingChanged(CWsSetting* scopeSetting)
{
	MessageBox("This command is illegal without a attched document", "Error");
/*
	if (m_pCommPacket == NULL) return;			// Does MainFrame own scope now??
	if (m_pCommPacket->threadBusy) {			// Is thread busy?
		m_threadAborted = FALSE;
		m_postAbortRerunThis = RERUN_SCOPESETTINGCHANGED;
		m_postAbortParameter = (void*) scopeSetting;
		m_pCommPacket->loopAbort = TRUE;		// Yes, order thread to die and wait for temination
		return;
	}
	m_pCommPacket->commandType = WSC_WRITE_SETTING;
	m_pCommPacket->settings = scopeSetting;

	AfxBeginThread(scopeThreadCallback, m_pCommPacket);
*/
}

LRESULT	CMainFrame::OnThreadAborted(WPARAM wParam, LPARAM lParam)
{
	m_pCommPacket->commandType = WSC_TERMINATE_THREAD;
	return 0;
}

LRESULT	CMainFrame::OnThreadTerminated(WPARAM wParam, LPARAM lParam)
{
//	MessageBox("Thread terminated", "Info", MB_OK);
	return 0;
}

void CMainFrame::OnHwStartinfiniteloopinthread() 
{
	MessageBox("This command is not supported any more", "Error");
/*
	if (m_pCommPacket == NULL) return;
	if (m_pCommPacket->threadBusy) {
		MessageBox("Thread already busy", "Hello");
		return;
	}
	m_pCommPacket->commandType = WSC_INFINITE_LOOP;
	AfxBeginThread(scopeThreadCallback, m_pCommPacket);
*/
}


void CMainFrame::OnUpdateScopeStatePane(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(); 
    CString strPage;
	if (NULL != m_pCommPacket) {
		strPage = "Document does not own scope";
	} else {
		switch( m_commPacket.state->status.bits.SWEEP_STATE ) {
		case S_SS_IDLE :
			strPage = "Ready";
			break;
		case S_SS_PRE_FILL :
			strPage = "Pre trigger buffer fill...";
			break;
		case S_SS_ARMED :
			strPage = "Waiting for trigger...";
			break;
		case S_SS_TRIGGED :
			strPage = "Trigged";
			break;
		case S_SS_FULL :
			strPage = "Full";
			break;
		default:
			strPage = "Illegal state";
			break;

		}
	}
    pCmdUI->SetText( strPage ); 
}

void CMainFrame::OnUpdateScopeDebugStatePane(CCmdUI *pCmdUI)
{
	pCmdUI->SetText(*(m_commPacket.debugString));
}

void CMainFrame::OnUpdateDebugMousePoint(CCmdUI *pCmdUI)
{
	CString msg;
	
	msg.Format("x: %d, y: %d", m_mouseX, m_mouseY);
	pCmdUI->SetText(msg);
}

void CMainFrame::OnHwSelectconfigurationfile() 
{
	CFileDialog			fd(TRUE, ".bit", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, "XILINX Configuration file (*.bit)|*.bit||");

	if (fd.DoModal() == IDOK) {
		m_hwConfigFile = fd.GetPathName();
	}
}

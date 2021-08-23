// WsProbeView.cpp : implementation of the CWsProbeView class
//

#include "stdafx.h"
#include "Winscope.h"

#include "MainFrm.h"
#include "WsWaveDoc.h"
#include "WsProbeInfoView.h"
#include "WsProbeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWsProbeView

IMPLEMENT_DYNCREATE(CWsProbeView, CWsWaveDataView)

BEGIN_MESSAGE_MAP(CWsProbeView, CWsWaveDataView)
	//{{AFX_MSG_MAP(CWsProbeView)
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_MESSAGE(WM_USER_ERROR, OnScopeError)
	ON_MESSAGE(WM_USER_THREAD_ABORTED, OnThreadAborted)
	ON_MESSAGE(WM_USER_NEW_DATA_AVAILABLE, OnNewDataAvailable)
	ON_MESSAGE(WM_USER_COMMAND_COMPLETE, OnScopeDone)
	ON_MESSAGE(WM_USER_THREAD_TERMINATED, OnThreadTerminated)
	ON_MESSAGE(WM_USER_HW_NOT_CONFIGURED, OnHwNotConfigured)
	ON_MESSAGE(WM_USER_SWEEP_STATE_ARMED, OnSweepStateArmed)
	ON_MESSAGE(WM_USER_SWEEP_STATE_FULL, OnSweepStateFull)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWsProbeView construction/destruction

CWsProbeView::CWsProbeView()
{
	// TODO: add construction code here
	m_probeInfoView = NULL;
	m_x = 0;
	m_dragState = DS_NONE;
}

CWsProbeView::~CWsProbeView()
{
}

BOOL CWsProbeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWsProbeView drawing

void CWsProbeView::OnDraw(CDC* pDC)
{
	unsigned long	i;
	int				offset;

	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

// Grid -------------------------------------------

	CPen aPen;
	aPen.CreatePen(PS_SOLID, 1, COLORREF(PROBE_GRID_COLOR));
	CPen* oldPen = pDC->SelectObject(&aPen);
	
	if ((!pDoc->m_noErase) && (pDoc->m_grid)) {

		for (i=0 ; i < pDoc->m_docSetting.m_bufferSize ; i += 30) {
			pDC->MoveTo((long) (i * pDoc->m_viewScaleFactor), 0);
			pDC->LineTo((long) (i * pDoc->m_viewScaleFactor), 255);
		}

		for (i=0 ; i < 256+1 ; i += 32) {
			pDC->MoveTo(0, i);
			pDC->LineTo((long) (pDoc->m_docSetting.m_bufferSize * pDoc->m_viewScaleFactor), i);
		}
	}

// Cursors --------------------------------------------
	char	cvr;

	if (CVR_A_PROBE == pDoc->m_cursorVoltageRelation) cvr = 'A';
	else cvr = 'B';

// Cursor Y -------------------------------------------

	if (pDoc->m_yCursorOn) {
		CPen	curYPen;
		curYPen.CreatePen(PS_SOLID, 1, COLORREF(CURSOR_Y_COLOR));
		pDC->SelectObject(&curYPen);
		pDC->MoveTo((long) (pDoc->m_yCursorT * pDoc->m_viewScaleFactor), 0);
		pDC->LineTo((long) (pDoc->m_yCursorT * pDoc->m_viewScaleFactor), 256);
		pDC->MoveTo(0, (long) (128.0 - pDoc->m_yCursorU / pDoc->m_docSetting.GetScreenVoltageRange(cvr) * 128.0) + pDoc->GetProbeOffset(cvr));
		pDC->LineTo((long) ((double) pDoc->m_docSetting.m_bufferSize * pDoc->m_viewScaleFactor), 
					(long) (128.0 - pDoc->m_yCursorU / pDoc->m_docSetting.GetScreenVoltageRange(cvr) * 128.0) + pDoc->GetProbeOffset(cvr));
	}

// Cursor X -------------------------------------------

	if (pDoc->m_xCursorOn) {
		CPen	curXPen;
		curXPen.CreatePen(PS_SOLID, 1, COLORREF(CURSOR_X_COLOR));
		pDC->SelectObject(&curXPen);
		pDC->MoveTo((long) (pDoc->m_xCursorT * pDoc->m_viewScaleFactor), 0);
		pDC->LineTo((long) (pDoc->m_xCursorT * pDoc->m_viewScaleFactor), 256);
		pDC->MoveTo(0, (long) (128.0 - pDoc->m_xCursorU / pDoc->m_docSetting.GetScreenVoltageRange(cvr) * 128.0) + pDoc->GetProbeOffset(cvr));
		pDC->LineTo((long) ((double) pDoc->m_docSetting.m_bufferSize * pDoc->m_viewScaleFactor), 
					(long) (128.0 - pDoc->m_xCursorU / pDoc->m_docSetting.GetScreenVoltageRange(cvr) * 128.0) + pDoc->GetProbeOffset(cvr));
	}

// Probe B trace -------------------------------------------

	CPen	yPen;
	yPen.CreatePen(PS_SOLID, 1, COLORREF(PROBE_B_COLOR));
	pDC->SelectObject(&yPen);

	if ((pDoc->m_probeB) && (pDoc->m_docSetting.m_prbPodCntrl.bits.PRBB_ACTIVE)) {

		offset = 256 + pDoc->m_probeBPosOffset;

		if (!pDoc->m_drawWithDots) pDC->MoveTo(0, offset - pDoc->m_probeB[0]);
		for (i=0 ; i<pDoc->m_docSetting.m_bufferSize ; i++) {
			if (!pDoc->m_drawWithDots) {
				pDC->LineTo((long) (i * pDoc->m_viewScaleFactor), offset - pDoc->m_probeB[i]);
			} else {
				pDC->SetPixel((long) (i * pDoc->m_viewScaleFactor), offset - pDoc->m_probeB[i], PROBE_B_COLOR);
			}
		}
	}

// Probe A trace -------------------------------------------

	CPen	bPen;
	bPen.CreatePen(PS_SOLID, 1, COLORREF(PROBE_A_COLOR));
	pDC->SelectObject(&bPen);

	if ((pDoc->m_probeA) && (pDoc->m_docSetting.m_prbPodCntrl.bits.PRBA_ACTIVE)) {

		offset = 256 + pDoc->m_probeAPosOffset;

		if (!pDoc->m_drawWithDots) pDC->MoveTo(0, offset - pDoc->m_probeA[0]);
		for (i=0 ; i<pDoc->m_docSetting.m_bufferSize ; i++) {
			if (!pDoc->m_drawWithDots) {
				pDC->LineTo((long) (i * pDoc->m_viewScaleFactor), offset-pDoc->m_probeA[i]);
			} else {
				pDC->SetPixel((long) (i * pDoc->m_viewScaleFactor), offset-pDoc->m_probeA[i], PROBE_A_COLOR);
			}
		}
	}

// Trigger time mark -------------------------------------------

	CPen	wPen;
	wPen.CreatePen(PS_SOLID, 1, COLORREF(PROBE_TRIGG_ARROW_COLOR));
	pDC->SelectObject(&wPen);
	pDC->MoveTo((long) (pDoc->m_docSetting.m_triggDelay * pDoc->m_viewScaleFactor), 10);
	pDC->LineTo((long) (pDoc->m_docSetting.m_triggDelay * pDoc->m_viewScaleFactor + 5), 0);
	pDC->LineTo((long) (pDoc->m_docSetting.m_triggDelay * pDoc->m_viewScaleFactor - 5), 0);
	pDC->LineTo((long) (pDoc->m_docSetting.m_triggDelay * pDoc->m_viewScaleFactor), 10);


	pDC->SelectObject(oldPen);
}


void CWsProbeView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	OnChangedSize();
}

void	CWsProbeView::OnChangedSize( void )
{
	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CSize docSize = CSize(pDoc->m_docSetting.m_bufferSize, 256);
	docSize.cx = (long) (docSize.cx * pDoc->m_viewScaleFactor);

	SetScrollSizes(MM_TEXT, docSize, CSize((long) (10 * pDoc->m_viewScaleFactor), 100), CSize((long) (pDoc->m_viewScaleFactor), 100));
}

/////////////////////////////////////////////////////////////////////////////
// CWsProbeView printing

BOOL CWsProbeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWsProbeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWsProbeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CWsProbeView diagnostics

#ifdef _DEBUG
void CWsProbeView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CWsProbeView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

/////////////////////////////////////////////////////////////////////////////
// CWsProbeView message handlers


CWsWaveDoc* CWsProbeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWsWaveDoc)));
	return (CWsWaveDoc*)m_pDocument;
}
#endif //_DEBUG

BOOL CWsProbeView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	if (m_probeInfoView) {
		m_probeInfoView->SetProbeScrollPosition(GetScrollPosition());
	}
	
	return CScrollView::OnScrollBy(sizeScroll, bDoScroll);
}

LRESULT	CWsProbeView::OnThreadAborted(WPARAM wParam, LPARAM lParam)
{
	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	return pDoc->OnThreadAborted(wParam, lParam);
}

LRESULT CWsProbeView::OnScopeError(WPARAM wParam, LPARAM lParam)
{
	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	return pDoc->OnScopeError(wParam, lParam);
}

LRESULT CWsProbeView::OnScopeDone(WPARAM wParam, LPARAM lParam)
{
	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	return pDoc->OnScopeDone(wParam, lParam);
}


void CWsProbeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CWsWaveDoc* pDoc = GetDocument();
	OnChangedSize();
	Invalidate(!pDoc->m_noErase);
}

LRESULT	CWsProbeView::OnNewDataAvailable(WPARAM wParam, LPARAM lParam)
{
	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	return pDoc->OnNewDataAvailable(wParam, lParam);
}

LRESULT	CWsProbeView::OnThreadTerminated(WPARAM wParam, LPARAM lParam)
{
	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	return pDoc->OnThreadTerminated(wParam, lParam);
}

LRESULT	CWsProbeView::OnHwNotConfigured(WPARAM wParam, LPARAM lParam)
{
	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	return pDoc->OnHwNotConfigured(wParam, lParam);
}

LRESULT	CWsProbeView::OnSweepStateArmed(WPARAM wParam, LPARAM lParam)
{
	m_timer = SetTimer(1, AUTO_TRIGGER_WAIT_DELAY, NULL);
	return 0;
}

LRESULT	CWsProbeView::OnSweepStateFull(WPARAM wParam, LPARAM lParam)
{
	KillTimer(m_timer);
	return 0;
}

BOOL CWsProbeView::OnEraseBkgnd(CDC* pDC) 
{
    // Set brush to desired background color.
    CBrush backBrush(PROBE_BK_COLOR);
    // Save old brush.
    CBrush* pOldBrush = pDC->SelectObject(&backBrush);
    CRect rect;
    pDC->GetClipBox(&rect);     // Erase the area needed.
    pDC->PatBlt(rect.left, rect.top, rect.Width(), 
    rect.Height(), PATCOPY);
    pDC->SelectObject(pOldBrush);
    return TRUE;
}

void CWsProbeView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{

	if (nSBCode == SB_THUMBTRACK) {
//		MessageBeep(0xFFFFFFFF);
		SCROLLINFO lpScrollInfo;
		CScrollView::GetScrollInfo(SB_HORZ, &lpScrollInfo);
		nPos = lpScrollInfo.nTrackPos;
		TRACE("nPos = %d, nTrackPos %d\n", 
                        lpScrollInfo.nPos, 
                        lpScrollInfo.nTrackPos);
	}

	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CWsProbeView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	RECT			wr;

	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	GetWindowRect(&wr);

	pDoc->MWZoom((zDelta > 0), pt.x - wr.left);

	return 0;
}


int	CWsProbeView::GetViewportCenter( void )
{
	int				cx;
	int				dummy;

	m_splitterWnd->GetColumnInfo(1, cx, dummy);

	return GetScrollPosition().x + cx / 2;
}

void CWsProbeView::OnTimer(UINT nIDEvent) 
{
	KillTimer(m_timer);

	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->AutoTriggerTimedOut();
	
	CWsWaveDataView::OnTimer(nIDEvent);
}

void CWsProbeView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int		curLogPos;
	int		distance;
	char	cvr;

	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (CVR_A_PROBE == pDoc->m_cursorVoltageRelation) cvr = 'A';
	else cvr = 'B';

	point += GetScrollPosition();

	if (pDoc->m_xCursorOn) {
		// Cursor X Time
		curLogPos = (long) (pDoc->m_xCursorT * pDoc->m_viewScaleFactor);
		distance = abs(curLogPos - point.x);
		if (distance < PV_CURSOR_GRAB_DISTANCE) {
			m_dragState = DS_DRAG_X_TIME_CURSOR;
			SetCapture();
		}

		// Cursor X Voltage
		curLogPos = (long) (128.0 - pDoc->m_xCursorU / pDoc->m_docSetting.GetScreenVoltageRange(cvr) * 128.0) + pDoc->GetProbeOffset(cvr);
		distance = abs(curLogPos - point.y);
		if (distance < PV_CURSOR_GRAB_DISTANCE) {
			m_dragState |= DS_DRAG_X_VOLT_CURSOR;
		}
	}

	if (m_dragState) return;	// If previous hit, no more checking is needed.

	if (pDoc->m_yCursorOn) {
		// Cursor Y Time
		curLogPos = (long) (pDoc->m_yCursorT * pDoc->m_viewScaleFactor);
		distance = abs(curLogPos - point.x);
		if (distance < PV_CURSOR_GRAB_DISTANCE) {
			m_dragState = DS_DRAG_Y_TIME_CURSOR;
			SetCapture();
		}

		// Cursor Y Voltage
		curLogPos = (long) (128.0 - pDoc->m_yCursorU / pDoc->m_docSetting.GetScreenVoltageRange(cvr) * 128.0) + pDoc->GetProbeOffset(cvr);
		distance = abs(curLogPos - point.y);
		if (distance < PV_CURSOR_GRAB_DISTANCE) {
			m_dragState |= DS_DRAG_Y_VOLT_CURSOR;
		}
	}
}


void CWsProbeView::OnMouseMove(UINT nFlags, CPoint point) 
{
	char	cvr;

	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	point += GetScrollPosition();

	if (CVR_A_PROBE == pDoc->m_cursorVoltageRelation) cvr = 'A';
	else cvr = 'B';

	// Cursor X Time
	if (m_dragState & DS_DRAG_X_TIME_CURSOR) {
		pDoc->m_xCursorT = point.x / pDoc->m_viewScaleFactor;
		if (pDoc->m_xCursorT < 0.0) pDoc->m_xCursorT = 0.0;
		if (pDoc->m_xCursorT > pDoc->m_docSetting.m_bufferSize) pDoc->m_xCursorT = pDoc->m_docSetting.m_bufferSize;
	}
	// Cursor X Voltage
	if (m_dragState & DS_DRAG_X_VOLT_CURSOR) {
		pDoc->m_xCursorU = pDoc->m_docSetting.GetScreenVoltageRange(cvr) / 128.0 * (pDoc->GetProbeOffset(cvr) + 128.0 - point.y);
		if (pDoc->m_xCursorU < -pDoc->m_docSetting.GetScreenVoltageRange(cvr)) pDoc->m_xCursorU = -pDoc->m_docSetting.GetScreenVoltageRange(cvr);
		if (pDoc->m_xCursorU > pDoc->m_docSetting.GetScreenVoltageRange(cvr)) pDoc->m_xCursorU = pDoc->m_docSetting.GetScreenVoltageRange(cvr);
	}

	// Cursor Y Time
	if (m_dragState & DS_DRAG_Y_TIME_CURSOR) {
		pDoc->m_yCursorT = point.x / pDoc->m_viewScaleFactor;
		if (pDoc->m_yCursorT < 0.0) pDoc->m_yCursorT = 0.0;
		if (pDoc->m_yCursorT > pDoc->m_docSetting.m_bufferSize) pDoc->m_yCursorT = pDoc->m_docSetting.m_bufferSize;
	}
	// Cursor Y Voltage
	if (m_dragState & DS_DRAG_Y_VOLT_CURSOR) {
		pDoc->m_yCursorU = pDoc->m_docSetting.GetScreenVoltageRange(cvr) / 128.0 * (pDoc->GetProbeOffset(cvr) + 128.0 - point.y);
		if (pDoc->m_yCursorU < -pDoc->m_docSetting.GetScreenVoltageRange(cvr)) pDoc->m_yCursorU = -pDoc->m_docSetting.GetScreenVoltageRange(cvr);
		if (pDoc->m_yCursorU > pDoc->m_docSetting.GetScreenVoltageRange(cvr)) pDoc->m_yCursorU = pDoc->m_docSetting.GetScreenVoltageRange(cvr);
	}

	if (m_dragState) pDoc->UpdateAllViews(FALSE);
}

void CWsProbeView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	char	cvr;

	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (CVR_A_PROBE == pDoc->m_cursorVoltageRelation) cvr = 'A';
	else cvr = 'B';

	point += GetScrollPosition();

	// Cursor X Time
	if (m_dragState & DS_DRAG_X_TIME_CURSOR) {
		pDoc->m_xCursorT = point.x / pDoc->m_viewScaleFactor;
		if (pDoc->m_xCursorT < 0.0) pDoc->m_xCursorT = 0.0;
		if (pDoc->m_xCursorT > pDoc->m_docSetting.m_bufferSize) pDoc->m_xCursorT = pDoc->m_docSetting.m_bufferSize;
		pDoc->SetModifiedFlag(TRUE);
	}
	// Cursor X Voltage
	if (m_dragState & DS_DRAG_X_VOLT_CURSOR) {
		pDoc->m_xCursorU = pDoc->m_docSetting.GetScreenVoltageRange(cvr) / 128.0 * (pDoc->GetProbeOffset(cvr) + 128.0 - point.y);
		if (pDoc->m_xCursorU < -pDoc->m_docSetting.GetScreenVoltageRange(cvr)) pDoc->m_xCursorU = -pDoc->m_docSetting.GetScreenVoltageRange(cvr);
		if (pDoc->m_xCursorU > pDoc->m_docSetting.GetScreenVoltageRange(cvr)) pDoc->m_xCursorU = pDoc->m_docSetting.GetScreenVoltageRange(cvr);
		pDoc->SetModifiedFlag(TRUE);
	}

	// Cursor Y Time
	if (m_dragState & DS_DRAG_Y_TIME_CURSOR) {
		pDoc->m_yCursorT = point.x / pDoc->m_viewScaleFactor;
		if (pDoc->m_yCursorT < 0.0) pDoc->m_yCursorT = 0.0;
		if (pDoc->m_yCursorT > pDoc->m_docSetting.m_bufferSize) pDoc->m_yCursorT = pDoc->m_docSetting.m_bufferSize;
		pDoc->SetModifiedFlag(TRUE);
	}
	// Cursor Y Voltage
	if (m_dragState & DS_DRAG_Y_VOLT_CURSOR) {
		pDoc->m_yCursorU = pDoc->m_docSetting.GetScreenVoltageRange(cvr) / 128.0 * (pDoc->GetProbeOffset(cvr) + 128.0 - point.y);
		if (pDoc->m_yCursorU < -pDoc->m_docSetting.GetScreenVoltageRange(cvr)) pDoc->m_yCursorU = -pDoc->m_docSetting.GetScreenVoltageRange(cvr);
		if (pDoc->m_yCursorU > pDoc->m_docSetting.GetScreenVoltageRange(cvr)) pDoc->m_yCursorU = pDoc->m_docSetting.GetScreenVoltageRange(cvr);
		pDoc->SetModifiedFlag(TRUE);
	}

	if (m_dragState) pDoc->UpdateAllViews(FALSE);
	m_dragState = DS_NONE;
	ReleaseCapture();
}

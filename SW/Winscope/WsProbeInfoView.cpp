// WsProbeInfoView.cpp : implementation file
//

#include "stdafx.h"
#include "Winscope.h"
#include "WsProbeInfoView.h"
#include "WsProbeView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWsProbeInfoView

IMPLEMENT_DYNCREATE(CWsProbeInfoView, CView)

CWsProbeInfoView::CWsProbeInfoView()
{
	m_scrollPosition = CPoint(0, 0);
	m_dragState = PIV_DS_IDLE;
	m_cursorXOnBoxDepressed = FALSE;
	m_cursorYOnBoxDepressed = FALSE;
}

CWsProbeInfoView::~CWsProbeInfoView()
{
}


BEGIN_MESSAGE_MAP(CWsProbeInfoView, CView)
	//{{AFX_MSG_MAP(CWsProbeInfoView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWsProbeInfoView drawing

void CWsProbeInfoView::OnDraw(CDC* pDC)
{
	CString	msg;
	CRect	strRect;
	int		tly;
	CSize	strSize;
	int		tcx;						// Trigg rightmost x
	int		pcx;						// Y pos rightmost x


	CWsWaveDoc* pDoc = (CWsWaveDoc*) GetDocument();
	// TODO: add draw code here

	RECT	myRect;
	GetClientRect(&myRect);
	tcx = myRect.right;
	pcx = tcx - 10;

	CFont	font;
	font.CreatePointFont(TRIGG_LEVEL_FONT_SIZE, TRIGG_LEVEL_FONT_NAME);
	CFont*	oldFont = pDC->SelectObject(&font);

	CPen	yPen;
	yPen.CreatePen(PS_SOLID, 1, COLORREF(PROBE_B_COLOR));
	CPen* oldPen = pDC->SelectObject(&yPen);

	pDC->SetTextColor(PROBE_B_COLOR);
	pDC->SetBkColor(PROBE_BK_COLOR);

// Trigg level B
	tly = 256 + pDoc->m_probeBPosOffset - pDoc->m_docSetting.m_prbBTriggLevel - m_scrollPosition.y;
	pDC->MoveTo(tcx, tly);
	pDC->LineTo(tcx-10, tly-5);
	pDC->LineTo(tcx-10, tly+5);
	pDC->LineTo(tcx, tly);

	if (pDoc->m_docSetting.m_triggSetup.bits.PRB_TRG_SOURCE) {
		if (pDoc->m_docSetting.m_triggSetup.bits.PRB_TRG_SLOPE) {
			pDC->MoveTo(tcx - 27, tly + 5);
			pDC->LineTo(tcx - 23, tly + 5);
			pDC->LineTo(tcx - 18, tly - 5);
			pDC->LineTo(tcx - 14, tly - 5);
			pDC->MoveTo(tcx - 24, tly);
			pDC->LineTo(tcx - 19, tly - 2);
			pDC->LineTo(tcx - 18, tly + 3);
		} else {
			pDC->MoveTo(tcx - 27, tly - 5);
			pDC->LineTo(tcx - 23, tly - 5);
			pDC->LineTo(tcx - 18, tly + 5);
			pDC->LineTo(tcx - 14, tly + 5);
			pDC->MoveTo(tcx - 24, tly);
			pDC->LineTo(tcx - 19, tly + 2);
			pDC->LineTo(tcx - 18, tly - 3);
		}
		msg.Format("%.2f", ((double) pDoc->m_docSetting.m_prbBTriggLevel - 128.0) / 128.0 * pDoc->m_docSetting.GetScreenVoltageRange('B'));

		strSize = pDC->GetTextExtent(msg);
		strRect.top = tly - strSize.cy / 2;
		strRect.bottom = tly + strSize.cy / 2;
		strRect.right = tcx - 30;
		strRect.left = tcx - strSize.cx - 30;
		pDC->DrawText(msg, strRect, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
	}

// GND B
	tly = 128 + pDoc->m_probeBPosOffset;
	pDC->MoveTo(pcx, tly);
	pDC->LineTo(pcx - 10, tly-5);
	pDC->LineTo(pcx - 10, tly+5);
	pDC->LineTo(pcx, tly);
	pDC->MoveTo(pcx - 10, tly);
	pDC->LineTo(pcx - 25, tly);


	CPen	bPen;
	bPen.CreatePen(PS_SOLID, 1, COLORREF(PROBE_A_COLOR));
	pDC->SelectObject(&bPen);

	pDC->SetTextColor(PROBE_A_COLOR);
	pDC->SetBkColor(PROBE_BK_COLOR);

// Trigg level A
	tly = 256 + pDoc->m_probeAPosOffset - pDoc->m_docSetting.m_prbATriggLevel - m_scrollPosition.y;
	pDC->MoveTo(tcx, tly);
	pDC->LineTo(tcx - 10, tly-5);
	pDC->LineTo(tcx - 10, tly+5);
	pDC->LineTo(tcx, tly);

	if (!pDoc->m_docSetting.m_triggSetup.bits.PRB_TRG_SOURCE) {
		if (pDoc->m_docSetting.m_triggSetup.bits.PRB_TRG_SLOPE) {
			pDC->MoveTo(tcx - 27, tly + 5);
			pDC->LineTo(tcx - 23, tly + 5);
			pDC->LineTo(tcx - 18, tly - 5);
			pDC->LineTo(tcx - 14, tly - 5);
			pDC->MoveTo(tcx - 24, tly);
			pDC->LineTo(tcx - 19, tly - 2);
			pDC->LineTo(tcx - 18, tly + 3);
		} else {
			pDC->MoveTo(tcx - 27, tly - 5);
			pDC->LineTo(tcx - 23, tly - 5);
			pDC->LineTo(tcx - 18, tly + 5);
			pDC->LineTo(tcx - 14, tly + 5);
			pDC->MoveTo(tcx - 24, tly);
			pDC->LineTo(tcx - 19, tly + 2);
			pDC->LineTo(tcx - 18, tly - 3);
		}
		msg.Format("%.2f", ((double) pDoc->m_docSetting.m_prbATriggLevel - 128.0) / 128.0 * pDoc->m_docSetting.GetScreenVoltageRange('A'));

		strSize = pDC->GetTextExtent(msg);
		strRect.top = tly - strSize.cy / 2;
		strRect.bottom = tly + strSize.cy / 2;
		strRect.right = tcx - 30;
		strRect.left = tcx - strSize.cx - 30;
		pDC->DrawText(msg, strRect, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
	}

// GND A
	tly = 128 + pDoc->m_probeAPosOffset;
	pDC->MoveTo(pcx, tly);
	pDC->LineTo(pcx - 10, tly-5);
	pDC->LineTo(pcx - 10, tly+5);
	pDC->LineTo(pcx, tly);
	pDC->MoveTo(pcx - 10, tly);
	pDC->LineTo(pcx - 25, tly);

// Probe frames:
	CPen	cfPen;
	cfPen.CreatePen(PS_SOLID, 1, COLORREF(CURSOR_FRAME_COLOR));
	pDC->SelectObject(&cfPen);
	CBrush	bkBrush;
	bkBrush.CreateSolidBrush(PROBE_BK_COLOR);
	CBrush* oldBrush = pDC->SelectObject(&bkBrush);

	CFont	cursorFont;
	cursorFont.CreatePointFont(CURSOR_FONT_SIZE, CURSOR_FONT_NAME);
	pDC->SelectObject(&cursorFont);


	int		frameBase = CFS_TOP_MOST_BASE;
	CString	voltage;
	CString	time;
	CString	freq;
	CString	vPrefix;
	CString	tPrefix;
	CString	fPrefix;
	double	xt, yt; 

	pDoc->GetFormatedVoltage(pDoc->m_xCursorU, &voltage, &vPrefix);
	xt = pDoc->m_xCursorT / BASE_SAMPLE_FREQ * ((double) pDoc->m_docSetting.m_clockReset + 1.0);
	pDoc->GetFormatedTime(xt, &time, &tPrefix);

	PaintFrames(pDC, CURSOR_X_COLOR, frameBase, "Cursor X", voltage, vPrefix, time, tPrefix, pDoc->m_xCursorOn, m_cursorXOnBoxDepressed);

	frameBase += CFS_DELTA;

	pDoc->GetFormatedVoltage(pDoc->m_yCursorU, &voltage, &vPrefix);
	yt = pDoc->m_yCursorT / BASE_SAMPLE_FREQ * ((double) pDoc->m_docSetting.m_clockReset + 1.0);
	pDoc->GetFormatedTime(yt, &time, &tPrefix);

	PaintFrames(pDC, CURSOR_Y_COLOR, frameBase, "Cursor Y", voltage, vPrefix, time, tPrefix, pDoc->m_yCursorOn, m_cursorYOnBoxDepressed);

	frameBase += CFS_DELTA;

	pDoc->GetFormatedVoltage(fabs(pDoc->m_xCursorU - pDoc->m_yCursorU), &voltage, &vPrefix);
	pDoc->GetFormatedTime(fabs(xt - yt), &time, &tPrefix);

	PaintFrames(pDC, CURSOR_FRAME_COLOR, frameBase, "Delta", voltage, vPrefix, time, tPrefix, pDoc->m_xCursorOn & pDoc->m_yCursorOn, FALSE);

	frameBase += CFS_DELTA;

	if (pDoc->m_xCursorT == pDoc->m_yCursorT) {
		freq = "-.-";
		fPrefix = "Hz";
	} else {
		pDoc->GetFormatedFreq(1.0 / fabs(xt - yt), &freq, &fPrefix);
	}

	PaintFrames(pDC, CURSOR_FRAME_COLOR, frameBase, "Calc", freq, fPrefix, NULL, NULL, pDoc->m_xCursorOn & pDoc->m_yCursorOn, FALSE);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldFont);
	pDC->SelectObject(oldBrush);
}

void CWsProbeInfoView::PaintFrames(CDC* pDC, COLORREF textColor, int frameBase, 
								   const char* caption, const char* tls, const char* trs, 
								   const char* bls, const char* brs, BOOL on, BOOL depressed)
{
// Requires pens, brushes and fonts to be setup
	CSize	strSize;
	CRect	strRect;
	CBrush	cfBrush;

	cfBrush.CreateSolidBrush(CURSOR_FRAME_COLOR);

	pDC->FrameRect(CRect(CFS_LEFT, frameBase, CFS_LEFT + CFS_WIDTH, frameBase + CFS_HEIGHT), &cfBrush);
	pDC->Rectangle(CRect(CFS_ON_BOX_LEFT, frameBase - CFS_ON_BOX_HEIGHT / 2, CFS_ON_BOX_LEFT + CFS_ON_BOX_WIDTH, frameBase + CFS_ON_BOX_HEIGHT / 2));
	if (caption != NULL) {
		pDC->SetTextColor(textColor);
		strSize = pDC->GetTextExtent(caption);
		strRect.top = frameBase - strSize.cy / 2;
		strRect.bottom = frameBase + strSize.cy / 2;
		strRect.left = CFS_CAPTION_LEFT;
		strRect.right = strSize.cx + CFS_CAPTION_LEFT;
		pDC->DrawText(caption, strRect, DT_RIGHT | DT_SINGLELINE | DT_VCENTER | DT_NOCLIP );
	}
	if ((on) || (depressed)) {
		if (on) {
			if (tls != NULL) {
				strSize = pDC->GetTextExtent(tls);
				strRect.top = frameBase + CFS_TOP_TEXT_Y - strSize.cy / 2;
				strRect.bottom = frameBase + CFS_TOP_TEXT_Y + strSize.cy / 2;
				strRect.left = CFS_TEXT_LEFT_COL;
				strRect.right = strSize.cx + CFS_TEXT_LEFT_COL;
				pDC->DrawText(tls, strRect, DT_RIGHT | DT_SINGLELINE | DT_VCENTER | DT_NOCLIP );
			}
			if (trs != NULL) {
				strSize = pDC->GetTextExtent(trs);
				strRect.top = frameBase + CFS_TOP_TEXT_Y - strSize.cy / 2;
				strRect.bottom = frameBase + CFS_TOP_TEXT_Y + strSize.cy / 2;
				strRect.left = CFS_TEXT_RIGHT_COL;
				strRect.right = strSize.cx + CFS_TEXT_RIGHT_COL;
				pDC->DrawText(trs, strRect, DT_RIGHT | DT_SINGLELINE | DT_VCENTER | DT_NOCLIP );
			}
			if (bls != NULL) {
				strSize = pDC->GetTextExtent(bls);
				strRect.top = frameBase + CFS_BOTT_TEXT_Y - strSize.cy / 2;
				strRect.bottom = frameBase + CFS_BOTT_TEXT_Y + strSize.cy / 2;
				strRect.left = CFS_TEXT_LEFT_COL;
				strRect.right = strSize.cx + CFS_TEXT_LEFT_COL;
				pDC->DrawText(bls, strRect, DT_RIGHT | DT_SINGLELINE | DT_VCENTER | DT_NOCLIP );
			}
			if (brs != NULL) {
				strSize = pDC->GetTextExtent(brs);
				strRect.top = frameBase + CFS_BOTT_TEXT_Y - strSize.cy / 2;
				strRect.bottom = frameBase + CFS_BOTT_TEXT_Y + strSize.cy / 2;
				strRect.left = CFS_TEXT_RIGHT_COL;
				strRect.right = strSize.cx + CFS_TEXT_RIGHT_COL;
				pDC->DrawText(brs, strRect, DT_RIGHT | DT_SINGLELINE | DT_VCENTER | DT_NOCLIP );
			}
		}
		CPen redPen;
		if (depressed) {
			redPen.CreatePen(PS_SOLID, 2, CURSOR_FRM_CK_DEPR_COLOR);
		} else {
			redPen.CreatePen(PS_SOLID, 2, CURSOR_FRAME_CHECK_COLOR);
		}
		CPen* oldPen = pDC->SelectObject(&redPen);
		pDC->MoveTo(CFS_ON_BOX_LEFT + 2, frameBase - 2);
		pDC->LineTo(CFS_ON_BOX_LEFT + 3, frameBase + 2);
		pDC->LineTo(CFS_ON_BOX_LEFT + 7, frameBase - 3);
		pDC->SelectObject(oldPen);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWsProbeInfoView diagnostics

#ifdef _DEBUG
void CWsProbeInfoView::AssertValid() const
{
	CView::AssertValid();
}

void CWsProbeInfoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CWsProbeInfoView message handlers

void CWsProbeInfoView::SetProbeScrollPosition(CPoint p)
{
	m_scrollPosition = p;
	Invalidate(TRUE);
	UpdateWindow();
}


BOOL CWsProbeInfoView::OnEraseBkgnd(CDC* pDC) 
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


void CWsProbeInfoView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int		tcx;						// Trigg rightmost x
	int		pcx;						// Y pos rightmost x

	CWsWaveDoc* pDoc = (CWsWaveDoc*) GetDocument();

	RECT	myRect;
	GetClientRect(&myRect);
	tcx = myRect.right;
	pcx = tcx - 10;

	int	hitTestY = 256 - pDoc->m_docSetting.m_prbATriggLevel + pDoc->m_probeAPosOffset;
	if ((point.y < hitTestY + DMCS_DIFF_Y_CENTER) && (point.y > hitTestY - DMCS_DIFF_Y_CENTER) &&
		(point.x < tcx) && (point.x > tcx - DMCS_DIFF_FROM_RIGHT)) {
		if (pDoc->m_pCommPacket) {
			m_dragState = PIV_DS_DRAG_A_TRIGG;
			pDoc->m_docSetting.m_prbATriggLevel = 256 - (unsigned char) point.y + pDoc->m_probeAPosOffset;
			pDoc->UserChangedSetting(SM_NONE);
			return;
		}
	}

	hitTestY = 256 - pDoc->m_docSetting.m_prbBTriggLevel + pDoc->m_probeBPosOffset;
	if ((point.y < hitTestY + DMCS_DIFF_Y_CENTER) && (point.y > hitTestY - DMCS_DIFF_Y_CENTER) &&
		(point.x < tcx) && (point.x > tcx - DMCS_DIFF_FROM_RIGHT)) {
		if (pDoc->m_pCommPacket) {
			m_dragState = PIV_DS_DRAG_B_TRIGG;
			pDoc->m_docSetting.m_prbBTriggLevel = 256 - (unsigned char) point.y + pDoc->m_probeBPosOffset;
			pDoc->UserChangedSetting(SM_NONE);
			return;
		}
	}
	
	hitTestY = 128 + pDoc->m_probeAPosOffset;
	if ((point.y < hitTestY + DMCS_DIFF_Y_CENTER) && (point.y > hitTestY - DMCS_DIFF_Y_CENTER) &&
		(point.x < pcx) && (point.x > pcx - DMCS_DIFF_FROM_RIGHT)) {
		m_dragState = PIV_DS_DRAG_A_GND;
		pDoc->m_probeAPosOffset = point.y - 128;
		pDoc->UpdateProbeViews();
		SetCapture();
		return;
	}

	hitTestY = 128 + pDoc->m_probeBPosOffset;
	if ((point.y < hitTestY + DMCS_DIFF_Y_CENTER) && (point.y > hitTestY - DMCS_DIFF_Y_CENTER) &&
		(point.x < pcx) && (point.x > pcx - DMCS_DIFF_FROM_RIGHT)) {
		m_dragState = PIV_DS_DRAG_B_GND;
		pDoc->m_probeBPosOffset = point.y - 128;
		pDoc->UpdateProbeViews();
		SetCapture();
		return;
	}

	int		frameBase = CFS_TOP_MOST_BASE;
	CRect	testRect = CRect(CFS_ON_BOX_LEFT, frameBase - CFS_ON_BOX_HEIGHT / 2, 
						CFS_ON_BOX_LEFT + CFS_ON_BOX_WIDTH, frameBase + CFS_ON_BOX_HEIGHT / 2);
	if (testRect.PtInRect(point)) {
		m_cursorXOnBoxDepressed = TRUE;
		pDoc->UpdateProbeViews();
		SetCapture();
		return;
	}

	frameBase += CFS_DELTA;
	testRect = CRect(CFS_ON_BOX_LEFT, frameBase - CFS_ON_BOX_HEIGHT / 2, 
				CFS_ON_BOX_LEFT + CFS_ON_BOX_WIDTH, frameBase + CFS_ON_BOX_HEIGHT / 2);
	if (testRect.PtInRect(point)) {
		m_cursorYOnBoxDepressed = TRUE;
		pDoc->UpdateProbeViews();
		SetCapture();
		return;
	}

	CView::OnLButtonDown(nFlags, point);
}

void CWsProbeInfoView::OnMouseMove(UINT nFlags, CPoint point) 
{
	int	tempLevel;

	CWsWaveDoc* pDoc = (CWsWaveDoc*) GetDocument();

	if (point.y < 0) {
		point.y = 0;
	}
	
	if (point.y > 255) {
		point.y = 255;
	}

	switch (m_dragState) {
	case PIV_DS_IDLE:
		break;
	case PIV_DS_DRAG_A_GND:
		pDoc->m_probeAPosOffset = point.y - 128;
		pDoc->UpdateProbeViews();
		break;
	case PIV_DS_DRAG_B_GND:
		pDoc->m_probeBPosOffset = point.y - 128;
		pDoc->UpdateProbeViews();
		break;
	case PIV_DS_DRAG_A_TRIGG:
		tempLevel = 256 - point.y + pDoc->m_probeAPosOffset;
		if (tempLevel > 255) tempLevel = 255;
		if (tempLevel < 0) tempLevel = 0;
		pDoc->m_docSetting.m_prbATriggLevel = (unsigned char) tempLevel;
		pDoc->UserChangedSetting(SM_NONE);
		break;
	case PIV_DS_DRAG_B_TRIGG:
		tempLevel = 256 - point.y + pDoc->m_probeBPosOffset;
		if (tempLevel > 255) tempLevel = 255;
		if (tempLevel < 0) tempLevel = 0;
		pDoc->m_docSetting.m_prbBTriggLevel = (unsigned char) tempLevel;
		pDoc->UserChangedSetting(SM_NONE);
		break;
	}
		
	CView::OnMouseMove(nFlags, point);
}

void CWsProbeInfoView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int		tempLevel;
	int		frameBase;
	CRect	testRect;

	CWsWaveDoc* pDoc = (CWsWaveDoc*) GetDocument();

	if (point.y < 0) point.y = 0;
	if (point.y > 256) point.y = 255;

	if (m_cursorXOnBoxDepressed) {
		m_cursorXOnBoxDepressed = FALSE;
		frameBase = CFS_TOP_MOST_BASE;
		testRect = CRect(CFS_ON_BOX_LEFT, frameBase - CFS_ON_BOX_HEIGHT / 2, 
						CFS_ON_BOX_LEFT + CFS_ON_BOX_WIDTH, frameBase + CFS_ON_BOX_HEIGHT / 2);
		if (testRect.PtInRect(point)) {
			pDoc->ToggleCursor('X');
		}
		pDoc->UpdateProbeViews();
	} else if (m_cursorYOnBoxDepressed) {
		m_cursorYOnBoxDepressed = FALSE;
		frameBase = CFS_TOP_MOST_BASE + CFS_DELTA;
		testRect = CRect(CFS_ON_BOX_LEFT, frameBase - CFS_ON_BOX_HEIGHT / 2, 
						CFS_ON_BOX_LEFT + CFS_ON_BOX_WIDTH, frameBase + CFS_ON_BOX_HEIGHT / 2);
		if (testRect.PtInRect(point)) {
			pDoc->ToggleCursor('Y');
		}
		pDoc->UpdateProbeViews();
	} else {
		switch (m_dragState) {
		case PIV_DS_IDLE:
			break;
		case PIV_DS_DRAG_A_GND:
			pDoc->m_probeAPosOffset = point.y - 128;
			pDoc->UpdateProbeViews();
			break;
		case PIV_DS_DRAG_B_GND:
			pDoc->m_probeBPosOffset = point.y - 128;
			pDoc->UpdateProbeViews();
			break;
		case PIV_DS_DRAG_A_TRIGG:
			tempLevel = 256 - point.y + pDoc->m_probeAPosOffset;
			if (tempLevel > 255) tempLevel = 255;
			if (tempLevel < 0) tempLevel = 0;
			pDoc->m_docSetting.m_prbATriggLevel = (unsigned char) tempLevel;
			pDoc->UserChangedSetting(SM_NONE);
			break;
		case PIV_DS_DRAG_B_TRIGG:
			tempLevel = 256 - point.y + pDoc->m_probeBPosOffset;
			if (tempLevel > 255) tempLevel = 255;
			if (tempLevel < 0) tempLevel = 0;
			pDoc->m_docSetting.m_prbBTriggLevel = (unsigned char) tempLevel;
			pDoc->UserChangedSetting(SM_NONE);
			break;
		}

		m_dragState = PIV_DS_IDLE;
	}

	ReleaseCapture();
	
	CView::OnLButtonUp(nFlags, point);
}

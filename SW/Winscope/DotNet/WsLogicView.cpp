// WsLogicView.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "Winscope.h"
#include "WsWaveDoc.h"
#include "WsLogicLabelView.h"
#include "WsLogicView.h"
#include "WsProbeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CWsLogicView

IMPLEMENT_DYNCREATE(CWsLogicView, CWsWaveDataView)

CWsLogicView::CWsLogicView()
{
	m_dragState = DS_NONE;
}

CWsLogicView::~CWsLogicView()
{
}


BEGIN_MESSAGE_MAP(CWsLogicView, CWsWaveDataView)
	//{{AFX_MSG_MAP(CWsLogicView)
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWsLogicView drawing

void CWsLogicView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	OnChangedSize();
}

void	CWsLogicView::OnChangedSize( void )
{
	CClientDC	dc(this);

	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CSize size = m_logicLabelView->GetMaxSize(&dc);

	size.cx = (long) (pDoc->m_docSetting.m_bufferSize * pDoc->m_viewScaleFactor);


	SetScrollSizes(MM_TEXT, size, CSize((long) (10 * pDoc->m_viewScaleFactor), 100), CSize((long) (pDoc->m_viewScaleFactor), 100));
}


void CWsLogicView::OnDraw(CDC* pDC)
{
	unsigned long	i = 0;
	CPodLabel*		tmpSymbol;
	int				lastData = -1;				// Data will never happen
	int				dataAtIndex;
	int				lastChangedIndex = -1;		// 
	int				activeRow = 0;
	int				j;
	int				activeBitIndex;
	int				tmpBit;
	int				textHeight;
	long			tmpYU;
	long			tmpYL;
	long			tmpXL;
	long			tmpXR;
	CString			dataRepr;
	CRect			reprRect;
	int				cx;
	int				dummy;
	unsigned long	firstVisIndex;
	unsigned long	lastVisIndex;
	
	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_splitterWnd->GetColumnInfo(1, cx, dummy);

	firstVisIndex = (long) ( (double) GetScrollPosition().x / pDoc->m_viewScaleFactor) + 1;
	lastVisIndex = (long) ( (double) (GetScrollPosition().x  + cx) / pDoc->m_viewScaleFactor);


	textHeight = m_logicLabelView->GetLabelHeight(pDC);

	CFont	font;
	font.CreateFont(textHeight, 
					0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH, LOGIC_LABEL_FONT_NAME);
	CFont*	oldFont = pDC->SelectObject(&font);

	CPen	gPen;
	gPen.CreatePen(PS_SOLID, 1, COLORREF(LOGIC_COLOR));
	CPen*	oldPen = pDC->SelectObject(&gPen);

	pDC->SetTextColor(COLORREF(LOGIC_COLOR));
	pDC->SetBkColor(PROBE_BK_COLOR);

	POSITION pos = pDoc->m_docSetting.m_symbolList->GetHeadPosition();
	while (pos) {
		tmpSymbol = pDoc->m_docSetting.m_symbolList->GetNext(pos);
		lastData = -1;
		lastChangedIndex = -1;
		for (i=0 ; i <= pDoc->m_docSetting.m_bufferSize ; i++) {
//			if (tmpSymbol->m_viewRadix == SVR_ANALOG) {
//			} else {
			if (tmpSymbol->GetWeight() == 1) {
				if (i < pDoc->m_docSetting.m_bufferSize) {						// Is this not the last index?...
					dataAtIndex = 0;												// Display as bit
					if (tmpSymbol->m_validPtrnP) {
						if (tmpSymbol->m_validPtrnP & pDoc->m_podP[i]) dataAtIndex = 1;
					} else {
						if (tmpSymbol->m_validPtrnQ & pDoc->m_podQ[i]) dataAtIndex = 1;
					}
					} else {
						dataAtIndex = -1;											// yes, make sure last data is displayed
					}
				if (dataAtIndex != lastData) {
					tmpYU = activeRow * textHeight + LVC_ROW_MARGIN;
					tmpYL = tmpYU + textHeight - LVC_ROW_MARGIN;
					tmpXL = (long) (lastChangedIndex * pDoc->m_viewScaleFactor);
					tmpXR = (long) (i * pDoc->m_viewScaleFactor);
					if (lastData == 1) {
						pDC->MoveTo(tmpXL, tmpYL);
						pDC->LineTo(tmpXL, tmpYU);
						pDC->LineTo(tmpXR, tmpYU);
					} else {
						pDC->MoveTo(tmpXL, tmpYU);
						pDC->LineTo(tmpXL, tmpYL);
						pDC->LineTo(tmpXR, tmpYL);
					}
					lastData = dataAtIndex;
					lastChangedIndex = i;
				}
			} else if (tmpSymbol->GetWeight() > 1) {	
																				// Display as vector
				if (i < pDoc->m_docSetting.m_bufferSize) {						// Is this not the last index?...
					activeBitIndex = 0;										
					dataAtIndex = 0;											// ...no, get data at index
					for (j=0 ; j<8 ; j++) {
						if (tmpSymbol->m_validPtrnQ & (0x01<<j)) {
							tmpBit = (pDoc->m_podQ[i] >> j) & 0x01;
							dataAtIndex += tmpBit << activeBitIndex++;
						}
					}
					for (j=0 ; j<8 ; j++) {
						if (tmpSymbol->m_validPtrnP & (0x01<<j)) {
							tmpBit = (pDoc->m_podP[i] >> j) & 0x01;
							dataAtIndex += tmpBit << activeBitIndex++;
						}
					}
				} else {
					dataAtIndex = -1;											// yes, make sure last data is displayed
				}
				if (dataAtIndex != lastData) {
					tmpYU = activeRow * textHeight + LVC_ROW_MARGIN;
					tmpYL = tmpYU + textHeight - LVC_ROW_MARGIN;
					tmpXL = (long) (lastChangedIndex * pDoc->m_viewScaleFactor);
					tmpXR = (long) (i * pDoc->m_viewScaleFactor);

					switch (tmpSymbol->m_viewRadix) {
					case PLR_HEX:
						dataRepr.Format("%X", lastData);
						break;
					case PLR_BIN:
						dataRepr = "";
						for (j=tmpSymbol->GetWeight()-1 ; j>=0 ; j--) {
							if ((lastData >> j) & 0x0001) {
								dataRepr += "1";
							} else {
								dataRepr += "0";
							}
						}
						break;
					case PLR_SIGNED:
						if (lastData > pow(2,tmpSymbol->GetWeight()) / 2 - 1) {
							lastData = lastData - (int) pow(2,tmpSymbol->GetWeight());
						}
						dataRepr.Format("%d", lastData);
						break;
					case PLR_UNSIGNED:
						dataRepr.Format("%d", lastData);
						break;
					}

					reprRect.top = tmpYU;
					reprRect.bottom = tmpYL;
					reprRect.left = tmpXL;
					reprRect.right = tmpXR;
/*
					if (i == firstVisIndex) reprRect.left = __max(tmpXL, GetScrollPosition().x);
					else reprRect.left = tmpXL;
					if (i == lastVisIndex + 1) reprRect.right = __min(tmpXR, GetScrollPosition().x + cx);
					else reprRect.right = tmpXR;
*/
					pDC->DrawText(dataRepr, reprRect, /*DT_MODIFYSTRING | DT_END_ELLIPSIS |*/ DT_CENTER | DT_SINGLELINE | DT_VCENTER);

					pDC->MoveTo(tmpXL - LVC_VECTOR_SLOPE_COMP, tmpYU);
					pDC->LineTo(tmpXL + LVC_VECTOR_SLOPE_COMP, tmpYL);
					pDC->LineTo(tmpXR - LVC_VECTOR_SLOPE_COMP, tmpYL);

					pDC->MoveTo(tmpXL - LVC_VECTOR_SLOPE_COMP, tmpYL);
					pDC->LineTo(tmpXL + LVC_VECTOR_SLOPE_COMP, tmpYU);
					pDC->LineTo(tmpXR - LVC_VECTOR_SLOPE_COMP, tmpYU);

					lastData = dataAtIndex;
					lastChangedIndex = i;

				}

			}									
//			}


		}
		activeRow++;
	}

	int	height = m_logicLabelView->GetLabelHeight(pDC) * pDoc->m_docSetting.m_symbolList->GetCount();
	
// Cursor Y -------------------------------------------
	if (pDoc->m_yCursorOn) {
		CPen	curYPen;
		curYPen.CreatePen(PS_SOLID, 1, COLORREF(CURSOR_Y_COLOR));
		pDC->SelectObject(&curYPen);
		pDC->MoveTo((long) (pDoc->m_yCursorT * pDoc->m_viewScaleFactor), 0);
		pDC->LineTo((long) (pDoc->m_yCursorT * pDoc->m_viewScaleFactor), height + 100);
	}

// Cursor X -------------------------------------------

	if (pDoc->m_xCursorOn) {
		CPen	curXPen;
		curXPen.CreatePen(PS_SOLID, 1, COLORREF(CURSOR_X_COLOR));
		pDC->SelectObject(&curXPen);
		pDC->MoveTo((long) (pDoc->m_xCursorT * pDoc->m_viewScaleFactor), 0);
		pDC->LineTo((long) (pDoc->m_xCursorT * pDoc->m_viewScaleFactor), height + 100);
	}

	pDC->SelectObject(oldFont);
	pDC->SelectObject(oldPen);
}

CWsWaveDoc* CWsLogicView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWsWaveDoc)));
	return (CWsWaveDoc*)m_pDocument;
}

/////////////////////////////////////////////////////////////////////////////
// CWsLogicView diagnostics

#ifdef _DEBUG
void CWsLogicView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CWsLogicView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWsLogicView message handlers

void CWsLogicView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	OnChangedSize();
	Invalidate();
}

BOOL CWsLogicView::OnEraseBkgnd(CDC* pDC) 
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

void CWsLogicView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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

BOOL CWsLogicView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	RECT			wr;

	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	GetWindowRect(&wr);
/*
	((CMainFrame*) AfxGetMainWnd())->m_mouseX = pt.x - wr.left;
	((CMainFrame*) AfxGetMainWnd())->m_mouseY = pt.y - wr.top;
*/
	pDoc->MWZoom((zDelta > 0), pt.x - wr.left);

	return 0;
}


void CWsLogicView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int		curLogPos;
	int		distance;

	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	point += GetScrollPosition();

	if (pDoc->m_xCursorOn) {
		// Cursor X Time
		curLogPos = (long) (pDoc->m_xCursorT * pDoc->m_viewScaleFactor);
		distance = abs(curLogPos - point.x);
		if (distance < PV_CURSOR_GRAB_DISTANCE) {
			m_dragState = DS_DRAG_X_TIME_CURSOR;
			SetCapture();
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
	}
}

void CWsLogicView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	point += GetScrollPosition();

	// Cursor X Time
	if (m_dragState & DS_DRAG_X_TIME_CURSOR) {
		pDoc->m_xCursorT = point.x / pDoc->m_viewScaleFactor;
		if (pDoc->m_xCursorT < 0.0) pDoc->m_xCursorT = 0.0;
		if (pDoc->m_xCursorT > pDoc->m_docSetting.m_bufferSize) pDoc->m_xCursorT = pDoc->m_docSetting.m_bufferSize;
	}

	// Cursor Y Time
	if (m_dragState & DS_DRAG_Y_TIME_CURSOR) {
		pDoc->m_yCursorT = point.x / pDoc->m_viewScaleFactor;
		if (pDoc->m_yCursorT < 0.0) pDoc->m_yCursorT = 0.0;
		if (pDoc->m_yCursorT > pDoc->m_docSetting.m_bufferSize) pDoc->m_yCursorT = pDoc->m_docSetting.m_bufferSize;
	}

	if (m_dragState) pDoc->UpdateAllViews(FALSE);
}

void CWsLogicView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWsWaveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	point += GetScrollPosition();

	// Cursor X Time
	if (m_dragState & DS_DRAG_X_TIME_CURSOR) {
		pDoc->m_xCursorT = point.x / pDoc->m_viewScaleFactor;
		if (pDoc->m_xCursorT < 0.0) pDoc->m_xCursorT = 0.0;
		if (pDoc->m_xCursorT > pDoc->m_docSetting.m_bufferSize) pDoc->m_xCursorT = pDoc->m_docSetting.m_bufferSize;
		pDoc->SetModifiedFlag(TRUE);
	}
	// Cursor Y Time
	if (m_dragState & DS_DRAG_Y_TIME_CURSOR) {
		pDoc->m_yCursorT = point.x / pDoc->m_viewScaleFactor;
		if (pDoc->m_yCursorT < 0.0) pDoc->m_yCursorT = 0.0;
		if (pDoc->m_yCursorT > pDoc->m_docSetting.m_bufferSize) pDoc->m_yCursorT = pDoc->m_docSetting.m_bufferSize;
		pDoc->SetModifiedFlag(TRUE);
	}
	if (m_dragState) pDoc->UpdateAllViews(FALSE);
	m_dragState = DS_NONE;
	ReleaseCapture();
}

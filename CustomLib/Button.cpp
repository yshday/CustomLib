#include "stdafx.h"
#include "Button.h"

IMPLEMENT_DYNAMIC(Button, CButton)

Button::Button()
{
}


Button::~Button()
{
	if (m_backDC)
	{
		delete m_backDC;
		m_backDC = nullptr;
	}
}

BEGIN_MESSAGE_MAP(Button, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

BOOL Button::Create(LPCTSTR caption, DWORD style, const RECT & rect, CWnd * parent, UINT id, LPCTSTR imagePath, bool isAutoResize)
{
	style |= BS_OWNERDRAW;

	BOOL bRes = CButton::Create(caption, style, rect, parent, id);
	ImageBase image;
	image.Load(imagePath);

	UINT imageWidth = image.GetWidth() / StateCount;
	UINT imageHeight = image.GetHeight();

	for (UINT i = 0; i < StateCount; i++)
	{
		m_images[i].Load(
			image.GetBitmap()->Clone(
				Rect(imageWidth * i, 0, imageWidth, imageHeight),
				PixelFormatDontCare
			)
		);
	}

	if (isAutoResize)
	{
		Resize(imageWidth, imageHeight);
	}
	else
	{
		CRect rc;
		GetWindowRect(&rc);
		GetParent()->ScreenToClient(&rc);

		UINT width, height;
		width = (rc.Width() > 0) ? rc.Width() : 0;
		height = (rc.Height() > 0) ? rc.Height() : 0;

		if (width > 0 && height > 0)
		{
			rc.right = rc.left + width;
			rc.bottom = rc.top + height;
			MoveWindow(rc);

			GetClientRect(m_textRect);
			m_textRect.OffsetRect(m_textLeft, m_textTop);
		}
		else
		{
			Resize(imageWidth, imageHeight);
		}
	}
	
	return bRes;
}

void Button::OffsetText(const int left, const int top)
{
	m_textLeft = left;
	m_textTop = top;

	GetClientRect(m_textRect);
	m_textRect.OffsetRect(left, top);
}

void Button::EnableHoverCursor(BOOL enable)
{
	m_enabled = enable;
}

void Button::SetFont(CString fontName, int fontSize)
{
	m_font = std::make_unique<Gdiplus::Font>(fontName, fontSize);
}

BOOL Button::PreTranslateMessage(MSG * pMsg)
{
	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;
	
	return CButton::PreTranslateMessage(pMsg);
}

void Button::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rc;
	GetClientRect(&rc);

	CDC* dc = CDC::FromHandle(lpDrawItemStruct->hDC);
	MemoryDC memDC(dc, rc);
	Graphics graphics(memDC.GetSafeHdc());

	graphics.SetInterpolationMode(InterpolationModeDefault);
	graphics.SetPixelOffsetMode(PixelOffsetModeNone);

	if (m_backDC)
		m_backDC->Draw(&memDC);

	ImageBase* curImage;
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		curImage = &m_images[Down];
	else if (lpDrawItemStruct->itemState & ODS_DISABLED)
		curImage = &m_images[Disabled];
	else if (m_isHover)
		curImage = &m_images[Hover];
	else
		curImage = &m_images[Normal];

	if (curImage->GetLastStatus() == Ok)
	{
		curImage->DrawImage(&graphics, rc);
	}

	CString text;
	GetWindowText(text);

	if (!text.IsEmpty())
	{
		 
		//GetFont()->DrawText(&graphics, text, m_textRect);
		StringFormat format;
		format.SetAlignment(StringAlignmentCenter);
		format.SetLineAlignment(StringAlignmentCenter);
		Color color(RGB(255, 255, 255));

		graphics.DrawString(
			text,
			_tcslen(text),
			m_font.get(),
			RectF(
				(REAL)m_textRect.left,
				(REAL)m_textRect.top,
				(REAL)m_textRect.Width(),
				(REAL)m_textRect.Height()
			),
			&format,
			&SolidBrush(color)
		);
	}
}

void Button::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_isTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = 1;
		m_isTracking = _TrackMouseEvent(&tme);
	}

	CButton::OnMouseMove(nFlags, point);
}

void Button::OnMouseHover(UINT nFlags, CPoint point)
{
	m_isHover = TRUE;

	if (m_enableHoverCursor)
		SetClassLong(GetSafeHwnd(), GCL_HCURSOR, (LONG)AfxGetApp()->LoadStandardCursor(IDC_HAND));

	Invalidate(FALSE);

	CButton::OnMouseHover(nFlags, point);
}

void Button::OnMouseLeave()
{
	if (m_isTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_CANCEL;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = 1;
		_TrackMouseEvent(&tme);

		m_isTracking = FALSE;
	}

	m_isHover = FALSE;

	if (m_enableHoverCursor)
		SetClassLong(GetSafeHwnd(), GCL_HCURSOR, (LONG)AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	Invalidate(FALSE);

	CButton::OnMouseLeave();
}

void Button::OnLButtonDown(UINT nFlags, CPoint point)
{
	Invalidate(FALSE);

	CButton::OnLButtonDown(nFlags, point);
}

void Button::OnLButtonUp(UINT nFlags, CPoint point)
{
	Invalidate(FALSE);

	CButton::OnLButtonUp(nFlags, point);
}

BOOL Button::OnEraseBkgnd(CDC * pDC)
{
	return FALSE;
}

void Button::OnEnable(BOOL bEnable)
{
	CButton::OnEnable(bEnable);

	if (!bEnable)
	{
		Invalidate();
	}
}

void Button::Resize(UINT width, UINT height)
{
	CRect rc;
	GetWindowRect(&rc);
	GetParent()->ScreenToClient(&rc);

	rc.right = rc.left + width;
	rc.left = rc.top + height;

	MoveWindow(rc);
	GetClientRect(m_textRect);
	m_textRect.OffsetRect(m_textLeft, m_textTop);
}

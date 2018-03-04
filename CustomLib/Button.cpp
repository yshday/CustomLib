#include "stdafx.h"
#include "Button.h"

IMPLEMENT_DYNAMIC(Button, CButton)

Button::Button()
{
	//CButton::PreSubclassWindow();
	//ModifyStyle(0, BS_OWNERDRAW);
}


Button::~Button()
{
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

void Button::SetImage(HBITMAP image)
{
	m_image = image;
}

void Button::SetSize(int width, int height)
{
	m_size.cx = width;
	m_size.cy = height;
}

void Button::SetBackgroundColor(COLORREF color)
{
	m_backgroundColor = color;
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

	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		m_state = State::Down;
	else if (lpDrawItemStruct->itemState & ODS_DISABLED)
		m_state = State::Disabled;
	else if (m_hover)
		m_state = State::Hover;
	else
		m_state = State::Normal;

	Draw(lpDrawItemStruct->hDC);

	CString text;
	GetWindowText(text);

	if (!text.IsEmpty())
	{
		 
		//GetFont()->DrawText(&graphics, text, m_textRect);
		
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
	m_hover = TRUE;

	//if (m_enableHoverCursor)
	//	SetClassLong(GetSafeHwnd(), GCL_HCURSOR, (LONG)AfxGetApp()->LoadStandardCursor(IDC_HAND));

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

	m_hover = FALSE;

	//if (m_enableHoverCursor)
	//	SetClassLong(GetSafeHwnd(), GCL_HCURSOR, (LONG)AfxGetApp()->LoadStandardCursor(IDC_ARROW));

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

void Button::Draw(HDC hdc)
{
	HBITMAP image = m_image;
	CRect destRect, srcRect(0, 0, m_size.cx, m_size.cy);
	GetClientRect(&destRect);

	switch (m_state)
	{
	case State::Hover:
	{
		srcRect.OffsetRect(m_size.cx, 0);

		break;
	}
	case State::Down:
	{
		srcRect.OffsetRect(m_size.cx * 2, 0);

		break;
	}
	case State::Disabled:
	{
		srcRect.OffsetRect(m_size.cx * 3, 0);

		break;
	}
	default:
		break;
	}

	if (image != NULL)
	{
		FillSolidRect(hdc, destRect, m_backgroundColor);
		HDC hdcBmpMem = ::CreateCompatibleDC(hdc);
		HBITMAP hbmOldBmp = (HBITMAP)::SelectObject(hdcBmpMem, image);

		BLENDFUNCTION bf = { AC_SRC_OVER, 0, 0xFF, AC_SRC_ALPHA };
		::GdiAlphaBlend(hdc, destRect.left, destRect.top, destRect.Width(), destRect.Height(),
			hdcBmpMem, srcRect.left, srcRect.top, srcRect.Width(), srcRect.Height(), bf);

		::SelectObject(hdcBmpMem, hbmOldBmp);
		::DeleteDC(hdcBmpMem);

	}
}

void Button::FillSolidRect(HDC hdc, LPCRECT lpRect, COLORREF clr)
{
	ASSERT(hdc != NULL);

	COLORREF clrOld = ::SetBkColor(hdc, clr);
	ASSERT(clrOld != CLR_INVALID);
	if (clrOld != CLR_INVALID)
	{
		::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
		::SetBkColor(hdc, clrOld);
	}
}
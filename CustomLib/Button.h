#pragma once

#include <memory>

using namespace Gdiplus;

class Button : public CButton
{
	DECLARE_DYNAMIC(Button)

public:
	Button();
	virtual ~Button();

	enum class State : int
	{
		Normal,
		Hover,
		Down,
		Disabled,

		StateCount
	};

	void SetImage(HBITMAP image);
	void SetSize(int width, int height);
	void SetBackgroundColor(COLORREF color);

protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	virtual void	DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void	OnMouseLeave();
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg void	OnEnable(BOOL bEnable);

private:
	void Draw(HDC hdc);
	void FillSolidRect(HDC hdc, LPCRECT lpRect, COLORREF clr);

	HBITMAP m_image = NULL;
	CSize m_size;
	State m_state = State::Normal;
	BOOL m_hover = FALSE;
	BOOL m_isTracking = FALSE;
	COLORREF m_backgroundColor;
};


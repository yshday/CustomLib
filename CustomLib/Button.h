#pragma once

#include "ImageBase.h"
#include "MemoryDC.h"

#include <memory>

using namespace Gdiplus;

class Button : public CButton
{
	DECLARE_DYNAMIC(Button)

public:
	Button();
	virtual ~Button();

	enum State
	{
		Normal,
		Hover,
		Down,
		Disabled,

		StateCount
	};


	virtual BOOL Create(LPCTSTR caption, DWORD style, const RECT& rect, CWnd* parent, UINT id, LPCTSTR imagePath, bool isAutoResize);
	void OffsetText(const int left, const int top);
	void EnableHoverCursor(BOOL enable);
	void SetFont(CString fontName, int fontSize);

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
	void Resize(UINT width, UINT height);

	ImageBase m_images[StateCount];
	MemoryDC* m_backDC;
	std::unique_ptr<Gdiplus::Font> m_font;
	int m_textLeft;
	int m_textTop;
	CRect m_textRect;
	BOOL m_isTracking;
	BOOL m_isHover;
	BOOL m_enableHoverCursor;
	BOOL m_enabled;

};


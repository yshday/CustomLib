#include "stdafx.h"
#include "ComboBox.h"

IMPLEMENT_DYNAMIC(ComboBox, CComboBox)

ComboBox::ComboBox()
{
}

ComboBox::~ComboBox()
{
}



BEGIN_MESSAGE_MAP(ComboBox, CComboBox)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void ComboBox::OnPaint()
{
	CRect rect;
	GetClientRect(&rect);

	CPaintDC dc(this);

	dc.SetBkColor(RGB(255, 255, 255));
	
}

BOOL ComboBox::OnEraseBkgnd(CDC * pDC)
{
	return FALSE;
}

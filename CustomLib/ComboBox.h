#pragma once
class ComboBox : public CComboBox
{
	DECLARE_DYNAMIC(ComboBox)

public:
	ComboBox();
	virtual ~ComboBox();

protected:
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

};


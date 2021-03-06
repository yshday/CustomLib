#pragma once

#include "Button.h"
#include "afxwin.h"
#include "ComboBox.h"


class CustomLibDlg : public CDialogEx
{
public:
	CustomLibDlg(CWnd* pParent = NULL);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUSTOMLIB_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CComboBox m_combo;
	Button m_btn;
	HBITMAP m_image;
};

#include "stdafx.h"
#include "MenuButton.h"

MenuData::MenuData()
{
	menuIcon = -1;
	xoffset = -1;
	bitmap = NULL;
	context = nullptr;
	flags = 0;
	id = 0;
	syncFlag = 0;
	m_menuText = nullptr;
	globalOffset = -1;
}

MenuData::~MenuData()
{
	if (bitmap)
	{
		delete bitmap;
	}

	if (m_menuText)
	{
		delete[] m_menuText;
	}
}

void MenuData::SetAnsiString(LPCSTR str)
{
	USES_CONVERSION;
	SetWideString(A2W(str));
}

void MenuData::SetWideString(const wchar_t * str)
{
	delete[] m_menuText;

	if (str)
	{
		m_menuText = new wchar_t[sizeof(wchar_t)*(wcslen(str) + 1)];
		if (m_menuText)
		{
			wcscpy(m_menuText, str);
		}
	}
	else
	{
		m_menuText = nullptr;
	}
}

const wchar_t * MenuData::GetWideString()
{
	return m_menuText;
}

CString MenuData::GetString()
{
	CString str;
	if (m_menuText)
	{
		str = m_menuText;
	}
	return str;
}

IMPLEMENT_DYNAMIC(MenuButton, CMenu)

MenuButton::MenuButton()
{
}

MenuButton::~MenuButton()
{
	DestroyMenu();
}

BOOL MenuButton::LoadMenu(LPCTSTR resName)
{
	ASSERT_VALID(this);

	HINSTANCE ins = AfxFindResourceHandle(resName, RT_MENU);
	HRSRC src = ::FindResource(ins, resName, RT_MENU);
	if (src == NULL)
	{
		ins = NULL;
		src = ::FindResource(ins, resName, RT_MENU);
	}

	if (src == NULL)
	{
		return FALSE;
	}

	HGLOBAL global = LoadResource(ins, src);
	if (global == NULL)
	{
		return FALSE;
	}

	DestroyMenu();

	if (!CMenu::CreateMenu())
	{
		return FALSE;
	}

	MENUITEMTEMPLATEHEADER* header = reinterpret_cast<MENUITEMTEMPLATEHEADER*>(LockResource(global));
	byte* tp = (byte*)header + (sizeof(MENUITEMTEMPLATEHEADER) + header->offset);

	WORD menuFlags = 0;
	WORD id = 0;
	UINT flags;
	wchar_t* caption = nullptr;
	int len = 0;
	CTypedPtrArray<CPtrArray, MenuButton*> stack;
	CArray<BOOL, BOOL> endStack;
	stack.Add(this);
	endStack.Add(FALSE);

	int j = 0;
	do
	{
		memcpy(&menuFlags, tp, sizeof(WORD));
		tp += sizeof(WORD);
		if (!(menuFlags & MF_POPUP))
		{
			memcpy(&id, tp, sizeof(WORD));
			tp += sizeof(WORD);
		}
		else
		{
			id = 0;
		}
		flags = static_cast<UINT>(menuFlags);

	} while (stack.GetUpperBound() != -1);
}

BOOL MenuButton::LoadMenu(int res)
{
	return MenuButton::LoadMenu(MAKEINTRESOURCE(res));
}

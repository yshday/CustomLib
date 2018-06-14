#pragma once

class MenuData
{
private:
	wchar_t* m_menuText;

public:
	MenuData();
	~MenuData();

	void SetAnsiString(LPCSTR str);
	void SetWideString(const wchar_t* str);
	const wchar_t* GetWideString();
	CString GetString();

	int xoffset;
	int globalOffset;
	int menuIcon;
	UINT flags;
	UINT id;
	UINT syncFlag;
	CImageList* bitmap;
	void* context;

};

struct MenuItem : public tagMENUITEMINFOW
{
	MenuItem()
	{
		memset(this, 0, sizeof MENUITEMINFOW);
		cbSize = sizeof(MENUITEMINFOW);
	}
};

enum class MenuDrawMode
{
	Original,
	Xp
};

enum class Seperator
{
	None,
	Head,
	Tail,
	Both
};

class MenuButton : public CMenu
{
	DECLARE_DYNAMIC(MenuButton)
public:
	MenuButton();
	virtual ~MenuButton();

	virtual BOOL LoadMenu(LPCTSTR resName);
	virtual BOOL LoadMenu(int res);
	BOOL LoadToolbar(UINT toolbar);
};


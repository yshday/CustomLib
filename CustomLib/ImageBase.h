#pragma once

//#include <memory>

using namespace Gdiplus;

/*
class GdiBitmap
{
public:
	GdiBitmap() {}
	GdiBitmap(LPCTSTR filename) { Load(filename); }
	virtual ~GdiBitmap() { Empty(); }

	operator Bitmap*() const { return m_bitmap; }

	bool Load(LPCTSTR filename)
	{
		Empty();
		m_bitmap = Bitmap::FromFile(filename);
		return m_bitmap->GetLastStatus() == Ok;
	}
	void Empty()
	{
		delete m_bitmap;
		m_bitmap = nullptr;
	}

	Bitmap * m_bitmap = nullptr;

};
*/

class Resource/* : public GdiBitmap*/
{
public:
	Resource() { m_buffer = NULL; }
	Resource(LPCTSTR name, LPCTSTR type = RT_RCDATA, HMODULE inst = NULL)
	{
		m_buffer = NULL; Load(name, type, inst);
	}
	Resource(UINT id, LPCTSTR type = RT_RCDATA, HMODULE inst = NULL)
	{
		m_buffer = NULL; Load(id, type, inst);
	}
	Resource(UINT id, UINT type, HMODULE inst = NULL)
	{
		m_buffer = NULL; Load(id, type, inst);
	}
	virtual ~Resource() { Empty(); }

	inline void Empty()
	{ 
		if (m_buffer)
		{
			::GlobalUnlock(m_buffer);

			

			::GlobalFree(m_buffer);
			m_buffer = NULL;

		}
		/*GdiBitmap::Empty();*/

		if (m_bitmap)
		{
			delete m_bitmap;
			m_bitmap = nullptr;
		}
	}

	inline bool Load(LPCTSTR name, LPCTSTR type, HMODULE inst = NULL)
	{
		Empty();

		//HRSRC hResource = ::FindResource(inst, name, type);
		HRSRC hResource = FindResource(AfxGetApp()->m_hInstance, name, TEXT("PNG"));
		if (!hResource)
			return false;

		DWORD imageSize = ::SizeofResource(inst, hResource);
		if (!imageSize)
			return false;

		const void* resource = ::LockResource(::LoadResource(inst, hResource));
		if (!resource)
			return false;

		m_buffer = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
		if (m_buffer)
		{
			void* pBuffer = ::GlobalLock(m_buffer);
			if (pBuffer)
			{
				CopyMemory(pBuffer, resource, imageSize);

				IStream* stream = nullptr;
				if (::CreateStreamOnHGlobal(m_buffer, FALSE, &stream) == S_OK)
				{
					m_bitmap = Gdiplus::Bitmap::FromStream(stream);
					stream->Release();
					if (m_bitmap)
					{
						if (m_bitmap->GetLastStatus() == Ok)
						{
							return true;
						}

						delete m_bitmap;
						m_bitmap = nullptr;
					}
				}
				else
				{
					TRACE(L"");
				}
				::GlobalUnlock(m_buffer);
			}
			::GlobalFree(m_buffer);
			m_buffer = NULL;
		}
		return false;
	}
	bool Load(UINT id, LPCTSTR type = RT_RCDATA, HMODULE inst = NULL)
	{
		return Load(MAKEINTRESOURCE(id), type, inst);
	}

	bool Load(UINT id, UINT type, HMODULE inst = NULL)
	{
		return Load(MAKEINTRESOURCE(id), MAKEINTRESOURCE(type), inst);
	}

	Bitmap * m_bitmap = nullptr;

private:
	HGLOBAL m_buffer;
	
};


class ImageBase
{
public:
	ImageBase();
	ImageBase(LPCTSTR imagePath);
	~ImageBase();

	BOOL Load(LPCTSTR imagePath);
	BOOL Load(Bitmap* bitmap);
	BOOL Remove();
	Bitmap* GetBitmap() const;
	UINT GetWidth() const;
	UINT GetHeight() const;
	Status GetLastStatus() const;
	BOOL DrawImage(Graphics* graphics, CRect rect, UINT alphaBlendRatio = 100);

	Resource* m_resource = nullptr;
private:

};


#include "stdafx.h"
#include "ImageBase.h"


ImageBase::ImageBase()
{

}

ImageBase::ImageBase(LPCTSTR imagePath)
{
	Load(imagePath);
}


ImageBase::~ImageBase()
{
	Remove();
}

BOOL ImageBase::Load(LPCTSTR imagePath)
{
	Remove();

	m_resource = new Resource(imagePath, L"PNG");
	if (m_resource->m_bitmap->GetLastStatus() != Ok)
	{
		Remove();
		return FALSE;
	}

	return TRUE;
}

BOOL ImageBase::Load(Gdiplus::Bitmap * bitmap)
{
	Remove();

	m_resource = new Resource();
	m_resource->m_bitmap = bitmap->Clone(0, 0, bitmap->GetWidth(), bitmap->GetHeight(), PixelFormatDontCare);

	if (m_resource->m_bitmap->GetLastStatus() != Ok)
	{
		Remove();
		return FALSE;
	}

	return TRUE;
}

BOOL ImageBase::Remove()
{
	if (m_resource != nullptr)
	{
		delete m_resource;
		m_resource = nullptr;
	}

	return TRUE;
}

Bitmap * ImageBase::GetBitmap() const
{
	if (m_resource->m_bitmap == nullptr)
	{
		return nullptr;
	}

	return m_resource->m_bitmap;
}

UINT ImageBase::GetWidth() const
{
	return m_resource->m_bitmap->GetWidth();
}

UINT ImageBase::GetHeight() const
{
	return m_resource->m_bitmap->GetHeight();
}

Status ImageBase::GetLastStatus() const
{
	return m_resource->m_bitmap->GetLastStatus();
}

BOOL ImageBase::DrawImage(Graphics * graphics, CRect rect, UINT alphaBlendRatio)
{
	ImageAttributes imageAttr;
	float alpahBlend = (float)alphaBlendRatio / 100;
	ColorMatrix matrix = {
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, alpahBlend, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	imageAttr.SetColorMatrix(&matrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
	graphics->DrawImage(
		m_resource->m_bitmap,
		//rect,
		Rect(rect.left, rect.top, rect.right, rect.bottom),
		0,
		0,
		m_resource->m_bitmap->GetWidth(),
		m_resource->m_bitmap->GetHeight(),
		UnitPixel,
		&imageAttr);

	return TRUE;
}

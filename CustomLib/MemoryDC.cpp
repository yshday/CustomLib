#include "stdafx.h"
#include "MemoryDC.h"

MemoryDC::MemoryDC(CDC *pDC, BOOL bDrawOut)
{
	Initialize(pDC, NULL, bDrawOut);
}

MemoryDC::MemoryDC(CDC *pDC, CRect &rect, BOOL bDrawOut)
{
	Initialize(pDC, &rect, bDrawOut);
}

MemoryDC::~MemoryDC()
{
	if (m_bMemDC)
	{
		if (m_bDrawOut)
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), this, m_rect.left, m_rect.top, SRCCOPY);

		SelectObject(m_pOldBitmap);
		//DeleteDC(); // ���� delete ���� �ʾƵ� �ȴ�.
	}
	else
	{
		m_hDC = NULL;
		m_hAttribDC = NULL;
	}
}

void MemoryDC::Initialize(CDC *pDC, const CRect *pRect, BOOL bDrawOut)
{
	m_pDC = pDC;
	m_pOldBitmap = NULL;
	m_bMemDC = !pDC->IsPrinting(); // DC�� �μ⸦ ���Ͽ� ���Ǵ��� �˻��ϴ� �Լ�
	m_bDrawOut = bDrawOut;

	if (m_bMemDC)
	{
		CreateCompatibleDC(pDC);

		if (NULL == pRect)
			pDC->GetClipBox(&m_rect);
		else
			m_rect = *pRect;

		// �޸� ��Ʈ���� ȭ�� ǥ���� �������� �����, �̸� �����մϴ�.
		m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
		m_pOldBitmap = SelectObject(&m_bitmap);

		SetWindowOrg(m_rect.left, m_rect.top);

		// Fill background, ����� ���� ���� �Ű����� DC �� �������� ä���.
		FillSolidRect(m_rect, pDC->GetBkColor());
		//FillSolidRect(m_rect, RGB(255, 255, 255));
	}
	else
	{
		m_bPrinting = pDC->m_bPrinting;
		m_hDC = pDC->m_hDC;
		m_hAttribDC = pDC->m_hAttribDC;
	}
}

MemoryDC *MemoryDC::operator->()
{
	return this;
}

MemoryDC::operator MemoryDC*()
{
	return this;
}

BOOL MemoryDC::Draw(CDC *pDC)
{
	return pDC->BitBlt(0, 0, m_rect.Width(), m_rect.Height(), this, m_rect.left, m_rect.top, SRCCOPY);
}

void MemoryDC::EnableDrawOut(BOOL bDrawOut)
{
	m_bDrawOut = bDrawOut;
}
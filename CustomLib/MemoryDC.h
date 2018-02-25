#pragma once
class MemoryDC : public CDC
{
	// Constructors
public:
	MemoryDC(CDC *pDC, BOOL bDrawOut = TRUE);
	MemoryDC(CDC *pDC, CRect &rect, BOOL bDrawOut = TRUE);

	// Attributes
private:
	CDC * m_pDC;
	CBitmap		m_bitmap;
	CBitmap		*m_pOldBitmap;

	CRect		m_rect;
	BOOL		m_bMemDC;
	BOOL		m_bDrawOut;

	// Operations
public:
	BOOL Draw(CDC *pDC);
	void EnableDrawOut(BOOL bDrawOut = TRUE);

	MemoryDC *operator->();
	operator MemoryDC *();

protected:
	void Initialize(CDC *pDC, const CRect *pRect, BOOL bDrawOut);

	// Implementation
public:
	virtual ~MemoryDC();
};


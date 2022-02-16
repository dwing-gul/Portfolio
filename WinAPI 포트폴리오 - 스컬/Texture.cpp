#include "stdafx.h"
#include "Texture.h"
#include "Core.h"

Texture::Texture() :
	m_bit(0),
	m_dc(0),
	m_bitmap{}
{
}

Texture::~Texture()
{
	DeleteObject(m_bit);
	DeleteDC(m_dc);
}

int Texture::Load(const wchar_t* _filePath)
{
	m_bit = (HBITMAP)LoadImage(nullptr, _filePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if (nullptr == m_bit)
	{
		return E_FAIL;
	}

	m_dc = CreateCompatibleDC(Core::GetInst()->GetCoreDC());

	HBITMAP prevBit = (HBITMAP)SelectObject(m_dc, m_bit);
	DeleteObject(prevBit);

	m_bitmap = {};
	GetObject(m_bit, sizeof(BITMAP), (void*)&m_bitmap);

	return S_OK;
}

void Texture::Create(UINT _width, UINT _height)
{
	m_dc = CreateCompatibleDC(Core::GetInst()->GetCoreDC());
	m_bit = CreateCompatibleBitmap(Core::GetInst()->GetCoreDC(), _width, _height);
	HBITMAP prevBitmap = (HBITMAP)SelectObject(m_dc, m_bit);
	DeleteObject(prevBitmap);

	m_bitmap = {};
	GetObject(m_bit, sizeof(BITMAP), (void*)&m_bitmap);
}

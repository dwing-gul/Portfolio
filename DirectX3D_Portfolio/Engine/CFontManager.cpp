#include "pch.h"
#include "CFontManager.h"
#include "CDevice.h"

CFontManager::CFontManager()
	: m_FW1Factory(nullptr)
	, m_FontWrapper(nullptr)
{
}

CFontManager::~CFontManager()
{
	if(nullptr != m_FW1Factory)
		m_FW1Factory->Release();

	if (nullptr != m_FontWrapper)
		m_FontWrapper->Release();
}

void CFontManager::init()
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_FW1Factory)))
	{
		assert(NULL);
	}
		
	if (FAILED(m_FW1Factory->CreateFontWrapper(DEVICE, L"Arial", &m_FontWrapper)))
	{
		assert(NULL);
	}
}

void CFontManager::DrawFont(const wchar_t * _str, float _posX, float _posY, float _fontSize, UINT _color)
{
	m_FontWrapper->DrawString(
		CONTEXT,
		_str, // String
		_fontSize,// Font size
		_posX,// X position
		_posY,// Y position
		_color,// Text color, 0xAaBbGgRr
		0x1      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}


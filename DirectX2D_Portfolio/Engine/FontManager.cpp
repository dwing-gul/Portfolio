#include "pch.h"
#include "FontManager.h"
#include "Device.h"

FontManager::FontManager()
	: m_FW1Factory(nullptr)
	, m_FontWrapper(nullptr)
{
}

FontManager::~FontManager()
{
	if(nullptr != m_FW1Factory)
		m_FW1Factory->Release();

	if (nullptr != m_FontWrapper)
		m_FontWrapper->Release();
}

void FontManager::init()
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_FW1Factory)))
	{
		assert(NULL);
	}
		
	if (FAILED(m_FW1Factory->CreateFontWrapper(DEVICE.Get(), L"Arial", &m_FontWrapper)))
	{
		assert(NULL);
	}
}

void FontManager::DrawFont(const wchar_t * _str, float _posX, float _posY, float _fontSize, UINT _color)
{
	m_FontWrapper->DrawString(
		CONTEXT.Get(),
		_str, // String
		_fontSize,// Font size
		_posX,// X position
		_posY,// Y position
		_color,// Text color, 0xAaBbGgRr
		0      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}


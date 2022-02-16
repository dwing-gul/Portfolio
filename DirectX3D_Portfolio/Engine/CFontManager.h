#pragma once

#include <FontEngine/FW1FontWrapper.h>
#include <FontEngine/FW1CompileSettings.h>

#ifdef _DEBUG
#pragma comment(lib, "FontEngine/FW1FontWrapper_debug")
#else
#pragma comment(lib, "FontEngine/FW1FontWrapper")
#endif


#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)

class CFontManager
	: public CSingleton<CFontManager>
{
	SINGLE(CFontManager);

private:
	IFW1Factory*		m_FW1Factory;
	IFW1FontWrapper*	m_FontWrapper;
	
public:
	void init();
	void DrawFont(const wchar_t* _str, float _posX, float _posY, float _fontSize, UINT _color);
};




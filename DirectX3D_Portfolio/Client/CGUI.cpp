#include "pch.h"
#include "CGUI.h"
#include <Engine/CRes.h>

CGUI::CGUI() :
	m_bActive(true),
	m_bCancelButton(true)
{
}

CGUI::~CGUI()
{
}

void CGUI::GetResourceName(CRes* _res, char* _out)
{
	wstring key;

	if (nullptr != _res)
		key = _res->GetKey();

	string output = string(key.begin(), key.end());
	strcpy_s(_out, 255, output.c_str());
}

#include "pch.h"
#include "GUI.h"

GUI::GUI() :
	m_bActive(true),
	m_bCancelButton(true)
{
}

GUI::~GUI()
{
}

void GUI::GetResourceName(Res* _res, char* _out)
{
	wstring key;

	if (nullptr != _res)
	{
		key = _res->GetKey();
	}

	string output = WStringToString(key);
	strcpy_s(_out, 255, output.c_str());
}

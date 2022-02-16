#pragma once
#include "CListGUI.h"

class CTexture;

class CParamGUI
{
public :
	static bool render_int(const string& _name, int* _out);
	static bool render_float(const string& _name, float* _out);
	static bool render_vec2(const string& _name, Vec2* _out);
	static bool render_vec4(const string& _name, Vec4* _out);
	static bool render_matrix(const string& _name, Matrix* _out);
	static bool render_texture(const string& _name, CTexture* _texture, CGUI* _inst, GUI_CALLBACK _func);

private :
	CParamGUI() {}
	~CParamGUI() {}
};


#pragma once
#include "ListGUI.h"

class Texture;

class ParamGUI
{
public :
	static bool render_int(const string& _name, int* _out);
	static bool render_float(const string& _name, float* _out);
	static bool render_vector2(const string& _name, Vector2* _out);
	static bool render_vector4(const string& _name, Vector4* _out);
	static bool render_texture(const string& _name, Texture* _outTexture, GUI* _inst, GUI_CALLBACK _func);

private :
	ParamGUI() {}
	~ParamGUI() {}
};


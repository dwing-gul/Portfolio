#include "pch.h"
#include "CParamGUI.h"
#include "imgui.h"
#include "CImGuiManager.h"

bool CParamGUI::render_int(const string& _name, int* _out)
{
	string text = _name.substr(0, _name.find("##"));
	ImGui::Text(text.c_str());

	string label = "##";
	label += _name;
	if (ImGui::InputInt(label.c_str(), _out))
	{
		return true;
	}

	return false;
}

bool CParamGUI::render_float(const string& _name, float* _out)
{
	string text = _name.substr(0, _name.find("##"));
	ImGui::Text(text.c_str());

	string label = "##";
	label += _name;
	if (ImGui::InputFloat(label.c_str(), _out))
	{
		return true;
	}

	return false;
}

bool CParamGUI::render_vec2(const string& _name, Vec2* _out)
{
	string text = _name.substr(0, _name.find("##"));
	ImGui::Text(text.c_str());

	string label = "##";
	label += _name;
	if (ImGui::InputFloat2(label.c_str(), *_out))
	{
		return true;
	}

	return false;
}

bool CParamGUI::render_vec4(const string& _name, Vec4* _out)
{
	string text = _name.substr(0, _name.find("##"));
	ImGui::Text(text.c_str());

	string label = "##";
	label += _name;
	if (ImGui::InputFloat4(label.c_str(), *_out))
	{
		return true;
	}

	return false;
}

bool CParamGUI::render_matrix(const string& _name, Matrix* _out)
{
	return false;
}

bool CParamGUI::render_texture(const string& _name, CTexture* _texture, CGUI* _inst, GUI_CALLBACK _func)
{
	string text = _name.substr(0, _name.find("##"));
	ImGui::Text(text.c_str());

	CListGUI* listGUI = (CListGUI*)CImGuiManager::GetInst()->FindGUI("ListGUI");
	listGUI->SetCaption("Shader");

	ImVec2 uvMin = ImVec2(0.f, 0.f);
	ImVec2 uvMax = ImVec2(1.f, 1.f);
	ImVec4 tintColor = ImVec4(1.f, 1.f, 1.f, 1.f);
	ImVec4 borderColor = ImVec4(1.f, 1.f, 1.f, 0.5f);

	ImTextureID texID = 0;
	if (nullptr != _texture)
	{
		texID = (ImTextureID)_texture->GetSRV().Get();
	}
	ImGui::Image(texID, ImVec2(150.f, 150.f), uvMin, uvMax, tintColor, borderColor);

	ImGui::SameLine();

	string label = "Select##";
	label += _name;

	if (ImGui::Button(label.c_str()))
	{
		vector<wstring> vectorName;
		CResourceManager::GetInst()->GetResName(RESOURCE_TYPE::TEXTURE, vectorName);
		listGUI->SetList(vectorName);
		listGUI->SetDoubleClickedCallback(_inst, _func);
		listGUI->Activate();

		return true;
	}

	return false;
}

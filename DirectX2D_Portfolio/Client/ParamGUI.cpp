#include "pch.h"
#include "ParamGUI.h"
#include "imgui.h"
#include "ImGuiManager.h"

bool ParamGUI::render_int(const string& _name, int* _out)
{
    string text = _name.substr(0, _name.find("##"));
    ImGui::Text(text.c_str());

    string label = "##";
    label += _name;
    if (ImGui::InputInt(label.c_str(), _out))
        return true;

    return false;
}

bool ParamGUI::render_float(const string& _name, float* _out)
{
    string text = _name.substr(0, _name.find("##"));
    ImGui::Text(text.c_str());

    string label = "##";
    label += _name;
    if (ImGui::InputFloat(label.c_str(), _out))
        return true;

    return false;
}

bool ParamGUI::render_vector2(const string& _name, Vector2* _out)
{
    string text = _name.substr(0, _name.find("##"));
    ImGui::Text(text.c_str());

    string label = "##";
    label += _name;
    if (ImGui::InputFloat2(label.c_str(), *_out))
        return true;

    return false;;
}

bool ParamGUI::render_vector4(const string& _name, Vector4* _out)
{
    string text = _name.substr(0, _name.find("##"));
    ImGui::Text(text.c_str());

    string label = "##";
    label += _name;
    if (ImGui::InputFloat4(label.c_str(), *_out))
        return true;

    return false;
}

bool ParamGUI::render_texture(const string& _name, Texture* _outTexture, GUI* _inst, GUI_CALLBACK _func)
{
    string text = _name.substr(0, _name.find("##"));
    ImGui::Text(text.c_str());

    ListGUI* listGUI = (ListGUI*)ImGuiManager::GetInst()->FindGUI("ListGUI");
    listGUI->SetCaption("Shader");

    ImVec2 uvMin = ImVec2(0.f, 0.f);
    ImVec2 uvMax = ImVec2(1.f, 1.f);
    ImVec4 tintColor = ImVec4(1.f, 1.f, 1.f, 1.f);
    ImVec4 borderColor = ImVec4(1.f, 1.f, 1.f, 1.f);

    ImTextureID texID = 0;

    if (nullptr != _outTexture)
    {
        texID = (ImTextureID)_outTexture->GetSRV().Get();
    }
    ImGui::Image(texID, ImVec2(150.f, 150.f), uvMin, uvMax, tintColor, borderColor);
    ImGui::SameLine();

    string label = "Select##";
    label += _name;

    if (ImGui::Button(label.c_str()))
    {
        vector<wstring> vectorName;
        ResourceManager::GetInst()->GetResName(RESOURCE_TYPE::TEXTURE, vectorName);
        listGUI->SetList(vectorName);
        listGUI->SetDoubleClickedCallBack(_inst, _func);
        listGUI->Activate();
        return true;
    }
    
    return false;
}

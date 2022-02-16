#include "pch.h"
#include "InspectorGUI.h"
#include <Engine/EventManager.h>
#include <Engine/KeyManager.h>
#include <Engine/SceneManager.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/GameObject.h>
#include <Engine/Collider2D.h>
#include <Engine/Light2D.h>
#include <Engine/Transform.h>
#include <Engine/Script.h>
#include <Engine/MeshRender.h>
#include <Engine/Camera.h>
#include <Engine/Animator2D.h>
#include <Engine/TileMap.h>
#include <Engine/Particle.h>
#include <Engine/Res.h>
#include <Engine/Core.h>
#include "ImGuiManager.h"
#include "TextureGUI.h"
#include "MaterialGUI.h"
#include "MeshGUI.h"
#include "PrefabGUI.h"
#include "GraphicsShaderGUI.h"
#include "ComputeShaderGUI.h"
#include "TransformGUI.h"
#include "MeshRenderGUI.h"
#include "CameraGUI.h"
#include "Collider2DGUI.h"
#include "Animator2DGUI.h"
#include "Light2DGUI.h"
#include "ParticleGUI.h"
#include "TileMapGUI.h"
#include "DoorScriptGUI.h"
#include "WallScriptGUI.h"
#include "TextUIScriptGUI.h"

InspectorGUI::InspectorGUI() :
    m_arrComponentGUI{},
    m_arrResourceGUI{},
    m_arrScriptGUI{},
    m_targetObject(nullptr),
    m_targetResource(nullptr),
    m_curScene(nullptr),
    m_mode(INFO_MODE::NONE)
{
    SetName("Inspector");
}

InspectorGUI::~InspectorGUI()
{
    Safe_Delete_Arr(m_arrComponentGUI);
    Safe_Delete_Arr(m_arrResourceGUI);
    Safe_Delete_Arr(m_arrScriptGUI);
}

void InspectorGUI::init()
{
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetSize(ImVec2(0.f, 100.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::MESH_RENDER] = new MeshRenderGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::MESH_RENDER]->SetSize(ImVec2(0.f, 80.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetSize(ImVec2(0.f, 80.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new Animator2DGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]->SetSize(ImVec2(0.f, 180.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetSize(ImVec2(0.f, 240.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::CAMERA] = new CameraGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::CAMERA]->SetSize(ImVec2(0.f, 130.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::PARTICLE] = new ParticleGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::PARTICLE]->SetSize(ImVec2(0.f, 300.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::TILEMAP] = new TileMapGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::TILEMAP]->SetSize(ImVec2(0.f, 180.f));

    m_arrScriptGUI[(UINT)SCRIPT_TYPE::DOORSCRIPT] = new DoorScriptGUI;
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::DOORSCRIPT]->SetSize(ImVec2(0.f, 80.f));

    m_arrScriptGUI[(UINT)SCRIPT_TYPE::WALLSCRIPT] = new WallScriptGUI;
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::WALLSCRIPT]->SetSize(ImVec2(0.f, 80.f));

    m_arrScriptGUI[(UINT)SCRIPT_TYPE::TEXTUISCRIPT] = new TextUIScriptGUI;
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::TEXTUISCRIPT]->SetSize(ImVec2(0.f, 210.f));

    m_arrResourceGUI[(UINT)RESOURCE_TYPE::MATERIAL] = new MaterialGUI;
    m_arrResourceGUI[(UINT)RESOURCE_TYPE::TEXTURE] = new TextureGUI;
    m_arrResourceGUI[(UINT)RESOURCE_TYPE::MESH] = new MeshGUI;
    m_arrResourceGUI[(UINT)RESOURCE_TYPE::PREFAB] = new PrefabGUI;
    m_arrResourceGUI[(UINT)RESOURCE_TYPE::GRAPHICS] = new GraphicsShaderGUI;
    m_arrResourceGUI[(UINT)RESOURCE_TYPE::COMPUTE] = new ComputeShaderGUI;
}

void InspectorGUI::update()
{
    Scene* curScene = SceneManager::GetInst()->GetCurScene();

    if (curScene && curScene->GetSceneType() == SCENE_TYPE::PLAY_STAGE)
    {
        SetCancelButton(false);
    }

    if (!IsCancelButton())
    {
        m_targetObject = nullptr;
        m_targetResource = nullptr;
        return;
    }

    if (curScene != m_curScene)
    {
        m_curScene = curScene;
        m_mode = INFO_MODE::NONE;
    }
    
    if (ImGui::Begin(GetName().c_str(), &IsCancelButton()))
    {
        switch (m_mode)
        {
        case INFO_MODE::GAMEOBJECT:
            update_object();
            break;
        case INFO_MODE::RESOURCE:
            update_resource();
            break;
        case INFO_MODE::NONE:
            break;
        }

        ImGui::End();
    }
}

void InspectorGUI::update_object()
{
    if (nullptr == m_targetObject || m_targetObject->IsDead())
        return;

    char buffer[255] = {};
    sprintf_s(buffer, "##%lld", (DWORD_PTR)m_targetObject);
    string nameID;
    nameID += buffer;

    string name = WStringToString(m_targetObject->GetName());
    char objectName[255] = {};
    strcpy_s(objectName, 255, name.c_str());
    ImGui::InputText(nameID.c_str(), objectName, 255);

    wstring targetName = StringToWString(objectName);
    if (KEY_TAP(KEY::ENTER) && ImGui::IsItemFocused())
    {
        m_targetObject->SetName(targetName);
        EventManager::GetInst()->EventOccur();
    }

    ImGui::Text("Layer");
    ImGui::SameLine();
    nameID = "##LayerInput";
    nameID += buffer;
    int curLayerIndex = m_targetObject->GetLayerIndex();
    int layerIndex = m_targetObject->GetLayerIndex();
    ImGui::InputInt(nameID.c_str(), &layerIndex);

    if (KEY_TAP(KEY::ENTER) && curLayerIndex != layerIndex && ImGui::IsItemFocused())
    {
        Layer* layer = SceneManager::GetInst()->GetCurScene()->GetLayer(curLayerIndex);
        layer->DeregisterObj(m_targetObject);

        Script::CreateObject(m_targetObject, m_targetObject->GetTransform()->GetLocalPos(), layerIndex);
    }

    OBJECT_TYPE type = m_targetObject->GetObjectType();

    ImGui::Text("ObjectType");
    ImGui::SameLine(100.f, 5.f);
    ImGui::SetNextItemWidth(180);
    int objTypeidx = (int)type; // Here we store our selection data as an index.
    const char* combo_preview_value = ObjectTypeString[objTypeidx];  // Pass in the preview value visible before opening the combo (it could be anything)
    if (ImGui::BeginCombo("##ObjectType", combo_preview_value))
    {
        for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; i++)
        {
            const bool is_selected = (objTypeidx == i);
            if (ImGui::Selectable(ObjectTypeString[i], is_selected))
            {
                objTypeidx = i;
                type = (OBJECT_TYPE)i;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    m_targetObject->SetObjectType(type);
   
    for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        if (nullptr == m_arrComponentGUI[i])
            continue;

        m_arrComponentGUI[i]->update();
    }

    ImGui::Separator();

    if (!m_targetObject->GetScripts().empty())
    {
        for (UINT i = 0; i < (UINT)SCRIPT_TYPE::END; ++i)
        {
            if (nullptr == m_arrScriptGUI[i])
                continue;

            m_arrScriptGUI[i]->update();
        }

        ImGui::Separator();
    }

    ImVec2 buttonSize = ImVec2(150.f, 30.f);
    float space = (ImGui::GetWindowWidth() - buttonSize.x) / 2.f;
    ImGui::Dummy(ImVec2(0.f, 10.f));
    ImGui::Dummy(ImVec2(0.f, 0.f));
    ImGui::SameLine(space);
    if (ImGui::Button("Add Component##ObjectMenu", buttonSize))
        ImGui::OpenPopup("Components##Add Component");
    if (ImGui::BeginPopup("Components##Add Component"))
    {
        AddComponentToObj();
        ImGui::EndPopup();
    }
}

void InspectorGUI::update_resource()
{
    if (nullptr == m_targetResource)
        return;

    char buffer[255] = {};
    sprintf_s(buffer, "##%lld", (DWORD_PTR)m_targetResource);
    string nameID;
    nameID += buffer;

    string name = WStringToString(m_targetResource->GetKey());
    char resourceName[255] = {};
    strcpy_s(resourceName, 255, name.c_str());
    UINT flag = 0;

    if (m_targetResource->IsDefault())
    {
        flag = ImGuiInputTextFlags_ReadOnly;
    }

    ImGui::InputText(nameID.c_str(), resourceName, 255, flag);

    wstring targetName = StringToWString(resourceName);
    if (KEY_TAP(KEY::ENTER) && ImGui::IsItemFocused())
    {
        ResourceManager::GetInst()->SubRes(m_targetResource->GetKey(), m_targetResource->GetType());
        m_targetResource->SetKey(targetName);
        ResourceManager::GetInst()->AddRes(m_targetResource->GetKey(), m_targetResource, m_targetResource->GetType());
    }

    if (m_targetResource->GetType() == RESOURCE_TYPE::MATERIAL)
    {
        SaveMaterial();
    }

    ImGui::Separator();

    RESOURCE_TYPE type = m_targetResource->GetType();

    if (nullptr == m_arrResourceGUI[(UINT)type])
        return;

    m_arrResourceGUI[(UINT)type]->update();
}

void InspectorGUI::SetTargetObject(GameObject* _target)
{
    m_targetObject = _target;
    m_mode = INFO_MODE::GAMEOBJECT;

    for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        if (nullptr == m_arrComponentGUI[i])
            continue;

        m_arrComponentGUI[i]->SetTarget(m_targetObject);
    }

    for (UINT i = 0; i < (UINT)SCRIPT_TYPE::END; ++i)
    {
        if (nullptr == m_arrScriptGUI[i])
            continue;

        m_arrScriptGUI[i]->SetTarget(m_targetObject);
    }
}

void InspectorGUI::SetTargetResource(Res* _target)
{
    m_targetResource = _target;
    m_mode = INFO_MODE::RESOURCE;

    RESOURCE_TYPE type = m_targetResource->GetType();

    if (nullptr == m_arrResourceGUI[(UINT)type])
        return;

    m_arrResourceGUI[(UINT)type]->SetTarget(m_targetResource);
}

void InspectorGUI::AddComponentToObj()
{
    Component* component = nullptr;

    if (ImGui::MenuItem("Collider2D##Add Component")) 
    {
        component = new Collider2D;
        if (!m_targetObject->GetComponent(component->GetType()))
            m_targetObject->AddComponent(component);
        else
            delete component;
    }
    if (ImGui::MenuItem("Animator2D##Add Component"))
    {
        component = new Animator2D;
        if (!m_targetObject->GetComponent(component->GetType()))
            m_targetObject->AddComponent(component);
        else
            delete component;
    }
    if (ImGui::MenuItem("Light2D##Add Component"))
    {
        component = new Light2D;
        if(!m_targetObject->GetComponent(component->GetType()))
            m_targetObject->AddComponent(component);
        else
            delete component;
    }
    if (ImGui::MenuItem("Camera##Add Component"))
    {
        component = new Camera;
        if (!m_targetObject->GetComponent(component->GetType()))
            m_targetObject->AddComponent(component);
        else
            delete component;
    }
    if (ImGui::MenuItem("MeshRender##Add Component"))
    {
        if (m_targetObject->GetComponent(COMPONENT_TYPE::TILEMAP) || m_targetObject->GetComponent(COMPONENT_TYPE::PARTICLE))
        {
            MessageBox(nullptr, L"TileMap 또는 Particle 컴포넌트가 이미 존재합니다.", L"Add Component Error", MB_OK);
            return;
        }

        component = new MeshRender;
        if (!m_targetObject->GetComponent(component->GetType()))
            m_targetObject->AddComponent(component);
        else
            delete component;
    }
    if (ImGui::MenuItem("TileMap##Add Component"))
    {
        if (m_targetObject->GetComponent(COMPONENT_TYPE::MESH_RENDER) || m_targetObject->GetComponent(COMPONENT_TYPE::PARTICLE))
        {
            MessageBox(nullptr, L"MeshRender 또는 Particle 컴포넌트가 이미 존재합니다.", L"Add Component Error", MB_OK);
            return;
        }

        component = new TileMap;
        if (!m_targetObject->GetComponent(component->GetType()))
            m_targetObject->AddComponent(component);
        else
            delete component;
    }
    if (ImGui::MenuItem("Particle##Add Component")) 
    {
        if (m_targetObject->GetComponent(COMPONENT_TYPE::MESH_RENDER) || m_targetObject->GetComponent(COMPONENT_TYPE::TILEMAP))
        {
            MessageBox(nullptr, L"MeshRender 또는 TileMap 컴포넌트가 이미 존재합니다.", L"Add Component Error", MB_OK);
            return;
        }

        component = new Particle;
        if (!m_targetObject->GetComponent(component->GetType()))
            m_targetObject->AddComponent(component);
        else
            delete component;
    }
    if (ImGui::BeginMenu("Scripts##Add Component"))
    {
        vector<wstring> vectorScriptsNameByWString;
        vector<string> vectorScriptsName;
        ScriptManager::GetScriptInfo(vectorScriptsNameByWString);

        for (size_t i = 0; i < vectorScriptsNameByWString.size(); ++i)
        {
            vectorScriptsName.push_back(WStringToString(vectorScriptsNameByWString[i]));
        }

        for (size_t i = 0; i < vectorScriptsName.size(); ++i)
        {
            vectorScriptsName[i] += "##Add Component";
            if (ImGui::MenuItem(vectorScriptsName[i].c_str()))
            {
                Script* script = nullptr;
                script = ScriptManager::GetScript(vectorScriptsNameByWString[i]);

                if (!m_targetObject->GetScripts().empty())
                {
                    for (size_t j = 0; j < m_targetObject->GetScripts().size(); ++j)
                    {
                        if (script->GetScriptType() != m_targetObject->GetScripts()[j]->GetScriptType())
                        {
                            m_targetObject->AddComponent(script);
                        }
                        else
                            delete script;
                    }
                }
                else
                {
                    m_targetObject->AddComponent(script);
                }
            }
        }
       
        ImGui::EndMenu();
    }
}

void InspectorGUI::SaveMaterial()
{
    ImGui::SameLine();

    if(ImGui::Button("Save##Material"))
    {
        OPENFILENAME ofn = {};       // common dialog box structure
        wchar_t szFile[260] = { 0 };       // if using TCHAR macros

        wstring strMapFilePath = PathManager::GetInst()->GetContentPath();
        strMapFilePath += L"material";

        // Initialize OPENFILENAME	
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = Core::GetInst()->GetWindowHandle();
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = L"All\0*.*\0머터리얼\0*.material\0";
        ofn.nFilterIndex = 2;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = strMapFilePath.c_str(); // 탐색창 초기 경로
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetSaveFileName(&ofn)) // DoModal (modal window)
        {
            wstring strOutput = ofn.lpstrFile;
            wstring contentPath = PathManager::GetInst()->GetContentPath();

            wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
            m_targetResource->SetRelativePath(relativePath);
            m_targetResource->Save(relativePath);
        }
    }
}

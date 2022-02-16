#include "pch.h"
#include "CInspectorGUI.h"

#include <Engine/CEventManager.h>
#include <Engine/CSceneManager.h>
#include <Engine/CGameObject.h>
#include <Engine/CRes.h>
#include <Engine/CScene.h>

#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CCollider2D.h>
#include <Engine/CCollider3D.h>
#include <Engine/CLight2D.h>
#include <Engine/CLight3D.h>
#include <Engine/CTransform.h>
#include <Engine/CTerrain.h>
#include <Engine/CScript.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCamera.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CTileMap.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CCore.h>

#include "CImGuiManager.h"
#include "CResourceGUI.h"
#include "CTextureGUI.h"
#include "CMaterialGUI.h"
#include "CMeshGUI.h"
#include "CMeshDataGUI.h"
#include "CPrefabGUI.h"
#include "CGraphicsShaderGUI.h"
#include "CComputeShaderGUI.h"
#include "CTransformGUI.h"
#include "CTerrainGUI.h"
#include "CMeshRenderGUI.h"
#include "CAnimator2DGUI.h"
#include "CAnimator3DGUI.h"
#include "CParticleSystemGUI.h"
#include "CCameraGUI.h"
#include "CScriptGUI.h"
#include "CCollider2DGUI.h"
#include "CCollider3DGUI.h"
#include "CLight2DGUI.h"
#include "CLight3DGUI.h"
#include "CTileMapGUI.h"
#include "CNaviMeshGUI.h"

#include "CIGItemScriptGUI.h"
#include "CMonsterScriptGUI.h"
#include "CEffectScriptGUI.h"
#include "CFontScriptGUI.h"
#include "CMonsterCreatGUI.h"
#include "CItemBoxScriptGUI.h"
#include "CSkillScriptGUI.h"
#include "CBarScriptGUI.h"
#include "CTrackingUIScriptGUI.h"
#include "CPictureScriptGUI.h"
#include "CButtonScriptGUI.h"
#include "CSwitchScriptGUI.h"
#include "CAIPointScriptGUI.h"

CInspectorGUI::CInspectorGUI() :
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

CInspectorGUI::~CInspectorGUI()
{
	Safe_Delete_Arr(m_arrComponentGUI);
    Safe_Delete_Arr(m_arrScriptGUI);
    Safe_Delete_Arr(m_arrResourceGUI);
}

void CInspectorGUI::init()
{
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new CTransformGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetSize(ImVec2(0.f, 100.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::MESH_RENDER] = new CMeshRenderGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::MESH_RENDER]->SetSize(ImVec2(0.f, 200.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new CCollider2DGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetSize(ImVec2(0.f, 80.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::COLLIDER3D] = new CCollider3DGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::COLLIDER3D]->SetSize(ImVec2(0.f, 80.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new CAnimator2DGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]->SetSize(ImVec2(0.f, 180.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::ANIMATOR3D] = new CAnimator3DGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::ANIMATOR3D]->SetSize(ImVec2(0.f, 180.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new CLight2DGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetSize(ImVec2(0.f, 240.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::LIGHT3D] = new CLight3DGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::LIGHT3D]->SetSize(ImVec2(0.f, 240.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::CAMERA] = new CCameraGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::CAMERA]->SetSize(ImVec2(0.f, 130.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::TERRAIN] = new CTerrainGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::TERRAIN]->SetSize(ImVec2(0.f, 130.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::PARTICLE] = new CParticleSystemGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::PARTICLE]->SetSize(ImVec2(0.f, 300.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::TILEMAP] = new CTileMapGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::TILEMAP]->SetSize(ImVec2(0.f, 180.f));

    m_arrComponentGUI[(UINT)COMPONENT_TYPE::NAVIMESH] = new CNaviMeshGUI;
    m_arrComponentGUI[(UINT)COMPONENT_TYPE::NAVIMESH]->SetSize(ImVec2(0.f, 180.f));

    m_arrScriptGUI[(UINT)SCRIPT_TYPE::MONSTERSCRIPT] = new CMonsterScriptGUI;
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::MONSTERSCRIPT]->SetSize(ImVec2(0.f, 180.f));
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::MONSTERSCRIPT]->init();

    m_arrScriptGUI[(UINT)SCRIPT_TYPE::IGITEMOBJSCRIPT] = new CIGItemScriptGUI;
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::IGITEMOBJSCRIPT]->SetSize(ImVec2(0.f, 180.f));
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::IGITEMOBJSCRIPT]->init();

    m_arrScriptGUI[(UINT)SCRIPT_TYPE::EFFECTSCRIPT] = new CEffectScriptGUI;
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::EFFECTSCRIPT]->SetSize(ImVec2(0.f, 180.f));
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::EFFECTSCRIPT]->init();

    m_arrScriptGUI[(UINT)SCRIPT_TYPE::FONTUISCRIPT] = new CFontScriptGUI;
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::FONTUISCRIPT]->SetSize(ImVec2(0.f, 180.f));

    m_arrScriptGUI[(UINT)SCRIPT_TYPE::ITEMBOXUISCRIPT] = new CItemBoxScriptGUI;
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::ITEMBOXUISCRIPT]->SetSize(ImVec2(0.f, 180.f));

    m_arrScriptGUI[(UINT)SCRIPT_TYPE::SKILLUISCRIPT] = new CSkillScriptGUI;
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::SKILLUISCRIPT]->SetSize(ImVec2(0.f, 180.f));

    m_arrScriptGUI[(UINT)SCRIPT_TYPE::BARUISCRIPT] = new CBarScriptGUI;
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::BARUISCRIPT]->SetSize(ImVec2(0.f, 180.f));

    m_arrScriptGUI[(UINT)SCRIPT_TYPE::TRACKINGUISCRIPT] = new CTrackingUIScriptGUI;
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::TRACKINGUISCRIPT]->SetSize(ImVec2(0.f, 180.f));

    m_arrScriptGUI[(UINT)SCRIPT_TYPE::PICTUREUISCRIPT] = new CPictureScriptGUI;
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::PICTUREUISCRIPT]->SetSize(ImVec2(0.f, 180.f));

    m_arrScriptGUI[(UINT)SCRIPT_TYPE::BUTTONUISCRIPT] = new CButtonScriptGUI;
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::BUTTONUISCRIPT]->SetSize(ImVec2(0.f, 180.f));

    m_arrScriptGUI[(UINT)SCRIPT_TYPE::SWITCHUISCRIPT] = new CSwitchScriptGUI;
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::SWITCHUISCRIPT]->SetSize(ImVec2(0.f, 180.f));

    m_arrScriptGUI[(UINT)SCRIPT_TYPE::AIPOINTSCRIPT] = new CAIPointScriptGUI;
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::AIPOINTSCRIPT]->init();
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::AIPOINTSCRIPT]->SetSize(ImVec2(0.f, 180.f));

    m_arrResourceGUI[(UINT)RESOURCE_TYPE::MATERIAL] = new CMaterialGUI;
    m_arrResourceGUI[(UINT)RESOURCE_TYPE::TEXTURE] = new CTextureGUI;
    m_arrResourceGUI[(UINT)RESOURCE_TYPE::MESH] = new CMeshGUI;
    m_arrResourceGUI[(UINT)RESOURCE_TYPE::PREFAB] = new CPrefabGUI;
    m_arrResourceGUI[(UINT)RESOURCE_TYPE::GRAPHICS] = new CGraphicsShaderGUI;
    m_arrResourceGUI[(UINT)RESOURCE_TYPE::COMPUTE] = new CComputeShaderGUI;
    m_arrResourceGUI[(UINT)RESOURCE_TYPE::MESHDATA] = new CMeshDataGUI;

    m_arrScriptGUI[(UINT)SCRIPT_TYPE::MONSTERCREATESCRIPT] = new CMonsterCreatGUI;
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::MONSTERCREATESCRIPT]->SetSize(ImVec2(0.f, 180.f));
    m_arrScriptGUI[(UINT)SCRIPT_TYPE::MONSTERCREATESCRIPT]->init();
}

void CInspectorGUI::update()
{
    CScene* curScene = CSceneManager::GetInst()->GetCurScene();

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

    ImGui::Begin(GetName().c_str(), &IsCancelButton());

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

void CInspectorGUI::update_object()
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
        CEventManager::GetInst()->EventOccur();
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
        CLayer* layer = CSceneManager::GetInst()->GetCurScene()->GetLayer(curLayerIndex);
        layer->DeregisterAsParent(m_targetObject);

        CScript::CreateObject(m_targetObject, m_targetObject->Transform()->GetLocalPos(), layerIndex);
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
    bool b = m_targetObject->IsOn();
    if (ImGui::Checkbox("ObjectOn", &b))  m_targetObject->SetOnOff(b);
    ImGui::SameLine(100.f, 5.f);
    ImGui::SetNextItemWidth(180);

   
   

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

void CInspectorGUI::update_resource()
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
        CResourceManager::GetInst()->SubRes(m_targetResource->GetKey(), m_targetResource->GetType());
        m_targetResource->SetKey(targetName);
        CResourceManager::GetInst()->AddRes(m_targetResource->GetKey(), m_targetResource, m_targetResource->GetType());
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

void CInspectorGUI::AddComponentToObj()
{
    CComponent* component = nullptr;

    if (ImGui::MenuItem("Collider2D##Add Component"))
    {
        component = new CCollider2D;
        if (!m_targetObject->GetComponent(component->GetType()))
            m_targetObject->AddComponent(component);
        else
            delete component;
    }
    if (ImGui::MenuItem("Collider3D##Add Component"))
    {
        component = new CCollider3D;
        if (!m_targetObject->GetComponent(component->GetType()))
            m_targetObject->AddComponent(component);
        else
            delete component;
    }
    if (ImGui::MenuItem("Animator2D##Add Component"))
    {
        component = new CAnimator2D;
        if (!m_targetObject->GetComponent(component->GetType()))
            m_targetObject->AddComponent(component);
        else
            delete component;
    }
    if (ImGui::MenuItem("Animator3D##Add Component"))
    {
        component = new CAnimator3D;
        if (!m_targetObject->GetComponent(component->GetType()))
            m_targetObject->AddComponent(component);
        else
            delete component;
    }
    if (ImGui::MenuItem("Light2D##Add Component"))
    {
        component = new CLight2D;
        if (!m_targetObject->GetComponent(component->GetType()))
            m_targetObject->AddComponent(component);
        else
            delete component;
    }
    if (ImGui::MenuItem("Light3D##Add Component"))
    {
        component = new CLight3D;
        if (!m_targetObject->GetComponent(component->GetType()))
            m_targetObject->AddComponent(component);
        else
            delete component;
    }
    if (ImGui::MenuItem("Camera##Add Component"))
    {
        component = new CCamera;
        if (!m_targetObject->GetComponent(component->GetType()))
            m_targetObject->AddComponent(component);
        else
            delete component;
    }
    if (ImGui::MenuItem("Terrain##Add Component"))
    {
        component = new CTerrain;
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

        component = new CMeshRender;
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

        component = new CTileMap;
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

        component = new CParticleSystem;
        if (!m_targetObject->GetComponent(component->GetType()))
            m_targetObject->AddComponent(component);
        else
            delete component;
    }
    if (ImGui::BeginMenu("Scripts##Add Component"))
    {
        vector<wstring> vecScriptsNameByWString;
        vector<string> vecScriptsName;
        CScriptManager::GetScriptInfo(vecScriptsNameByWString);

        for (size_t i = 0; i < vecScriptsNameByWString.size(); ++i)
        {
            vecScriptsName.push_back(WStringToString(vecScriptsNameByWString[i]));
        }

        for (size_t i = 0; i < vecScriptsName.size(); ++i)
        {
            vecScriptsName[i] += "##Add Component";
            if (ImGui::MenuItem(vecScriptsName[i].c_str()))
            {
                CScript* script = nullptr;
                script = CScriptManager::GetScript(vecScriptsNameByWString[i]);

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

void CInspectorGUI::SaveMaterial()
{
    ImGui::SameLine();

    if (ImGui::Button("Save##Material"))
    {
        OPENFILENAME ofn = {};       // common dialog box structure
        wchar_t szFile[260] = { 0 };       // if using TCHAR macros

        wstring strMapFilePath = CPathManager::GetInst()->GetContentPath();
        strMapFilePath += L"mtrl";

        // Initialize OPENFILENAME	
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = CCore::GetInst()->GetWindowHandle();
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = L"All\0*.*\0머터리얼\0*.mtrl\0";
        ofn.nFilterIndex = 2;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = strMapFilePath.c_str(); // 탐색창 초기 경로
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetSaveFileName(&ofn)) // DoModal (modal window)
        {
            wstring strOutput = ofn.lpstrFile;
            wstring contentPath = CPathManager::GetInst()->GetContentPath();

            wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
            m_targetResource->SetRelativePath(relativePath);
            m_targetResource->Save(relativePath);
        }
    }
}

void CInspectorGUI::SetTargetObject(CGameObject* _target)
{
    m_targetObject = _target;
    m_mode = INFO_MODE::GAMEOBJECT;

    for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        if (nullptr == m_arrComponentGUI[i])
            continue;

        m_arrComponentGUI[i]->SetTarget(m_targetObject);
    }

    for (UINT i = 0; i < (UINT)SCRIPT_TYPE::END; i++)
    {
        if (nullptr == m_arrScriptGUI[i])
            continue;

        m_arrScriptGUI[i]->SetTarget(m_targetObject);
    }
}

void CInspectorGUI::SetTargetResource(CRes* _target)
{
    m_targetResource = _target;
    m_mode = INFO_MODE::RESOURCE;

    RESOURCE_TYPE type = _target->GetType();

    if (nullptr == m_arrResourceGUI[(UINT)type])
        return;

    m_arrResourceGUI[(UINT)type]->SetTarget(m_targetResource);
}

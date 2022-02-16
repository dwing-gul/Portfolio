#include "pch.h"
#include "CMenuGUI.h"
#include <Engine/CSceneManager.h>
#include <Engine/CPathManager.h>
#include <Engine/CTransform.h>
#include <Engine/CCore.h>
#include <Engine/CScript.h>
#include "CSceneSaveLoad.h"
#include "CImGuiManager.h"

UINT g_material = 0;
UINT g_object = 0;

CMenuGUI::CMenuGUI() :
    m_bPlay(true),
    m_bPause(true),
    m_bStop(true)
{
    SetName("Menu");
}

CMenuGUI::~CMenuGUI()
{
}

void CMenuGUI::init()
{
}

void CMenuGUI::update()
{
    ActiveCheck();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save Scene") && (m_bPlay && !m_bPause))
            {
                OPENFILENAME ofn = {};       // common dialog box structure
                wchar_t szFile[260] = { 0 };       // if using TCHAR macros

                wstring strMapFilePath = CPathManager::GetInst()->GetContentPath();
                strMapFilePath += L"scene";

                // Initialize OPENFILENAME	
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = CCore::GetInst()->GetWindowHandle();
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = L"All\0*.*\0��\0*.scene\0";
                ofn.nFilterIndex = 2;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = strMapFilePath.c_str(); // Ž��â �ʱ� ���
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                if (GetSaveFileName(&ofn)) // DoModal (modal window)
                {
                    wstring strOutput = ofn.lpstrFile;
                    wstring contentPath = CPathManager::GetInst()->GetContentPath();

                    wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
                    CSceneSaveLoad::SaveScene(CSceneManager::GetInst()->GetCurScene(), relativePath);
                }
            }

            if (ImGui::MenuItem("Load Scene") && (m_bPlay && !m_bPause))
            {
                OPENFILENAME ofn = {};       // common dialog box structure
                wchar_t szFile[260] = { 0 };       // if using TCHAR macros

                wstring strMapFilePath = CPathManager::GetInst()->GetContentPath();
                strMapFilePath += L"scene";

                // Initialize OPENFILENAME	
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = CCore::GetInst()->GetWindowHandle();
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = L"All\0*.*\0��\0*.scene\0";
                ofn.nFilterIndex = 2;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = strMapFilePath.c_str(); // Ž��â �ʱ� ���
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                if (GetOpenFileName(&ofn)) // DoModal (modal window)
                {
                    wstring strOutput = ofn.lpstrFile;
                    wstring contentPath = CPathManager::GetInst()->GetContentPath();

                    wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
                    CSceneSaveLoad::LoadScene(relativePath);
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Menu"))
        {
            if (ImGui::MenuItem("DemoMenu") && (m_bPlay && !m_bPause))
            {
                CImGuiManager::GetInst()->SetDemo(true);
            }

            if (ImGui::MenuItem("Open Tool") && (m_bPlay && !m_bPause))
            {
                CGUI* inspector = CImGuiManager::GetInst()->FindGUI("Inspector");
                inspector->SetCancelButton(true);
                CGUI* hierarchy = CImGuiManager::GetInst()->FindGUI("Hierarchy");
                hierarchy->SetCancelButton(true);
                CGUI* resource = CImGuiManager::GetInst()->FindGUI("Resource");
                resource->SetCancelButton(true);
            }

            if (ImGui::MenuItem("Inspector") && (m_bPlay && !m_bPause))
            {
                CGUI* inspector = CImGuiManager::GetInst()->FindGUI("Inspector");
                inspector->SetCancelButton(true);
            }

            if (ImGui::MenuItem("Hierarchy") && (m_bPlay && !m_bPause))
            {
                CGUI* hierarchy = CImGuiManager::GetInst()->FindGUI("Hierarchy");
                hierarchy->SetCancelButton(true);
            }

            if (ImGui::MenuItem("Resource") && (m_bPlay && !m_bPause))
            {
                CGUI* resource = CImGuiManager::GetInst()->FindGUI("Resource");
                resource->SetCancelButton(true);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Scene"))
        {
            if (ImGui::MenuItem("Play", nullptr, nullptr, m_bPlay)) 
            {
                ChangeSceneMode(SCENE_MODE::PLAY);
                CSceneSaveLoad::SaveScene(CSceneManager::GetInst()->GetCurScene(), L"scene\\temp.scene");
            }
            if (ImGui::MenuItem("Pause", nullptr, nullptr, m_bPause))
            {
                ChangeSceneMode(SCENE_MODE::PAUSE);
            }
            if (ImGui::MenuItem("Stop", nullptr, nullptr, m_bStop))
            {
                ChangeSceneMode(SCENE_MODE::STOP);
                CSceneSaveLoad::LoadScene(L"scene\\temp.scene");
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Object"))
        {
            if (ImGui::MenuItem("Empty GameObject") && (m_bPlay && !m_bPause))
            {
                CreateEmptyObject();
            }

            if (ImGui::MenuItem("Empty Material") && (m_bPlay && !m_bPause))
            {
                CreateEmptyMaterial();
            }

            if (ImGui::MenuItem("Load Material") && (m_bPlay && !m_bPause))
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
                ofn.lpstrFilter = L"All\0*.*\0���͸���\0*.mtrl\0";
                ofn.nFilterIndex = 2;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = strMapFilePath.c_str(); // Ž��â �ʱ� ���
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                if (GetOpenFileName(&ofn)) // DoModal (modal window)
                {
                    wstring strOutput = ofn.lpstrFile;

                    CMaterial* material = new CMaterial;
                    material->Load(strOutput);

                    if (nullptr != CResourceManager::GetInst()->FindRes<CMaterial>(material->GetKey()))
                    {
                        delete material;
                        ImGui::EndMenu();
                        ImGui::EndMainMenuBar();
                        return;
                    }

                    CResourceManager::GetInst()->AddRes<CMaterial>(material->GetKey(), material);
                }
            }

            if (ImGui::MenuItem("Load Prefab") && (m_bPlay && !m_bPause))
            {
                OPENFILENAME ofn = {};       // common dialog box structure
                wchar_t szFile[260] = { 0 };       // if using TCHAR macros

                wstring strMapFilePath = CPathManager::GetInst()->GetContentPath();
                strMapFilePath += L"prefab";

                // Initialize OPENFILENAME	
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = CCore::GetInst()->GetWindowHandle();
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = L"All\0*.*\0������\0*.prefab\0";
                ofn.nFilterIndex = 2;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = strMapFilePath.c_str(); // Ž��â �ʱ� ���
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                if (GetOpenFileName(&ofn)) // DoModal (modal window)
                {
                    wstring strOutput = ofn.lpstrFile;
                    wstring contentPath = CPathManager::GetInst()->GetContentPath();

                    wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
                    wstring key;
                    key = relativePath.substr(0, relativePath.find_last_of(L"."));

                    while (true)
                    {
                        if (wstring::npos == key.find(L"\\"))
                            break;

                        key = key.substr(key.find(L"\\") + 1);
                    }

                    CResourceManager::GetInst()->Load<CPrefab>(key, relativePath);
                }
            }

            if (ImGui::MenuItem("Texture") && (m_bPlay && !m_bPause))
            {
                OPENFILENAME ofn = {};       // common dialog box structure
                wchar_t szFile[260] = { 0 };       // if using TCHAR macros

                wstring strMapFilePath = CPathManager::GetInst()->GetContentPath();
                strMapFilePath += L"texture";

                // Initialize OPENFILENAME	
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = CCore::GetInst()->GetWindowHandle();
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = L"All\0*.*\0PNG\0*.png\0JPG\0*.jpg\0BMP\0*.bmp\0";
                ofn.nFilterIndex = 2;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = strMapFilePath.c_str(); // Ž��â �ʱ� ���
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                if (GetOpenFileName(&ofn)) // DoModal (modal window)
                {
                    wstring strOutput = ofn.lpstrFile;
                    wstring contentPath = CPathManager::GetInst()->GetContentPath();

                    wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
                    wstring key;
                    key = relativePath.substr(0, relativePath.find_last_of(L"."));

                    while (true)
                    {
                        if (wstring::npos == key.find(L"\\"))
                            break;

                        key = key.substr(key.find(L"\\") + 1);
                    }

                    CResourceManager::GetInst()->Load<CTexture>(key, relativePath);
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Tool"))
        {
            if (ImGui::MenuItem("MRT View"))
            {
                CGUI* gui = CImGuiManager::GetInst()->FindGUI("MRTViewGUI");
                gui->SetCancelButton(true);
                gui->Activate();
            }

            if (ImGui::MenuItem("Tool Cam"))
            {
                CGUI* gui = CImGuiManager::GetInst()->FindGUI("ToolCamGUI");
                gui->SetCancelButton(true);
                gui->Activate();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Animation Tool"))
        {
            if (ImGui::MenuItem("Animation2D Tool"))
            {
                CGUI* gui = CImGuiManager::GetInst()->FindGUI("CAnimation2DTool");
                gui->SetCancelButton(true);
                gui->Activate();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void CMenuGUI::ActiveCheck()
{
    SCENE_MODE curMode = CSceneManager::GetInst()->GetSceneMode();

    switch (curMode)
    {
    case SCENE_MODE::PLAY:
        m_bPlay = false;
        m_bPause = true;
        m_bStop = true;
        break;
    case SCENE_MODE::PAUSE:
        m_bPlay = true;
        m_bPause = false;
        m_bStop = true;
        break;
    case SCENE_MODE::STOP:
        m_bPlay = true;
        m_bPause = false;
        m_bStop = false;
        break;
    }
}

void CMenuGUI::CreateEmptyMaterial()
{
    wchar_t buffer[255] = L"";

    while (true)
    {
        swprintf_s(buffer, L"material %d", g_material++);

        CMaterial* material = CResourceManager::GetInst()->FindRes<CMaterial>(buffer).Get();

        if (!material)
            break;
    }
    
    wstring key = buffer;
    wstring relativePath = L"material\\";
    relativePath += key;
    relativePath += L".material";

    CMaterial* newMaterial = new CMaterial;
    newMaterial->SetKey(key);
    newMaterial->SetRelativePath(relativePath);

    CResourceManager::GetInst()->AddRes<CMaterial>(key, newMaterial);
}

void CMenuGUI::CreateEmptyObject()
{
    wchar_t buffer[255] = L"";

    while (true)
    {
        swprintf_s(buffer, L"object %d", g_object++);
        CGameObject* object = CSceneManager::GetInst()->FindObjByName(buffer);

        if (!object)
            break;
    }

    CGameObject* newObj = new CGameObject;
    newObj->SetName(buffer);
    newObj->AddComponent(new CTransform);
    CScript::CreateObject(newObj, Vector3(0.f, 0.f, 0.f));
}

#include "pch.h"
#include "MenuGUI.h"
#include "SceneSaveLoad.h"
#include "ImGuiManager.h"
#include "PartyManager.h"
#include "SceneConverter.h"
#include "SceneConfig.h"
#include <Engine/PathManager.h>
#include <Engine/SceneManager.h>
#include <Engine/Script.h>
#include <Engine/Transform.h>
#include <Engine/Core.h>

UINT g_material = 0;
UINT g_object = 0;

MenuGUI::MenuGUI() :
    m_bPlay(true),
    m_bPause(true),
    m_bStop(true)
{
    SetName("Menu");
}

MenuGUI::~MenuGUI()
{
}

void MenuGUI::init()
{
}

void MenuGUI::update()
{
    ActiveCheck();

    if (SceneConverter::GetInst()->GetCurSceneConfig()->GetName() != "ToolScene")
    {
        return;
    }

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save Scene") && (m_bPlay && !m_bPause))
            {
                OPENFILENAME ofn = {};       // common dialog box structure
                wchar_t szFile[260] = { 0 };       // if using TCHAR macros

                wstring strMapFilePath = PathManager::GetInst()->GetContentPath();
                strMapFilePath += L"scene";

                // Initialize OPENFILENAME	
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = Core::GetInst()->GetWindowHandle();
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = L"All\0*.*\0¾À\0*.scene\0";
                ofn.nFilterIndex = 2;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = strMapFilePath.c_str(); // Å½»öÃ¢ ÃÊ±â °æ·Î
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                if (GetSaveFileName(&ofn)) // DoModal (modal window)
                {
                    wstring strOutput = ofn.lpstrFile;
                    wstring contentPath = PathManager::GetInst()->GetContentPath();

                    wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
                    SceneSaveLoad::SaveScene(SceneManager::GetInst()->GetCurScene(), relativePath);
                }
            }

            if (ImGui::MenuItem("Load Scene") && (m_bPlay && !m_bPause))
            {
                OPENFILENAME ofn = {};       // common dialog box structure
                wchar_t szFile[260] = { 0 };       // if using TCHAR macros

                wstring strMapFilePath = PathManager::GetInst()->GetContentPath();
                strMapFilePath += L"scene";

                // Initialize OPENFILENAME	
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = Core::GetInst()->GetWindowHandle();
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = L"All\0*.*\0¾À\0*.scene\0";
                ofn.nFilterIndex = 2;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = strMapFilePath.c_str(); // Å½»öÃ¢ ÃÊ±â °æ·Î
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                if (GetOpenFileName(&ofn)) // DoModal (modal window)
                {
                    wstring strOutput = ofn.lpstrFile;
                    wstring contentPath = PathManager::GetInst()->GetContentPath();

                    wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
                    SceneSaveLoad::LoadScene(relativePath);
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Menu"))
        {
            if (ImGui::MenuItem("DemoMenu") && (m_bPlay && !m_bPause))
            {
                ImGuiManager::GetInst()->SetDemo(true);
            }

            if (ImGui::MenuItem("Open Tool") && (m_bPlay && !m_bPause))
            {
                GUI* inspector = ImGuiManager::GetInst()->FindGUI("Inspector");
                inspector->SetCancelButton(true);
                GUI* hierarchy = ImGuiManager::GetInst()->FindGUI("Hierarchy");
                hierarchy->SetCancelButton(true);
                GUI* resource = ImGuiManager::GetInst()->FindGUI("Resource");
                resource->SetCancelButton(true);
            }

            if (ImGui::MenuItem("Inspector") && (m_bPlay && !m_bPause))
            {
                GUI* inspector = ImGuiManager::GetInst()->FindGUI("Inspector");
                inspector->SetCancelButton(true);
            }

            if (ImGui::MenuItem("Hierarchy") && (m_bPlay && !m_bPause))
            {
                GUI* hierarchy = ImGuiManager::GetInst()->FindGUI("Hierarchy");
                hierarchy->SetCancelButton(true);
            }

            if (ImGui::MenuItem("Resource") && (m_bPlay && !m_bPause))
            {
                GUI* resource = ImGuiManager::GetInst()->FindGUI("Resource");
                resource->SetCancelButton(true);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Scene"))
        {
            if (ImGui::MenuItem("Play", nullptr, nullptr, m_bPlay))
            {
                PartyManager::GetInst()->SetBattle(false);
                ChangeSceneMode(SCENE_MODE::PLAY);
                SceneSaveLoad::SaveScene(SceneManager::GetInst()->GetCurScene(), L"scene\\temp.scene");
            }
            if (ImGui::MenuItem("Pause", nullptr, nullptr, m_bPause))
            {
                ChangeSceneMode(SCENE_MODE::PAUSE);
            }
            if (ImGui::MenuItem("Stop", nullptr, nullptr, m_bStop))
            {
                ChangeSceneMode(SCENE_MODE::STOP);
                SceneSaveLoad::LoadScene(L"scene\\temp.scene");
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Create"))
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

                wstring strMapFilePath = PathManager::GetInst()->GetContentPath();
                strMapFilePath += L"material";

                // Initialize OPENFILENAME	
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = Core::GetInst()->GetWindowHandle();
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = L"All\0*.*\0¸ÓÅÍ¸®¾ó\0*.material\0";
                ofn.nFilterIndex = 2;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = strMapFilePath.c_str(); // Å½»öÃ¢ ÃÊ±â °æ·Î
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                if (GetOpenFileName(&ofn)) // DoModal (modal window)
                {
                    wstring strOutput = ofn.lpstrFile;

                    Material* material = new Material;
                    material->Load(strOutput);

                    if (nullptr != ResourceManager::GetInst()->FindRes<Material>(material->GetKey()))
                    {
                        delete material;
                        ImGui::EndMenu();
                        ImGui::EndMainMenuBar();
                        return;
                    }

                    ResourceManager::GetInst()->AddRes<Material>(material->GetKey(), material);
                }
            }

            if (ImGui::MenuItem("Load Prefab") && (m_bPlay && !m_bPause))
            {
                OPENFILENAME ofn = {};       // common dialog box structure
                wchar_t szFile[260] = { 0 };       // if using TCHAR macros

                wstring strMapFilePath = PathManager::GetInst()->GetContentPath();
                strMapFilePath += L"prefab";

                // Initialize OPENFILENAME	
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = Core::GetInst()->GetWindowHandle();
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = L"All\0*.*\0ÇÁ¸®ÆÕ\0*.prefab\0";
                ofn.nFilterIndex = 2;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = strMapFilePath.c_str(); // Å½»öÃ¢ ÃÊ±â °æ·Î
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                if (GetOpenFileName(&ofn)) // DoModal (modal window)
                {
                    wstring strOutput = ofn.lpstrFile;
                    wstring contentPath = PathManager::GetInst()->GetContentPath();

                    wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
                    wstring key;
                    key = relativePath.substr(0, relativePath.find_last_of(L"."));

                    while (true)
                    {
                        if (wstring::npos == key.find(L"\\"))
                            break;

                        key = key.substr(key.find(L"\\") + 1);
                    }

                    ResourceManager::GetInst()->Load<Prefab>(key, relativePath);
                }
            }

            if (ImGui::MenuItem("Texture") && (m_bPlay && !m_bPause))
            {
                OPENFILENAME ofn = {};       // common dialog box structure
                wchar_t szFile[260] = { 0 };       // if using TCHAR macros

                wstring strMapFilePath = PathManager::GetInst()->GetContentPath();
                strMapFilePath += L"texture";

                // Initialize OPENFILENAME	
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = Core::GetInst()->GetWindowHandle();
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = L"All\0*.*\0PNG\0*.png\0JPG\0*.jpg\0BMP\0*.bmp\0";
                ofn.nFilterIndex = 2;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = strMapFilePath.c_str(); // Å½»öÃ¢ ÃÊ±â °æ·Î
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                if (GetOpenFileName(&ofn)) // DoModal (modal window)
                {
                    wstring strOutput = ofn.lpstrFile;
                    wstring contentPath = PathManager::GetInst()->GetContentPath();

                    wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
                    wstring key;
                    key = relativePath.substr(0, relativePath.find_last_of(L"."));

                    while (true)
                    {
                        if (wstring::npos == key.find(L"\\"))
                            break;

                        key = key.substr(key.find(L"\\") + 1);
                    }

                    ResourceManager::GetInst()->Load<Texture>(key, relativePath);
                }
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void MenuGUI::ActiveCheck()
{
    SCENE_MODE curMode = SceneManager::GetInst()->GetSceneMode();

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

void MenuGUI::CreateEmptyMaterial()
{
    wchar_t buffer[255] = L"";

    while (true)
    {
        swprintf_s(buffer, L"material %d", g_material++);
        Material* material = ResourceManager::GetInst()->FindRes<Material>(buffer).Get();

        if (!material)
            break;
    }

    wstring key = buffer;
    wstring relativePath = L"material\\";
    relativePath += key;
    relativePath += L".material";

    Material* newMaterial = new Material;
    newMaterial->SetKey(key);
    newMaterial->SetRelativePath(relativePath);
    ResourceManager::GetInst()->AddRes<Material>(key, newMaterial);
}

void MenuGUI::CreateEmptyObject()
{
    wchar_t buffer[255] = L"";

    while (true)
    {
        swprintf_s(buffer, L"object %d", g_object++);
        GameObject* object = SceneManager::GetInst()->FindObjByName(buffer);

        if (!object)
            break;
    }

    GameObject* newObj = new GameObject;
    newObj->SetName(buffer);
    newObj->AddComponent(new Transform);
    Script::CreateObject(newObj, Vector3(0.f, 0.f, 0.f));
}

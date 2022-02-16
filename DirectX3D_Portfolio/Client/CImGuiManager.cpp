#include "pch.h"
#include "CImGuiManager.h"

#include <Engine/CCore.h>
#include <Engine/CDevice.h>
#include <Engine/CKeyManager.h>
#include <Engine/CSceneManager.h>
#include <Engine/CPathManager.h>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "CResourceManagerGUI.h"
#include "CInspectorGUI.h"
#include "CHierarchyGUI.h"
#include "CListGUI.h"
#include "CMenuGUI.h"
#include "CMRTViewGUI.h"
#include "CToolCamGUI.h"
#include "CAnimation2DTool.h"

CImGuiManager::CImGuiManager() :
    m_bDemo(false)
{
}

CImGuiManager::~CImGuiManager()
{
    Safe_Delete_Map(m_mapGUI);
}

void CImGuiManager::init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.
    wstring filePath = CPathManager::GetInst()->GetContentPath();
    filePath += L"fonts\\ArialUni.ttf";
    string buffer;
    buffer.assign(filePath.begin(), filePath.end());
    io.Fonts->AddFontFromFileTTF(buffer.c_str(), 16.f, NULL, io.Fonts->GetGlyphRangesKorean());

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(CCore::GetInst()->GetWindowHandle());
    ImGui_ImplDX11_Init(DEVICE, CONTEXT);

    CreateGUI();
}

void CImGuiManager::process()
{
    update();
    render();

    if(ImGui::GetIO().WantCaptureMouse)
        CKeyManager::GetInst()->SetMainWinMouseFocus(false);
    else
        CKeyManager::GetInst()->SetMainWinMouseFocus(true);
}

void CImGuiManager::update()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

   /* if (KEY_TAP(KEY::M))
        m_bDemo ? m_bDemo = false : m_bDemo = true;*/

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (m_bDemo)
        ImGui::ShowDemoWindow(&m_bDemo);

    for (const auto& pair : m_mapGUI)
    {
        if (pair.second->IsActive())
        {
            pair.second->update();
        }
    }
}

void CImGuiManager::render()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void CImGuiManager::CreateGUI()
{
    CGUI* gui = new CMenuGUI;
    gui->init();
    m_mapGUI.insert(make_pair(gui->GetName(), gui));

    gui = new CHierarchyGUI;
    gui->init();
    m_mapGUI.insert(make_pair(gui->GetName(), gui));

    gui = new CResourceManagerGUI;
    gui->init();
    m_mapGUI.insert(make_pair(gui->GetName(), gui));

    gui = new CInspectorGUI;
    gui->init();
    m_mapGUI.insert(make_pair(gui->GetName(), gui));

    gui = new CListGUI;
    gui->init();
    m_mapGUI.insert(make_pair(gui->GetName(), gui));

    gui = new CMRTViewGUI;
    gui->init();
    m_mapGUI.insert(make_pair(gui->GetName(), gui));

    gui = new CToolCamGUI;
    gui->init();
    m_mapGUI.insert(make_pair(gui->GetName(), gui));

    gui = new CAnimation2DTool;
    gui->init();
    m_mapGUI.insert(make_pair(gui->GetName(), gui));
}

CGUI* CImGuiManager::FindGUI(const string& _key)
{
    map<string, CGUI*>::iterator iter = m_mapGUI.find(_key);

    if (iter == m_mapGUI.end())
        return nullptr;

    return iter->second;
}

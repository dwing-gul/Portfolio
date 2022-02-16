#include "pch.h"
#include "ImGuiManager.h"
#include <Engine/Core.h>
#include <Engine/Device.h>
#include <Engine/SceneManager.h>
#include <Engine/PathManager.h>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "ResourceManagerGUI.h"
#include "InspectorGUI.h"
#include "HierarchyGUI.h"
#include "ListGUI.h"
#include "MenuGUI.h"

ImGuiManager::ImGuiManager() :
	m_bDemo(false)
{
}

ImGuiManager::~ImGuiManager()
{
	Safe_Delete_Map(m_mapGUI);
}

void ImGuiManager::init()
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
    wstring filePath = PathManager::GetInst()->GetContentPath();
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
    ImGui_ImplWin32_Init(Core::GetInst()->GetWindowHandle());
    ImGui_ImplDX11_Init(DEVICE.Get(), CONTEXT.Get());

    CreateGUI();
}

void ImGuiManager::progress()
{
    update();
    render();
}

void ImGuiManager::update()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (m_bDemo)
        ImGui::ShowDemoWindow(&m_bDemo);

    for (const auto& pair : m_mapGUI)
        pair.second->update();
}

void ImGuiManager::render()
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

void ImGuiManager::CreateGUI()
{
    GUI* gui = new MenuGUI;
    gui->init();
    m_mapGUI.insert(make_pair(gui->GetName(), gui));

    gui = new HierarchyGUI;
    gui->init();
    m_mapGUI.insert(make_pair(gui->GetName(), gui));

    gui = new ResourceManagerGUI;
    gui->init();
    m_mapGUI.insert(make_pair(gui->GetName(), gui));

    gui = new InspectorGUI;
    gui->init();
    m_mapGUI.insert(make_pair(gui->GetName(), gui));

    gui = new ListGUI;
    gui->init();
    m_mapGUI.insert(make_pair(gui->GetName(), gui));
}

GUI* ImGuiManager::FindGUI(const string& _key)
{
    map<string, GUI*>::iterator iter = m_mapGUI.find(_key);

    if (iter == m_mapGUI.end())
        return nullptr;

	return iter->second;
}

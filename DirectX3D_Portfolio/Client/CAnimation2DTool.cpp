#include "pch.h"
#include "CAnimation2DTool.h"

#include <Engine\CTexture.h>
#include <Engine\CCore.h>
#include <Engine\CAnimator2D.h>
#include <Engine\CAnimation2D.h>
#include <Engine\CScript.h>

#include "CImGuiManager.h"

#include "CParamGUI.h"

CAnimation2DTool::CAnimation2DTool()
    : m_pTex(nullptr)
    , m_fAccTime(0)
    , m_bPlay(false)
    , m_bRepeat(false)
{
    SetName("CAnimation2DTool");
}

CAnimation2DTool::~CAnimation2DTool()
{
}


void CAnimation2DTool::init()
{
    Listinit();
    Deactivate();
}

void CAnimation2DTool::update()
{

    ImGui::Begin(GetName().c_str(), &IsCancelButton());

    static int item_current_idx = 0;
    static ImGuiComboFlags flags = 0;
    static int iAnimIndexSize = 0;

    if (!IsCancelButton())
    {
        Deactivate();
    }



    if (ImGui::BeginCombo("##Texture", m_vecTexList[item_current_idx].c_str(), flags))
    {
        for (int n = 0; n < m_vecTexList.size(); n++)
        {
            const bool is_selected = (item_current_idx == n);
            if (ImGui::Selectable(m_vecTexList[n].c_str(), is_selected))
            {
                item_current_idx = n;
                m_strTexName = wstring(m_vecTexList[n].begin(), m_vecTexList[n].end());
                m_pTex = CResourceManager::GetInst()->FindRes<CTexture>(m_strTexName).Get();
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    // 텍스쳐	
    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  
    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
    Vec2 vResolution = {};

    ImTextureID tex_id = 0;
    if (nullptr != m_pTex)
    {
        tex_id = (ImTextureID)(m_pTex->GetSRV().Get());
        vResolution = Vec2(m_pTex->Width(), m_pTex->Height());
    }

    if (m_pTex)
        ImGui::Image(tex_id, ImVec2(m_pTex->Width(), m_pTex->Height()), uv_min, uv_max, tint_col, border_col);

    ImGui::Text("FramSize"); ImGui::SameLine();
    if (ImGui::InputInt("##FramSize", &iAnimIndexSize))
    {
        if (m_vecAnimInfo.size() < iAnimIndexSize)
        {
            for (size_t i = m_vecAnimInfo.size(); i < iAnimIndexSize; ++i)
            {
                tAnimFrame info = {};
                m_vecAnimInfo.push_back(info);
            }
        }
        else if (m_vecAnimInfo.size() > iAnimIndexSize)
        {
            for (size_t i = m_vecAnimInfo.size(); i > iAnimIndexSize; --i)
            {
                m_vecAnimInfo.pop_back();
            }
        }
    }

    static int iFrmID = 0;
    Vec2 vLeftTop = {};
    Vec2 vFrmSize = {};

    if (0 < m_vecAnimInfo.size())
    {
        ImGui::Text("FrmID   "); ImGui::SameLine(); ImGui::InputInt("##FrmID", &iFrmID);
        if (0 > iFrmID) iFrmID = 0; else if (m_vecAnimInfo.size() <= iFrmID) iFrmID = (int)m_vecAnimInfo.size() - 1;

        ImGui::Text("FrmSize "); ImGui::SameLine();

        ImGui::InputFloat2("##FrmSize", (float*)&m_vecAnimInfo[iFrmID].frameSize);

        ImGui::Text("LeftTop "); ImGui::SameLine(); ImGui::InputFloat2("##LeftTop", (float*)&m_vecAnimInfo[iFrmID].leftTop);

        ImGui::Text("Offset  "); ImGui::SameLine(); ImGui::InputFloat2("##Offset", (float*)&m_vecAnimInfo[iFrmID].offset);
        ImGui::Text("Duration"); ImGui::SameLine();
        ImGui::InputFloat("##Duration", &m_vecAnimInfo[iFrmID].duration);

        ImVec2 uv_0 = ImVec2(m_vecAnimInfo[iFrmID].leftTop.x / vResolution.x, m_vecAnimInfo[iFrmID].leftTop.y / vResolution.y);
        ImVec2 uv_1 = ImVec2(uv_0.x + m_vecAnimInfo[iFrmID].frameSize.x / vResolution.x, uv_0.y + m_vecAnimInfo[iFrmID].frameSize.y / vResolution.y);
        ImVec2 size = ImVec2(64.0f, 64.0f);

        wstring wstrName;
        string strName;
        static char cName[100];
        ImGui::InputText("##edit", cName, 100);
        ImGui::SameLine();
        if (ImGui::Button("AnimName"))
        {
            strName = cName;
            wstrName = wstring(strName.begin(), strName.end());
        }

        if (nullptr != m_pTex) ImGui::ImageButton((ImTextureID)(m_pTex->GetSRV().Get()), size, uv_0, uv_1, -1, ImVec4{ 0,0,0,1 }, tint_col);

        if (ImGui::Button("Play"))
        {
            iFrmID = 0;
            m_bPlay = true;
        }
        ImGui::SameLine(); if (ImGui::Button("repeat")) if (m_bRepeat) m_bRepeat = false; else m_bRepeat = true;

        if (ImGui::Button("Anim Save"))
        {
            strName = cName;
            wstrName = wstring(strName.begin(), strName.end());

            for (size_t i = 0; i < m_vecAnimInfo.size(); i++)
            {
                m_vecAnimInfo[i].frameSize.x /= m_pTex->Width();
                m_vecAnimInfo[i].frameSize.y /= m_pTex->Height();
                m_vecAnimInfo[i].leftTop.x /= m_pTex->Width();
                m_vecAnimInfo[i].leftTop.y /= m_pTex->Height();
            }
            AnimSave(wstrName);
        }

        if (m_bPlay) AnimPlay(iFrmID);
    }

    if (ImGui::Button("TexLoad")) TexLoad();
    ImGui::SameLine(); if (ImGui::Button("AnimLoad")) AnimLoad(iAnimIndexSize);


    ImGui::End();
}

void CAnimation2DTool::Listinit()
{
    m_vecTexList.clear();

    // 목록 전달
    vector<wstring> vecName;

    CResourceManager::GetInst()->GetResName(RESOURCE_TYPE::TEXTURE, vecName);

    for (size_t i = 0; i < vecName.size(); ++i)
    {
        m_vecTexList.push_back(string(vecName[i].begin(), vecName[i].end()));
    }

}

void CAnimation2DTool::AnimPlay(int& iFrmID)
{
    m_fAccTime += DELTATIME;

    while (m_vecAnimInfo[iFrmID].duration < m_fAccTime)
    {
        m_fAccTime = m_fAccTime - m_vecAnimInfo[iFrmID].duration;
        ++iFrmID;

        if (iFrmID == m_vecAnimInfo.size())
        {
            if (m_bRepeat)
            {
                iFrmID = 0;
                m_fAccTime = 0;
                m_bPlay = true;
            }
            else
            {
                iFrmID -= 1;
                m_fAccTime = 0;
                m_bPlay = false;
                break;
            }
        }
    }
}

void CAnimation2DTool::ChangeToolVersion()
{
    for (size_t i = 0; i < m_vecAnimInfo.size(); i++)
    {
        m_vecAnimInfo[i].frameSize.x *= m_pTex->Width();
        m_vecAnimInfo[i].frameSize.y *= m_pTex->Height();
        m_vecAnimInfo[i].leftTop.x *= m_pTex->Width();
        m_vecAnimInfo[i].leftTop.y *= m_pTex->Height();
    }
}

void CAnimation2DTool::AnimSave(wstring wstrAnimName)
{
    if (!m_pTex)
        return;

    CGameObject* pDummyObj = new CGameObject;
    pDummyObj->AddComponent(new CAnimator2D);
    pDummyObj->Animator2D()->CreateAnimation_Tool(wstrAnimName, CResourceManager::GetInst()->FindRes<CTexture>(m_strTexName), m_vecAnimInfo);

    CAnimation2D* pAnim2D = pDummyObj->Animator2D()->FindAnim(wstrAnimName);

    wstring strRoute = wstrAnimName + L".anim";
    pAnim2D->Save(L"anim\\", strRoute);

    CScript::DeleteObject(pDummyObj);
}

void CAnimation2DTool::AnimLoad(int& iSize)
{
    OPENFILENAME ofn = {}; 
    wchar_t szFile[260] = { 0 };  

    wstring strMapFilePath = CPathManager::GetInst()->GetContentPath();
    strMapFilePath += L"anim\\";

    // Initialize OPENFILENAME	
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = CCore::GetInst()->GetWindowHandle();
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All";
    ofn.nFilterIndex = 2;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = strMapFilePath.c_str(); 
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
    {
        wstring strOutput = ofn.lpstrFile;
        wstring strContentPath = CPathManager::GetInst()->GetContentPath();

        wstring strPath = strOutput.substr(strContentPath.length(), strOutput.length() - strContentPath.length());
        wstring strName = strPath.substr(7, strPath.length());

        CAnimation2D* pAnim = new CAnimation2D;
        pAnim->Load(strPath);

        m_vecAnimInfo.clear();
        m_vecAnimInfo = pAnim->GetVectorAnimFrame();
        m_pTex = pAnim->GetTexture().Get();
        iSize = (int)m_vecAnimInfo.size();
        ChangeToolVersion();

        delete pAnim;
    }
}

void CAnimation2DTool::TexLoad()
{
    OPENFILENAME ofn = {}; 
    wchar_t szFile[260] = { 0 }; 

    wstring strMapFilePath = CPathManager::GetInst()->GetContentPath();
    strMapFilePath += L"texture\\";

    // Initialize OPENFILENAME	
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = CCore::GetInst()->GetWindowHandle();
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All";
    ofn.nFilterIndex = 2;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = strMapFilePath.c_str(); 
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
    {
        wstring strOutput = ofn.lpstrFile;
        wstring strContentPath = CPathManager::GetInst()->GetContentPath();

        wstring strPath = strOutput.substr(strContentPath.length(), strOutput.length() - strContentPath.length());
        wstring strName = strPath.substr(7, strPath.length());
        strName = strName.substr(0, strName.length() - 4);
        CResourceManager::GetInst()->Load<CTexture>(strName, strPath);

        Listinit();
    }
}

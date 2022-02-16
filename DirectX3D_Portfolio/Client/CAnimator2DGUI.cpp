#include "pch.h"
#include "CAnimator2DGUI.h"
#include "CListGUI.h"
#include "CImGuiManager.h"
#include <Engine/CCore.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CTexture.h>
#include <Engine/CKeyManager.h>

UINT g_animation = 0;

CAnimator2DGUI::CAnimator2DGUI() :
	CComponentGUI(COMPONENT_TYPE::ANIMATOR2D),
	m_selectedAnim(nullptr),
	m_frameSize(Vec2(1.f, 1.f)),
	m_bFirst(true)
{
}

CAnimator2DGUI::~CAnimator2DGUI()
{
}

void CAnimator2DGUI::update()
{
	if (Start())
	{
		if (GetTarget())
		{
			CAnimation2D* pAnimation = GetTarget()->Animator2D()->GetCurAnim();
			if (pAnimation)
			{
				vector<tAnimFrame>& vecAnimFrm = pAnimation->GetVectorAnimFrame();
				int iFrmID = pAnimation->GetFrmIdx();
				float fAccTime = pAnimation->GetAccTime();
				wstring wstrName = pAnimation->GetName();
				string strName = string(wstrName.begin(), wstrName.end());

				static ImGuiComboFlags flags = 0;

				map<wstring, CAnimation2D*> mapAnim = GetTarget()->Animator2D()->GetAnimations();
				vector<string> strAnimName;
				for (auto iter = mapAnim.begin(); iter != mapAnim.end(); iter++)
				{
					string Name = string(iter->first.begin(), iter->first.end());
					strAnimName.push_back(Name);
				}

				ImGui::Text(strName.c_str());

				static int item_current_idx = 0; // Here we store our selection data as an index.
				if (ImGui::BeginCombo("##Animation", strName.c_str(), flags))
				{
					for (int n = 0; n < strAnimName.size(); n++)
					{
						const bool is_selected = (item_current_idx == n);
						if (ImGui::Selectable(strAnimName[n].c_str(), is_selected))
						{
							item_current_idx = n;
							wstring AnimName = wstring(strAnimName[n].begin(), strAnimName[n].end());
							GetTarget()->Animator2D()->Play(AnimName, true);
						}

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				ImGui::Text("FrmID   "); ImGui::SameLine(); if (ImGui::InputInt("##FrmID", &iFrmID))
				{
					if (iFrmID < 0)
						iFrmID = 0;
					else if (vecAnimFrm.size() == iFrmID)
						--iFrmID;

					pAnimation->SetFrmIdx(iFrmID);
				}
				ImGui::Text("fAccTime"); ImGui::SameLine();	ImGui::InputFloat("##fAccTime", &fAccTime);
				ImGui::Text("FrmSize "); ImGui::SameLine();
				ImGui::InputFloat2("##FrmSize", (float*)&vecAnimFrm[iFrmID].frameSize);
				ImGui::Text("LeftTop "); ImGui::SameLine();
				ImGui::InputFloat2("##LeftTop", (float*)&vecAnimFrm[iFrmID].leftTop);
				ImGui::Text("Offset  "); ImGui::SameLine();
				ImGui::InputFloat2("##Offset", (float*)&vecAnimFrm[iFrmID].offset);
				ImGui::Text("Duration"); ImGui::SameLine();
				ImGui::InputFloat("##Duration", &vecAnimFrm[iFrmID].duration);

				if (ImGui::Button("Reset")) pAnimation->Reset();
			}

			if (ImGui::Button("Anmaion Load"))
			{
				AnimationLoad();
			}
		}

		End();
	}
}

void CAnimator2DGUI::SetTexture(DWORD_PTR _data1, DWORD_PTR _data2)
{
	string name = (const char*)_data1;
	wstring key = StringToWString(name);
	Ptr<CTexture> texture = CResourceManager::GetInst()->FindRes<CTexture>(key);

	m_selectedAnim->SetTexture(texture);
}

void CAnimator2DGUI::AnimationLoad()
{
	OPENFILENAME ofn = {};       // common dialog box structure
	wchar_t szFile[260] = { 0 };       // if using TCHAR macros

	wstring strMapFilePath = CPathManager::GetInst()->GetContentPath();
	strMapFilePath += L"anim\\";

	// Initialize OPENFILENAME	
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = CCore::GetInst()->GetWindowHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0애니메이션\0*.anim\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strMapFilePath.c_str(); // 탐색창 초기 경로
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn)) // DoModal (modal window)
	{
		wstring strOutput = ofn.lpstrFile;
		wstring strContentPath = CPathManager::GetInst()->GetContentPath();

		wstring strPath = strOutput.substr(strContentPath.length(), strOutput.length() - strContentPath.length());
		GetTarget()->Animator2D()->LoadAnim(strPath);

		strMapFilePath = L"anim\\";
		wstring strName = strPath.substr(strMapFilePath.length(), strPath.length() - strMapFilePath.length());

		if (strName.substr(strName.length() - 5) == L".anim")
		{
			strName = strName.substr(0, strName.length() - 5);
		}

		GetTarget()->Animator2D()->Play(strName, true);
	}
}
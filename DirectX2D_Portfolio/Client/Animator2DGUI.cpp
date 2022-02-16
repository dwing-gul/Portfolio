#include "pch.h"
#include "Animator2DGUI.h"
#include "ListGUI.h"
#include "ImGuiManager.h"
#include <Engine/Core.h>
#include <Engine/Animator2D.h>
#include <Engine/Animation2D.h>
#include <Engine/Texture.h>
#include <Engine/KeyManager.h>

UINT g_animation = 0;

Animator2DGUI::Animator2DGUI() :
	ComponentGUI(COMPONENT_TYPE::ANIMATOR2D),
	m_selectedAnim(nullptr),
	m_frameSize(Vector2(1.f, 1.f)),
	m_bFirst(true)
{
}

Animator2DGUI::~Animator2DGUI()
{
}

void Animator2DGUI::update()
{
	if (Start())
	{
		Animator2D* animator = GetTarget()->GetAnimator2D();

		map<wstring, Animation2D*>& mapAnim = animator->GetAnimations();
		map<wstring, Animation2D*>::iterator iter = mapAnim.begin();
		Ptr<Texture> texture = nullptr;
		int frameCount = 1;
		Vector2 frameLeftTop = Vector2(0.f, 0.f);
		tAnimFrame frame = {};
		bool bInput = false;

		if (ImGui::Button("Create##Animator"))
			ImGui::OpenPopup("Anim Menu");
		if (ImGui::BeginPopup("Anim Menu"))
		{ 
			if (ImGui::MenuItem("Exist Animation##Animator"))
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
				ofn.lpstrFilter = L"All\0*.*\0애니메이션\0*.anim\0";
				ofn.nFilterIndex = 2;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = strMapFilePath.c_str(); // 탐색창 초기 경로
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				if (GetOpenFileName(&ofn)) // DoModal (modal window)
				{
					wstring strOutput = ofn.lpstrFile;
					wstring contentPath = PathManager::GetInst()->GetContentPath();

					wstring relativePath = strOutput.substr(contentPath.length(), strOutput.length() - contentPath.length());
					animator->LoadAnim(relativePath);
				}
			}

			if (ImGui::MenuItem("New Animation##Animator"))
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
				ofn.lpstrInitialDir = strMapFilePath.c_str(); // 탐색창 초기 경로
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

					texture = ResourceManager::GetInst()->FindRes<Texture>(key);

					wchar_t buffer[255];

					while (true)
					{
						swprintf_s(buffer, L"New Animation_%d", g_animation++);
						Animation2D* anim = animator->FindAnim(buffer);

						if (!anim)
							break;
					}

					animator->CreateAnim(buffer, texture, Vector2(0.f, 0.f),
						Vector2(texture->Width(), texture->Height()), Vector2(texture->Width(), texture->Height()), 0.1f, 1);
				}
			}

			ImGui::EndPopup();
		}

		ImGui::Text("Animations");
		for (; iter != mapAnim.end(); ++iter)
		{
			string name = WStringToString(iter->first);
			char buffer[255] = {};
			sprintf_s(buffer, "##%lld", (DWORD_PTR)iter->second);
			string TreeID = name + buffer;
			texture = iter->second->GetTexture();

			m_selectedAnim = iter->second;
			Ptr<Texture> atlas = m_selectedAnim->GetTexture();
			vector<tAnimFrame>& vectorFrame = m_selectedAnim->GetVectorAnimFrame();
			Vector2 baseSize = m_selectedAnim->GetBaseSize();
			bool repeat = m_selectedAnim->IsRepeat();

			char textureName[255] = {};
			GetResourceName(atlas.Get(), textureName);

			if (ImGui::TreeNodeEx(TreeID.c_str()))
			{
				string TextID = "##Name" + name + buffer;

				char animName[255] = {};
				strcpy_s(animName, 255, name.c_str());
				if (ImGui::InputText(TextID.c_str(), animName, 255))
				{
					if (KEY_TAP(KEY::ENTER) && ImGui::IsItemFocused())
					{
						wstring targetName = StringToWString(animName);
						if (iter->first != targetName)
						{
							Animation2D* temp = iter->second->Clone();
							mapAnim.insert(make_pair(targetName, temp));
							delete iter->second;
							mapAnim.erase(iter);
							ImGui::TreePop();
							End();
							return;
						}
					}
				}

				ImGui::Text("Texture");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(180);
				ImGui::InputText("##AtlasTexture", textureName, 255, ImGuiInputTextFlags_ReadOnly);
				ImGui::SameLine();
				name = "Select";
				name += buffer;
				if (ImGui::Button(name.c_str()))
				{
					ListGUI* listGUI = (ListGUI*)ImGuiManager::GetInst()->FindGUI("ListGUI");
					listGUI->SetCaption("AtlasTexture");
					vector<wstring> vectorName;
					ResourceManager::GetInst()->GetResName(RESOURCE_TYPE::TEXTURE, vectorName);
					listGUI->SetList(vectorName);
					listGUI->SetDoubleClickedCallBack(this, (GUI_CALLBACK)&Animator2DGUI::SetTexture);
					listGUI->Activate();
				}

				for (size_t i = 0; i < vectorFrame.size(); ++i)
				{
					name = "FrameInfo_";
					char ID[255] = {};
					sprintf_s(ID, "%d", (int)i);
					name += ID;
					name += buffer;

					if (ImGui::TreeNodeEx(name.c_str()))
					{
						ImGui::Text("LeftTop");
						ImGui::SameLine(120.f, 5.f);
						ImGui::SetNextItemWidth(180);
						name = "##AnimLeftTop";
						name += buffer;
						if (ImGui::InputFloat2(name.c_str(), vectorFrame[i].leftTop))
						{
							frameLeftTop = vectorFrame[i].leftTop;
						}

						ImGui::Text("FrameSize");
						ImGui::SameLine(120.f, 5.f);
						ImGui::SetNextItemWidth(180);
						name = "##FrameSize";
						name += buffer;
						if (m_bFirst)
						{
							m_bFirst = false;
							m_frameSize = vectorFrame[i].frameSize;
						}
						ImGui::InputFloat2(name.c_str(), m_frameSize);
						name = "Select##FrameSize";
						name += buffer;
						if (ImGui::Button(name.c_str()))
						{
							bInput = true;
						}

						if (bInput)
						{
							bInput = false;
							vectorFrame[i].frameSize.x = m_frameSize.x / texture->Width();
							vectorFrame[i].frameSize.y = m_frameSize.y / texture->Height();
							frame.frameSize = vectorFrame[i].frameSize;
						}

						ImGui::Text("Offset");
						ImGui::SameLine(120.f, 5.f);
						ImGui::SetNextItemWidth(180);
						name = "##Offset";
						name += buffer;
						ImGui::InputFloat2(name.c_str(), vectorFrame[i].offset);

						ImGui::Text("Duration");
						ImGui::SameLine(120.f, 5.f);
						ImGui::SetNextItemWidth(180);
						name = "##Duration";
						name += buffer;
						ImGui::InputFloat(name.c_str(), &vectorFrame[i].duration);

						ImGui::TreePop();
					}
				}

				ImGui::Text("BaseSize");
				ImGui::SameLine(100.f, 5.f);
				ImGui::SetNextItemWidth(180);
				name = "##BaseSize";
				sprintf_s(buffer, "##%lld", (DWORD_PTR)iter->second);
				name += buffer;
				ImGui::InputFloat2(name.c_str(), baseSize);

				ImGui::Text("FrameCount");
				ImGui::SameLine(100.f, 5.f);
				ImGui::SetNextItemWidth(180);
				name = "##FrameCount";
				sprintf_s(buffer, "##%lld", (DWORD_PTR)iter->second);
				name += buffer;
				if (ImGui::InputInt(name.c_str(), &frameCount))
				{
					for (int i = (int)vectorFrame.size(); i < frameCount; ++i)
					{
						frame.leftTop = Vector2(frameLeftTop.x + (float)i * m_frameSize.x, frameLeftTop.y);
						vectorFrame.push_back(frame);
					}
				}

				name = "Repeat";
				name += buffer;
				ImGui::Checkbox(name.c_str(), &repeat);

				name = "PlayAnim";
				name += buffer;
				if (ImGui::Button(name.c_str()))
				{
					animator->Play(iter->first);
				}

				ImGui::TreePop();
			}

			m_selectedAnim->SetBaseSize(baseSize);
			m_selectedAnim->SetRepeat(repeat);
		}
		End();
	}
}

void Animator2DGUI::SetTexture(DWORD_PTR _data1, DWORD_PTR _data2)
{
	string name = (const char*)_data1;
	wstring key = StringToWString(name);
	Ptr<Texture> texture = ResourceManager::GetInst()->FindRes<Texture>(key);

	m_selectedAnim->SetTexture(texture);
}

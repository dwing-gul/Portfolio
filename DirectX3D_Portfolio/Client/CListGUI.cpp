#include "pch.h"
#include "CListGUI.h"

CListGUI::CListGUI() :
	m_bPopUp(false),
	m_doubleClickedCallback(nullptr),
	m_inst(nullptr),
	m_doubleClicked(nullptr)
{
	SetName("ListGUI");
	Deactivate();
}

CListGUI::~CListGUI()
{
}

void CListGUI::update()
{
	bool b = true;

	if (m_bPopUp)
	{
		ImGui::OpenPopup(m_caption.c_str());
		m_bPopUp = false;
	}

	ImGui::SetNextWindowSize(ImVec2(300.f, 500.f));
	if (ImGui::BeginPopupModal(m_caption.c_str(),&b, ImGuiWindowFlags_None))// &IsCancelButton()))
	{
		static int itemIdx = 0;
		if (ImGui::BeginListBox("##ListBox"))
		{
			for (size_t i = 0; i < m_vecListName.size(); ++i)
			{
				const bool selected = (itemIdx == i);
				if (ImGui::Selectable(m_vecListName[i].c_str(), selected))
				{
					itemIdx = (int)i;
				}

				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
				{
					if (m_inst && m_doubleClickedCallback)
					{
						((*m_inst).*m_doubleClickedCallback)((DWORD_PTR)m_vecListName[i].c_str(), 0);
					}

					if (m_doubleClicked)
					{
						m_doubleClicked((DWORD_PTR)m_vecListName[i].c_str(), 0);
					}

					ImGui::CloseCurrentPopup();
					Clear();
				}
			}

			ImGui::EndListBox();
		}

		ImGui::EndPopup();
	}
	else
	{
		Clear();
	}
}

void CListGUI::Activate()
{
	CGUI::Activate();
	m_bPopUp = true;
}

void CListGUI::SetList(const vector<wstring>& _vector)
{
	for (size_t i = 0; i < _vector.size(); ++i)
	{
		m_vecListName.push_back(string(_vector[i].begin(), _vector[i].end()));
	}
}

void CListGUI::SetList(const vector<string>& _vector)
{
	m_vecListName = _vector;
}

void CListGUI::Clear()
{
	m_inst = nullptr;
	m_doubleClickedCallback = nullptr;
	m_vecListName.clear();

	ImGui::SetWindowFocus(nullptr);
	Deactivate();
}

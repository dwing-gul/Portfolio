#include "pch.h"
#include "ListGUI.h"

ListGUI::ListGUI() :
	m_bPopUp(false),
	m_doubleClickedCallBack(nullptr),
	m_inst(nullptr)
{
	SetName("ListGUI");
	Deactivate();
}

ListGUI::~ListGUI()
{
}

void ListGUI::update()
{
	if (m_bPopUp)
	{
		ImGui::OpenPopup(m_caption.c_str());
		m_bPopUp = false;
	}

	ImGui::SetNextWindowSize(ImVec2(300.f, 500.f));
	if (ImGui::BeginPopupModal(m_caption.c_str(), &IsCancelButton()))
	{
		static int itemIdx = 0;
		if (ImGui::BeginListBox("##ListBox"))
		{
			for (size_t i = 0; i < m_vectorListName.size(); ++i)
			{
				const bool selected = (itemIdx == i);
				if (ImGui::Selectable(m_vectorListName[i].c_str(), selected))
				{
					itemIdx = (int)i;
				}

				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}

				if (ImGui::IsItemClicked())
				{
					if (m_inst && m_doubleClickedCallBack)
					{
						((*m_inst).*m_doubleClickedCallBack)((DWORD_PTR)m_vectorListName[i].c_str(), 0);
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

void ListGUI::Activate()
{
	GUI::Activate();
	m_bPopUp = true;
}

void ListGUI::SetList(const vector<wstring>& _vector)
{
	for (size_t i = 0; i < _vector.size(); ++i)
	{
		string listName = WStringToString(_vector[i]);
		m_vectorListName.push_back(listName);
	}
}

void ListGUI::SetList(const vector<string>& _vector)
{
	m_vectorListName = _vector;
}

void ListGUI::Clear()
{
	m_inst = nullptr;
	m_doubleClickedCallBack = nullptr;
	m_vectorListName.clear();
	Deactivate();
}
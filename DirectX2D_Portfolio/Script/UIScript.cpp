#include "pch.h"
#include "UIScript.h"
#include "MonsterScript.h"
#include <Engine/GameObject.h>
#include <Engine/Transform.h>
#include <Engine/Collider2D.h>
#include <Engine/SceneManager.h>

UIScript::UIScript() :
	Script((UINT)SCRIPT_TYPE::UISCRIPT),
	m_targetScript(nullptr),
	m_focusedUI(nullptr),
	m_prevTargetUI(nullptr),
	m_curTargetUI(nullptr),
	m_originXPos(0.f),
	m_originYPos(0.f),
	m_bChange(true),
	m_bMonsterInfo(false)
{
}

UIScript::UIScript(SCRIPT_TYPE _type) :
	Script((UINT)_type),
	m_targetScript(nullptr),
	m_focusedUI(nullptr),
	m_prevTargetUI(nullptr),
	m_curTargetUI(nullptr),
	m_originXPos(0.f),
	m_originYPos(0.f),
	m_bChange(true),
	m_bMonsterInfo(false),
	m_bFirst(true)
{
}

UIScript::~UIScript()
{
}

void UIScript::start()
{
	if (Object()->GetName() == L"MonsterInfo")
		m_bMonsterInfo = true;

	Vector3 pos = GetTransform()->GetLocalPos();
	m_originXPos = pos.x;
	m_originYPos = pos.y;

	m_focusedUI = SceneManager::GetInst()->FindObjByName(L"RootUI");
}

void UIScript::update()
{
	GameObject* prevUI = m_focusedUI;
	m_focusedUI = GetFocusedUI();

	if (nullptr == m_focusedUI)
		return;

	if (prevUI != m_focusedUI)
	{
		vector<GameObject*>& vectorUI = SceneManager::GetInst()->FindObjByType_Ref(OBJECT_TYPE::UI);

		vector<GameObject*>::iterator iter = vectorUI.begin();
		for (; iter != vectorUI.end(); ++iter)
		{
			if ((*iter) == m_focusedUI)
			{
				vectorUI.erase(iter);
				break;
			}
		}

		vectorUI.push_back(m_focusedUI);
	}

	if (nullptr != m_focusedUI)
	{
		if (m_focusedUI->IsDead())
		{
			m_focusedUI = nullptr;
		}
		else
			UIEvent(m_focusedUI);
	}
}

void UIScript::lateUpdate()
{
	if (nullptr == Object()->GetParent())
	{
		GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");
		Vector3 pos = GetTransform()->GetLocalPos();
		Vector3 cameraPos = camera->GetTransform()->GetLocalPos();
		pos.x = cameraPos.x;
		pos.y = cameraPos.y + m_originYPos;
		pos.z = 20.f;

		GetTransform()->SetLocalPos(pos);
	}
	else
	{
		if (m_bChange)
		{
			m_bChange = false;
			float flag = 1.f;
			// 부모를 따라다닐 수 있도록
			Vector3 scale = GetTransform()->GetLocalScale();
			Vector3 parentScale = Object()->GetParent()->GetTransform()->GetLocalScale();
			Vector3 parentPos = Object()->GetParent()->GetTransform()->GetLocalPos();

			scale.x = scale.x / parentScale.x;
			scale.y = scale.y / parentScale.y;
			GetTransform()->SetLocalScale(scale);

			Vector3 finalPos = GetTransform()->GetLocalPos();
			finalPos.x = m_originXPos / parentScale.x;

			if (m_originXPos == parentPos.x)
				finalPos.x = 0.f;

			if (m_originYPos < parentPos.y)
				flag *= -1.f;
			else
				flag *= 1.f;

			finalPos.y = flag * (abs(m_originYPos - parentPos.y) / parentScale.y);
			finalPos.z = 20.f;

			GetTransform()->SetLocalPos(finalPos);
		}
	}
}

GameObject* UIScript::GetFocusedUI()
{
	vector<GameObject*>& vectorUI = SceneManager::GetInst()->FindObjByType_Ref(OBJECT_TYPE::UI);

	GameObject* focusedUI = m_focusedUI;

	for (size_t i = 0; i < vectorUI.size(); ++i)
	{
		if (((GameObject*)vectorUI[i])->GetCollider2D()->IsObjectHovered() && IsMouseClicked())
		{
			focusedUI = ((GameObject*)vectorUI[i]);
		}
	}

	return focusedUI;
}

GameObject* UIScript::GetTargetedUI(GameObject* _parentUI)
{
	list<GameObject*> queue;

	queue.push_back(_parentUI);
	GameObject* target = nullptr;

	while (!queue.empty())
	{
		GameObject* ui = queue.front();
		queue.pop_front();

		const vector<GameObject*>& vectorChild = ui->GetChilds();
		for (size_t i = 0; i < vectorChild.size(); ++i)
		{
			queue.push_back(vectorChild[i]);
		}

		if (m_bFirst)
		{
			m_bFirst = false;
			target = SceneManager::GetInst()->FindObjByName(L"InventoryPanel");
			m_prevTargetUI = target;
		}
		else
		{
			if (ui->GetCollider2D()->IsObjectHovered() && IsMouseClicked())
			{
				target = ui;
				m_prevTargetUI = target;
			}
		}

		const vector<Script*>& vectorScript = ui->GetScripts();
		for (size_t i = 0; i < vectorScript.size(); ++i)
		{
			if ((UINT)SCRIPT_TYPE::UISCRIPT == vectorScript[i]->GetScriptType())
			{
				if (((UIScript*)vectorScript[i])->m_bMonsterInfo && ((UIScript*)vectorScript[i])->m_targetScript != nullptr)
				{
					if (((UIScript*)vectorScript[i])->m_targetScript->IsInfoOpen())
					{
						target = ui;
					}
					else
					{
						target = m_prevTargetUI;
					}
				}
			}
		}
	}

	return target;
}

void UIScript::UIEvent(GameObject* _parentUI)
{
	GameObject* ui = GetTargetedUI(_parentUI);
	
	if (ui)
	{
		if (ui->GetParent())
		{
			vector<GameObject*>& vectorChild = ui->GetParent()->GetChild_Ref();

			for (size_t i = 0; i < vectorChild.size(); ++i)
			{
				if (ui == vectorChild[i])
				{
					GameObject* temp = ui;

					list<GameObject*> queue;
					for (size_t j = 0; j < i; ++j)
					{
						queue.push_back(vectorChild[j]);
					}

					vectorChild[0] = temp;

					for (size_t j = 1; j < i + 1; ++j)
					{
						vectorChild[j] = queue.front();
						queue.pop_front();
					}
				}
			}

			if (ui == SceneManager::GetInst()->FindObjByName(L"InventoryPanel"))
			{
				GameObject* torch = SceneManager::GetInst()->FindObjByName(L"Item_Torch");

				if (nullptr != torch)
				{
					ui = torch;

					for (size_t i = 0; i < vectorChild.size(); ++i)
					{
						if (ui == vectorChild[i])
						{
							GameObject* temp = ui;

							list<GameObject*> queue;
							for (size_t j = 0; j < i; ++j)
							{
								queue.push_back(vectorChild[j]);
							}

							vectorChild[0] = temp;
							m_curTargetUI = temp;

							for (size_t j = 1; j < i + 1; ++j)
							{
								vectorChild[j] = queue.front();
								queue.pop_front();
							}
						}
					}
				}
			}
			else
			{
				m_curTargetUI = nullptr;
			}
		}

		if (ui->GetCollider2D()->IsObjectHovered() && IsMouseClicked())
		{
			m_focusedUI = _parentUI;
		}

		if (ui->GetCollider2D()->IsObjectHovered() && IsMouseClicked(MOUSE_BUTTON::RIGHT))
		{
			m_focusedUI = _parentUI;
		}
	}
}
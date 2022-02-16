#include "pch.h"
#include "CUIScript.h"
#include "CPlayerScript.h"
#include <Engine/CSceneManager.h>
#include <Engine/CRenderManager.h>
#include <Engine/CResourceManager.h>
#include <Engine/CKeyManager.h>
#include <Engine/CCollider2D.h>

CUIScript::CUIScript(UINT _type) :
	CScript(_type),
	m_owner(nullptr),
	m_target(nullptr),
	m_uiType(UI_TYPE::END),
	m_originPos(Vec3(0.f, 0.f, 0.f)),
	m_originScale(Vec3(1.f, 1.f, 1.f)),
	m_bChange(true),
	m_bDrag(false),
	m_bMove(false)
{
}

CUIScript::CUIScript(const CUIScript& _origin) :
	CScript(_origin.m_scriptType),
	m_owner(nullptr),
	m_target(nullptr),
	m_uiType(_origin.m_uiType),
	m_originPos(_origin.m_originPos),
	m_originScale(_origin.m_originScale),
	m_bChange(true),
	m_bDrag(_origin.m_bDrag),
	m_bMove(false)
{
}

CUIScript::~CUIScript()
{
}

void CUIScript::start()
{
	if(m_originPos == Vec3(0.f, 0.f, 0.f))
		m_originPos = Transform()->GetLocalPos();

	m_originScale = Transform()->GetLocalScale();
	m_owner = CSceneManager::GetInst()->FindObjByName(L"Player");

	if(!m_targetName.empty())
		m_target = CSceneManager::GetInst()->FindObjByName(m_targetName);
}

void CUIScript::update()
{
	if (nullptr != Object()->GetParent())
	{
		if (m_bChange)
		{
			m_bChange = false;
			float flag = 1.f;
			// 부모를 따라다닐 수 있도록
			Vector3 scale;
			Vector3 parentScale = Object()->GetParent()->Transform()->GetLocalScale();
			Vector3 parentPos = Object()->GetParent()->Transform()->GetLocalPos();

			scale.x = m_originScale.x / parentScale.x;
			scale.y = m_originScale.y / parentScale.y;
			Transform()->SetLocalScale(scale);

			Vector3 finalPos = Transform()->GetLocalPos();
			finalPos.x = m_originPos.x / parentScale.x;
			finalPos.z = m_originPos.z;

			if (m_originPos.x == parentPos.x)
				finalPos.x = 0.f;

			if (m_originPos.y < parentPos.y)
				flag *= -1.f;
			else
				flag *= 1.f;

			finalPos.y = flag * (abs(m_originPos.y - parentPos.y) / parentScale.y);

			Transform()->SetLocalPos(finalPos);
		}
	}
}

void CUIScript::lateUpdate()
{
	if (m_bDrag)
	{
		if (Collider2D()->IsObjectHovered() && IsMouseClicked(MOUSE_BUTTON::LEFT, true))
		{
			m_bMove = true;
		}

		if (KEY_AWAY(KEY::LBUTTON))
		{
			m_bMove = false;
		}

		if (m_bMove)
		{
			DragNDrop(Object());
		}
	}
}

void CUIScript::PlayerChange()
{
	m_owner = CSceneManager::GetInst()->FindObjByName(L"Player");
}


void CUIScript::SaveToScene(FILE* _file)
{
	SaveToFile(&m_originPos, _file);
	SaveToFile(&m_originScale, _file);
	SaveToFile(&m_uiType, _file);
	SaveWStringToFile(m_targetName, _file);
}

void CUIScript::LoadFromScene(FILE* _file)
{
	LoadFromFile(&m_originPos, _file);
	LoadFromFile(&m_originScale, _file);
	LoadFromFile(&m_uiType, _file);
	LoadWStringFromFile(m_targetName, _file);
}

#include "pch.h"

#include "CMouseRay.h"
#include <Engine\CCamera.h>
#include <Engine\CRenderManager.h>
#include <Engine\CCollider3D.h>
#include <Engine\CKeyManager.h>

#include <Engine\CLayer.h>

CMouseRay::CMouseRay()
	: CScript((UINT)SCRIPT_TYPE::MOUSERAY), 
	m_distance(100000.f),
	m_targetObj(nullptr),
	m_pickingTime(0.f),
	m_camObj(nullptr)

{
	SetName(L"MouseRay");
}

CMouseRay::CMouseRay(const CMouseRay& _origin) : 
	CScript((UINT)SCRIPT_TYPE::MOUSERAY),
	m_distance(_origin.m_distance),
	m_targetObj(nullptr),
	m_pickingTime(0.f),
	m_camObj(nullptr)
{
	SetName(L"MouseRay");
}

CMouseRay::~CMouseRay()
{
}

void CMouseRay::init()
{
}

void CMouseRay::start()
{
	m_camObj = CSceneManager::GetInst()->FindObjByName(L"MainCamera");
}

void CMouseRay::update()
{
	if (nullptr != m_targetObj)
	{
		//��ŷ Ÿ���� �ش� Ÿ���� ���� �� �ð��� üũ���ְ� �ð��� ������ Ÿ���� nullptr�� ���ܝ���
		m_pickingTime += DELTATIME;

		if (m_pickingTime > 3.f || m_targetObj->IsDead())
		{
			m_targetObj = nullptr;
			m_pickingTime = 0.f;
		}
	}
}

void CMouseRay::lateUpdate()
{
	//���콺 ������Ʈ�� ���� ���̱� ������ �ش� ���콺 ������Ʈ�� MouseRay ��ũ��Ʈ�� �־������
	//�ٿ�� �ڽ��� �־��� ������Ʈ�� �� �ش� Ű�� Ŭ�� �Ͽ��� ��� Ÿ���� ���� �� ����
	if (KEY_TAP(KEY::LBUTTON) && KEY_HOLD(KEY::F))
	{
		MousePicking();
	}
}

CGameObject* CMouseRay::MousePicking()
{
	if (nullptr == m_camObj)
		return nullptr;

	tRay ray = m_camObj->Camera()->GetRay();
	m_mouseRay.position = ray.start;
	m_mouseRay.direction = ray.dir;

	BoundingSphere objBoundingSphere;
	//���� ���콺�� ī�޶��� ��ġ�� ���̾ �޾ƿ;��ϴµ�
	float fDist = 0.f;
	m_distance = 555555555.f;

	for (int i = 0; i < UI_LAYER; i++)
	{
		if (!CSceneManager::GetInst()->GetCurScene()->GetLayer(i)->GetLayerAction())
			continue;

		vector<CGameObject*> vecAllObj = CSceneManager::GetInst()->GetCurScene()->GetLayer(i)->GetAllObj();
	
		//���콺 Ŭ�� �Ͽ��� ��� �ش� ������Ʈ�� ���콺 ���̸� ����� �Ͽ� �浹�ߴ��� Ȯ���� ��
		//�浹 ���� ��� �Ÿ��� ���� ����� ������Ʈ�� Ÿ���� �־���
		for (int i = 0; i < vecAllObj.size(); ++i)
		{
			if (OBJECT_TYPE::PLAYER != vecAllObj[i]->GetObjectType() && OBJECT_TYPE::MONSTER != vecAllObj[i]->GetObjectType()
				&& OBJECT_TYPE::INTERACTION != vecAllObj[i]->GetObjectType())
				continue;

			objBoundingSphere.Center = vecAllObj[i]->Transform()->GetWorldPos();
			objBoundingSphere.Radius = vecAllObj[i]->Transform()->GetBoundingRange();

			if (objBoundingSphere.Radius == 0.f)
				objBoundingSphere.Radius = 100.f;

			if (m_mouseRay.Intersects(objBoundingSphere, fDist))
			{
				if (m_distance > fDist)
				{
					m_distance = fDist;
					m_targetObj = vecAllObj[i];
					m_pickingTime = 0.f;
				}
			}
		}
	}

	return m_targetObj;
}

void CMouseRay::SaveToScene(FILE* _file)
{
}

void CMouseRay::LoadFromScene(FILE * _file)
{
}

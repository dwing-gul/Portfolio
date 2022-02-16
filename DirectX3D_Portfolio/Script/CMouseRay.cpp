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
		//피킹 타임은 해당 타겟이 선택 된 시간을 체크해주고 시간이 지나면 타겟을 nullptr로 변겨앻줌
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
	//마우스 오브젝트를 만들 것이기 때문에 해당 마우스 오브젝트에 MouseRay 스크립트를 넣어줘야함
	//바운드 박스를 넣어준 오브젝트들 중 해당 키로 클릭 하였을 경우 타겟을 얻어올 수 있음
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
	//현재 마우스나 카메라의 위치에 레이어를 받아와야하는데
	float fDist = 0.f;
	m_distance = 555555555.f;

	for (int i = 0; i < UI_LAYER; i++)
	{
		if (!CSceneManager::GetInst()->GetCurScene()->GetLayer(i)->GetLayerAction())
			continue;

		vector<CGameObject*> vecAllObj = CSceneManager::GetInst()->GetCurScene()->GetLayer(i)->GetAllObj();
	
		//마우스 클릭 하였을 경우 해당 오브젝트와 마우스 레이를 계산을 하여 충돌했는지 확인을 함
		//충돌 했을 경우 거리가 가장 가까운 오브젝트의 타겟을 넣어줌
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

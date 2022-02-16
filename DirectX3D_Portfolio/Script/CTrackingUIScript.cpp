#include "pch.h"
#include "CTrackingUIScript.h"
#include "CBarUIScript.h"
#include "CCameraScript.h"
#include <Engine/CRenderManager.h>

float CTrackingUIScript::g_zPos = 10.f;

CTrackingUIScript::CTrackingUIScript() :
	CUIScript((UINT)SCRIPT_TYPE::TRACKINGUISCRIPT),
	m_offsetPos(Vec3(0.f, 0.f, 0.f)),
	m_trackingType(TRACKING_TARGET_TYPE::END),
	m_cam(nullptr),
	m_trackingScale(Vec3(1.f, 1.f, 1.f))
{
}

CTrackingUIScript::CTrackingUIScript(const CTrackingUIScript& _origin) :
	CUIScript(_origin),
	m_offsetPos(Vec3(0.f, 0.f, 0.f)),
	m_trackingType(_origin.m_trackingType),
	m_trackingScale(_origin.m_trackingScale),
	m_cam(nullptr)
{
}

CTrackingUIScript::~CTrackingUIScript()
{
}

void CTrackingUIScript::start()
{
	CUIScript::start();

	Object()->SetObjectType(OBJECT_TYPE::UI);

	if (TRACKING_TARGET_TYPE::MONSTER == m_trackingType)
	{
		m_offsetPos = Vec3(0.f, 280.f, 0.f);
		Transform()->SetLocalScale(Vec3(1.1f, 1.1f, 1.f));
		m_trackingScale = Vec3(1.1f, 1.1f, 1.f);
	}
	else if(TRACKING_TARGET_TYPE::PLAYER == m_trackingType)
	{
		m_offsetPos = Vec3(0.f, 380.f, 0.f);
		Transform()->SetLocalScale(Vec3(1.3f, 1.3f, 1.f));
		m_trackingScale = Vec3(1.3f, 1.3f, 1.f);
	}
	else if (TRACKING_TARGET_TYPE::ITEM_SPAWN == m_trackingType)
	{
		m_offsetPos = Vec3(-100.f, -100.f, 0.f);
		Transform()->SetLocalScale(Vec3(0.3f, 0.3f, 1.f));
	}

	m_cam = CSceneManager::GetInst()->FindObjByName(L"MainCamera");
}

void CTrackingUIScript::update()
{
	CUIScript::update();
}

void CTrackingUIScript::lateUpdate()
{
	CUIScript::lateUpdate();

	if (nullptr != m_target && nullptr != m_cam) // 캐릭터 머리 위에서 따라다니는 ui
	{
		Matrix ViewMatrix = m_cam->Camera()->GetViewMatrix();
		if (TRACKING_TARGET_TYPE::ITEM_SPAWN == m_trackingType)
		{
			m_targetName = m_target->GetName();
			Matrix targetWorldMatrix = m_target->Transform()->GetWorldMatrix();
			Vec3 targetViewPos = (targetWorldMatrix * ViewMatrix).Translation();
			targetViewPos.z = 0.f;

			Vec3 pos = targetViewPos + m_offsetPos;

			Transform()->SetLocalPos(pos);
		}
		else
		{
			Vec3 targetPos = m_target->Transform()->GetWorldPos();
			Vec3 dir = m_target->Transform()->GetWorldDir(DIR_TYPE::UP);
			Vec3 camPos = m_cam->Transform()->GetLocalPos();
			Matrix worldMatrix = Transform()->GetWorldMatrix();
			Vec3 viewPos = (worldMatrix * ViewMatrix).Translation();
			bool camChange = ((CCameraScript*)m_cam->GetScripts()[0])->IsChange();

			if (m_target == m_owner && !camChange)
			{
				Vec3 pos = Transform()->GetWorldPos();
				Vec3 up = m_cam->Transform()->GetWorldDir(DIR_TYPE::UP);
				Matrix newWorld = Matrix::CreateBillboard(pos, camPos, up);
				Vec3 finalPos = targetPos + m_offsetPos * dir;
				newWorld._41 = finalPos.x;
				newWorld._42 = finalPos.y;
				newWorld._43 = finalPos.z;

				g_zPos = viewPos.z;

				Transform()->SetWorldMatrix(newWorld);
			}
			else
			{
				Vec3 rot = m_cam->Transform()->GetLocalRot();
				Vec3 scale = m_trackingScale;
				Vec3 pos = targetPos + m_offsetPos * dir;

				float ratio = viewPos.z / g_zPos;

				scale.x *= ratio;
				scale.y *= ratio;

				Transform()->SetLocalPos(pos);
				Transform()->SetLocalRot(rot);
				Transform()->SetLocalScale(scale);
				Transform()->SetWorld(false);
			}

			if (L"RootTrackingUI" == Object()->GetName())
			{
				const vector<CGameObject*>& vecChild = Object()->GetChilds();

				for (size_t i = 0; i < vecChild.size(); ++i)
				{
					if (vecChild[i]->GetScripts().empty())
						continue;

					if (vecChild[i]->GetScripts()[0]->GetScriptType() == (UINT)SCRIPT_TYPE::BARUISCRIPT)
					{
						Vec3 childPos = ((CBarUIScript*)vecChild[i]->GetScripts()[0])->Transform()->GetLocalPos();
						childPos.x = 17.f;

						((CBarUIScript*)vecChild[i]->GetScripts()[0])->Transform()->SetLocalPos(childPos);
					}
				}
			}
		}
	}
}

void CTrackingUIScript::SaveToScene(FILE* _file)
{
	CUIScript::SaveToScene(_file);
	SaveToFile(&m_offsetPos, _file);
	SaveToFile(&m_trackingType, _file);
}

void CTrackingUIScript::LoadFromScene(FILE* _file)
{
	CUIScript::LoadFromScene(_file);
	LoadFromFile(&m_offsetPos, _file);
	LoadFromFile(&m_trackingType, _file);
}

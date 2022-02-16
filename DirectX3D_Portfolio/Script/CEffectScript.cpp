#include "pch.h"
#include "CEffectScript.h"

#include <Engine\CAnimator2D.h>
#include <Engine\CAnimation2D.h>

CEffectScript::CEffectScript():
	CScript((UINT)SCRIPT_TYPE::EFFECTSCRIPT),
	m_effectColor(),
	m_startColor(),
	m_mode(0),
	m_ratio(1.f),
	m_bDecal(false),
	m_bBuffEffect(false),
	m_bAnimEffect(false),
	m_bAlphaOff(false),
	m_mainCam(nullptr),
	m_targetObj(nullptr)
{
	SetName(L"EffectScript");
}

CEffectScript::~CEffectScript()
{
}


void CEffectScript::start()
{
	if (m_sharedMaterial.Get() == nullptr || m_cloneMaterial.Get() == nullptr)
	{
		m_sharedMaterial = MeshRender()->GetSharedMaterial();
		m_cloneMaterial = MeshRender()->GetCloneMaterial();
	}
	
	if (nullptr != m_cloneMaterial.Get())
	{
		m_cloneMaterial->SetData(SHADER_PARAM::VEC4_0, &m_effectColor);
	}

	if (Animator2D())
	{
		m_bAnimEffect = true;
		m_bDecal = false;
		m_bBuffEffect = false;
		m_bAlphaOff = false;
		m_mainCam = CSceneManager::GetInst()->FindObjByName(L"MainCamera");
	}

	if (m_bBuffEffect)
	{
		m_mainCam = CSceneManager::GetInst()->FindObjByName(L"MainCamera");
	}
}

void CEffectScript::update()
{
	if (m_sharedMaterial.Get() == nullptr || m_cloneMaterial.Get() == nullptr)
	{
		m_sharedMaterial = MeshRender()->GetSharedMaterial();
		m_cloneMaterial = MeshRender()->GetCloneMaterial();
	}

	if (m_bAnimEffect)
	{
		if (nullptr != m_mainCam)
			Object()->Transform()->SetLocalRot(m_mainCam->Transform()->GetLocalRot());

		if (nullptr != m_targetObj)
		{
			Vec3 Pos = m_targetObj->Transform()->GetWorldPos();
			Pos.y += 100.f;
			Object()->Transform()->SetLocalPos(Pos);
		}

		if (Animator2D()->GetCurAnim()->IsFinish()) // && m_bDelete
		{
			// 이펙트를 onoff로 하는 경우

			Object()->SetRenderCheck(false);


			//삭제로 하는 경우
			if (!Object()->IsDead())
			{
				CScript::DeleteObject(Object());
			}

		}
	}

	if (m_bDecal)
	{
		if (m_mode == 1)
		{
			m_effectColor.y -= DELTATIME;
			m_effectColor.z -= DELTATIME;

			if (m_effectColor.y <= 0.f)
			{
				m_mode = 2;
			}
		}
		else if (m_mode == 2)
		{
			m_effectColor.x -= DELTATIME * 2.f;
			m_ratio -= DELTATIME * 2.f;

			if (m_ratio <= 0.f)
			{
				Object()->SetObjOff();
			}
		}

		m_cloneMaterial->SetData(SHADER_PARAM::VEC4_0, &m_effectColor);
		m_cloneMaterial->SetData(SHADER_PARAM::FLOAT_0, &m_ratio);
		m_cloneMaterial->SetData(SHADER_PARAM::INT_0, &m_mode);
	}

	if (m_bBuffEffect)
	{
		// 나중에 마지막에 이펙트 추가할 때 효과 추가 할거면 하기
		m_ratio -= DELTATIME;
		m_cloneMaterial->SetData(SHADER_PARAM::VEC4_0, &m_effectColor);
		m_cloneMaterial->SetData(SHADER_PARAM::FLOAT_0, &m_ratio);
		m_cloneMaterial->SetData(SHADER_PARAM::INT_0, &m_mode);

		if(nullptr != m_mainCam)
			Object()->Transform()->SetLocalRot(m_mainCam->Transform()->GetLocalRot());

		if (m_ratio <= 0.f)
		{
			Object()->SetObjOff();
		}
	}

	if (m_bAlphaOff)
	{
		m_ratio -= DELTATIME;
		m_cloneMaterial->SetData(SHADER_PARAM::VEC4_0, &m_effectColor);
		m_cloneMaterial->SetData(SHADER_PARAM::FLOAT_0, &m_ratio);

		if (m_ratio <= 0.f)
		{
			Object()->SetObjOff();
		}
	}
}

void CEffectScript::lateUpdate()
{
}

void CEffectScript::Reset()
{
	if (m_bDecal)
	{
		m_effectColor = m_startColor;
		m_mode = 1;
	}

	if (m_bBuffEffect)
	{
		m_effectColor = m_startColor;
	}

	m_ratio = 1.f;
	if (m_bAlphaOff)
	{
		m_cloneMaterial->SetData(SHADER_PARAM::FLOAT_0, &m_ratio);
		m_bAlphaOff = false;
	}
}

void CEffectScript::SetAnimMode()
{
	m_bAnimEffect = true;
	m_bDecal = false;
	m_bBuffEffect = false;
	m_bAlphaOff = false;
}

void CEffectScript::SetMaterialSetting()
{
	if (m_sharedMaterial.Get() == nullptr)
	{
		m_sharedMaterial = MeshRender()->GetSharedMaterial();
		m_cloneMaterial = MeshRender()->GetCloneMaterial();
	}
}

void CEffectScript::SetEffectColor(Vec3 _color)
{
	m_effectColor = _color;
	m_startColor = m_effectColor;

	if (nullptr != m_cloneMaterial.Get())
	{
		m_cloneMaterial->SetData(SHADER_PARAM::VEC4_0, &m_effectColor);
	}
}

void CEffectScript::SetDecalEffect(bool _bDecal)
{
	m_bDecal = _bDecal;

	m_bAnimEffect = false;
	m_bBuffEffect = false;

	Reset();
}

void CEffectScript::SetBuffEffectType()
{
	m_bDecal = false;
	m_bAnimEffect = false;
	m_bBuffEffect = true;

	Reset();
}

void CEffectScript::SaveToScene(FILE* _file)
{
	SaveToFile(&m_effectColor, _file);
	SaveToFile(&m_startColor, _file);
	SaveToFile(&m_ratio, _file);
	SaveToFile(&m_bDecal, _file);
	SaveToFile(&m_bBuffEffect, _file);
	SaveToFile(&m_bAnimEffect, _file);
	SaveToFile(&m_bAlphaOff, _file);
}

void CEffectScript::LoadFromScene(FILE* _file)
{
	LoadFromFile(&m_effectColor, _file);
	LoadFromFile(&m_startColor, _file);
	LoadFromFile(&m_ratio, _file);
	LoadFromFile(&m_bDecal, _file);
	LoadFromFile(&m_bBuffEffect, _file);
	LoadFromFile(&m_bAnimEffect, _file);
	LoadFromFile(&m_bAlphaOff, _file);
	
	Reset();
}
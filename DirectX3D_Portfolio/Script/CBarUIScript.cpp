#include "pch.h"
#include "CBarUIScript.h"
#include <Engine/CSceneManager.h>
#include <Engine/CMeshRender.h>
#include <Engine/CMaterial.h>

CBarUIScript::CBarUIScript() :
	CUIScript((UINT)SCRIPT_TYPE::BARUISCRIPT),
	m_barMode(BAR_MODE::RIGHT_TO_LEFT),
	m_ratio(1.f)
{
}

CBarUIScript::~CBarUIScript()
{
}

void CBarUIScript::start()
{
	CUIScript::start();

	if (MeshRender())
	{
		CMaterial* material = MeshRender()->GetCloneMaterial(0).Get();
		material->SetData(SHADER_PARAM::INT_0, &m_barMode);
	}
}

void CBarUIScript::update()
{
	CUIScript::update();
}

void CBarUIScript::lateUpdate()
{
	CUIScript::lateUpdate();

	CMaterial* material = MeshRender()->GetSharedMaterial(0).Get();
	material->SetData(SHADER_PARAM::FLOAT_0, &m_ratio);
}

void CBarUIScript::SaveToScene(FILE* _file)
{
	CUIScript::SaveToScene(_file);
	SaveToFile(&m_barMode, _file);
}

void CBarUIScript::LoadFromScene(FILE* _file)
{
	CUIScript::LoadFromScene(_file);
	LoadFromFile(&m_barMode, _file);
}

#include "pch.h"
#include "CGridScript.h"

CGridScript::CGridScript() :
	CScript((UINT)COMPONENT_TYPE::SCRIPT),
	m_toolCam(nullptr),
	m_thickness(0.1f),
	m_distance(0.f),
	m_maxStep(5),
	m_alpha(0.f),
	m_color(Vector4(1.f, 1.f, 1.f, 1.f))
{
}

CGridScript::~CGridScript()
{
}

void CGridScript::update()
{
	if (CSceneManager::GetInst()->GetSceneMode() == SCENE_MODE::PLAY)
		return;

	m_toolCamPos = m_toolCam->Transform()->GetWorldPos();
	Transform()->SetLocalPos(Vec3(m_toolCamPos.x, 0.f, m_toolCamPos.z));

	CalculateGrid();
}

void CGridScript::CalculateGrid()
{
	Ptr<CMaterial> material = MeshRender()->GetSharedMaterial();

	UINT height = (UINT)abs(m_toolCamPos.y);
	UINT step = 1;
	UINT i = 100;
	for (; step <= 4; i *= 10, ++step)
	{
		if (height <= i)
			break;
	}

	m_thickness = ((float)height * 0.01f);
	m_alpha = abs(1.f - (float)((double)height / pow(10.0, (double)step + 1.0)));

	material->SetData(SHADER_PARAM::INT_0, &step);
	material->SetData(SHADER_PARAM::FLOAT_0, &m_alpha);
	material->SetData(SHADER_PARAM::FLOAT_1, &m_thickness);
	material->SetData(SHADER_PARAM::VEC4_0, &m_toolCamPos);
	material->SetData(SHADER_PARAM::VEC4_1, &m_color);
}
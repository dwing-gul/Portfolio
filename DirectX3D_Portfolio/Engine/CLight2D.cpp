#include "pch.h"
#include "CLight2D.h"
#include "CTransform.h"
#include "CResourceManager.h"
#include "CRenderManager.h"

CLight2D::CLight2D() :
	CComponent(COMPONENT_TYPE::LIGHT2D),
	m_info{}
{
	m_mesh = CResourceManager::GetInst()->FindRes<CMesh>(L"CircleLineMesh");
	m_material = CResourceManager::GetInst()->FindRes<CMaterial>(L"Collider2DMaterial");

	m_info.lightType = LIGHT_TYPE::POINT;
}

CLight2D::~CLight2D()
{
}

void CLight2D::finalUpdate()
{
	m_info.lightPos = Transform()->GetWorldPos();
	CRenderManager::GetInst()->RegisterLight2D(this);
}

void CLight2D::SetRange(float _range)
{
	m_info.range = _range;
	Transform()->SetLocalScale(Vec3(_range * 2, _range * 2, 1.f));
}

void CLight2D::SetLightPos(Vec3 _pos)
{
	Transform()->SetLocalPos(_pos);
}

void CLight2D::render()
{
	Transform()->UpdateData();

	m_mesh->UpdateData(0);
	m_material->UpdateData();

	if (CRenderManager::GetInst()->GetMainCam() == CRenderManager::GetInst()->GetToolCam())
		m_mesh->render(0);
	m_material->Clear();
}

void CLight2D::SaveToScene(FILE* _file)
{
	CComponent::SaveToScene(_file);
	SaveToFile(&m_info, _file);
}

void CLight2D::LoadFromScene(FILE* _file)
{
	CComponent::LoadFromScene(_file);
	LoadFromFile(&m_info, _file);
}

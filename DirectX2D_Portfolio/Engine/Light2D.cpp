#include "pch.h"
#include "Light2D.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "KeyManager.h"

Light2D::Light2D() :
	Component(COMPONENT_TYPE::LIGHT2D),
	m_info{},
	m_bShowBoundary(false),
	m_bTool(false)
{
	m_mesh = ResourceManager::GetInst()->FindRes<Mesh>(L"CircleLineMesh");
	m_material = ResourceManager::GetInst()->FindRes<Material>(L"Collider2DMaterial");

	m_info.lightType = LIGHT_TYPE::POINT;
}

Light2D::~Light2D()
{
}

void Light2D::finalUpdate()
{
	if (m_bTool && RenderManager::GetInst()->GetMainCam() != RenderManager::GetInst()->GetToolCam())
	{
		m_bShowBoundary = false;
		m_bTool = false;
	}

	if (RenderManager::GetInst()->GetMainCam() == RenderManager::GetInst()->GetToolCam())
	{
		m_bTool = true;
		m_bShowBoundary = true;
	}
	else if (KEY_TAP(KEY::L))
	{
		m_bShowBoundary ? m_bShowBoundary = false : m_bShowBoundary = true;
	}

	m_info.lightPos = GetTransform()->GetWorldPos();
	RenderManager::GetInst()->RegisterLight2D(this);
}

void Light2D::render()
{
	GetTransform()->UpdateData();

	m_mesh->UpdateData();
	m_material->UpdateData();

	if (m_bShowBoundary)
	{
		m_mesh->render();
	}

	m_material->Clear();
}

void Light2D::SetLightPos(Vector3 _pos)
{
	GetTransform()->SetLocalPos(_pos);
}

void Light2D::SetRange(float _range)
{
	m_info.range = _range;
	GetTransform()->SetLocalScale(Vector3(_range * 2, _range * 2, 1.f));
}

void Light2D::SaveToScene(FILE* _file)
{
	Component::SaveToScene(_file);
	SaveToFile(&m_info, _file);
}

void Light2D::LoadFromScene(FILE* _file)
{
	Component::LoadFromScene(_file);
	LoadFromFile(&m_info, _file);
}

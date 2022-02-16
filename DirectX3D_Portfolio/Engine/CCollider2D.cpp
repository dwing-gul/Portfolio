#include "pch.h"
#include "CCollider2D.h"
#include "CResourceManager.h"
#include "CRenderManager.h"
#include "CSceneManager.h"
#include "CKeyManager.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CTransform.h"
#include "CMaterial.h"
#include "CCore.h"

vector<CMaterial*> g_vectorColliderMaterial;

CCollider2D::CCollider2D() :
	CComponent(COMPONENT_TYPE::COLLIDER2D),
	m_offsetPos(Vec2(0.f, 0.f)),
	m_offsetScale(Vec2(1.f, 1.f)),
	m_collisionCount(0),
	m_bCollision(false),
	m_hovered(0)
{
	m_mesh = CResourceManager::GetInst()->FindRes<CMesh>(L"RectLineMesh");
	m_material = CResourceManager::GetInst()->FindRes<CMaterial>(L"Collider2DMaterial");
}

CCollider2D::CCollider2D(const CCollider2D& _origin) :
	CComponent(_origin),
	m_offsetPos(_origin.m_offsetPos),
	m_offsetScale(_origin.m_offsetScale),
	m_colliderWorldMatrix(_origin.m_colliderWorldMatrix),
	m_mesh(_origin.m_mesh),
	m_material(_origin.m_material),
	m_collisionCount(0),
	m_bCollision(false),
	m_hovered(0)
{
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::update()
{
}

void CCollider2D::finalUpdate()
{
	Vec2 offsetPos = m_offsetPos / Transform()->GetWorldScale().XY();
	Matrix trans = XMMatrixTranslation(offsetPos.x, offsetPos.y, 0.f);
	Matrix scale = XMMatrixScaling(m_offsetScale.x, m_offsetScale.y, 1.f);

	m_colliderWorldMatrix = scale * trans * Transform()->GetWorldMatrix();
	m_colliderWorldMatrix._43 = 0.f;
}

void CCollider2D::render()
{
	if (nullptr == m_mesh || nullptr == m_material)
		return;

	UpdateData();

	m_mesh->UpdateData(0);
	m_material->UpdateData();

	m_mesh->render(0);

	m_material->Clear();
}

void CCollider2D::UpdateData()
{
	static const CConstBuffer* CB = CDevice::GetInst()->GetCB(CONST_BUFFER::TRANSFORM);

	g_transform.worldMatrix = m_colliderWorldMatrix;
	g_transform.matrixWV = g_transform.worldMatrix * g_transform.viewMatrix;
	g_transform.matrixWVP = g_transform.matrixWV * g_transform.projectionMatrix;

	CB->SetData(&g_transform);
	CB->UpdateData(SHADER_STAGE::Vertex);
}

void CCollider2D::OnColliderEnter(CCollider2D* _other)
{
	if (0 == m_collisionCount)
	{
		if (g_vectorColliderMaterial.empty())
		{
			g_vectorColliderMaterial.push_back(m_material->Clone());
		}

		m_material = g_vectorColliderMaterial.back();
		g_vectorColliderMaterial.pop_back();

		m_material->SetData(SHADER_PARAM::INT_0, &m_collisionCount);
	}

	++m_collisionCount;
}

void CCollider2D::OnCollider(CCollider2D* _other)
{
}

void CCollider2D::OnColliderExit(CCollider2D* _other)
{
	--m_collisionCount;

	if (0 == m_collisionCount)
	{
		g_vectorColliderMaterial.push_back(m_material.Get());
		m_material = CResourceManager::GetInst()->FindRes<CMaterial>(L"Collider2DMaterial");
	}
}

void CCollider2D::SaveToScene(FILE* _file)
{
	CComponent::SaveToScene(_file);
	SaveToFile(&m_offsetPos, _file);
	SaveToFile(&m_offsetScale, _file);
}

void CCollider2D::LoadFromScene(FILE* _file)
{
	CComponent::LoadFromScene(_file);
	LoadFromFile(&m_offsetPos, _file);
	LoadFromFile(&m_offsetScale, _file);
}

bool CCollider2D::IsObjectHovered()
{
	if (nullptr == this)
		return false;

	float xHalf = (Transform()->GetLocalScale().x * m_offsetScale.x) / 2.f;
	float yHalf = (Transform()->GetLocalScale().y * m_offsetScale.y) / 2.f;
	Vec3 targetPos = Transform()->GetLocalPos();
	Vec2 mousePos = CKeyManager::GetInst()->GetMousePos();
	Vec2 resolution = CCore::GetInst()->GetWindowResolution();
	Vec4 pos = Vec4(targetPos.x - xHalf, targetPos.x + xHalf, targetPos.y - yHalf, targetPos.y + yHalf);

	mousePos = Vec2(mousePos.x - resolution.x / 2.f, -(mousePos.y - resolution.y / 2.f));

	if (pos.x < mousePos.x && mousePos.x < pos.y && pos.z < mousePos.y && mousePos.y < pos.w)
	{
		m_hovered = 1;
		if (g_vectorColliderMaterial.empty())
		{
			g_vectorColliderMaterial.push_back(m_material->Clone());
		}

		m_material = g_vectorColliderMaterial.back();
		g_vectorColliderMaterial.pop_back();

		m_material->SetData(SHADER_PARAM::INT_1, &m_hovered);
		return true;
	}

	if (1 == m_hovered)
	{
		m_hovered = 0;
		m_material = CResourceManager::GetInst()->FindRes<CMaterial>(L"Collider2DMaterial");
	}

	return false;
	return false;
}

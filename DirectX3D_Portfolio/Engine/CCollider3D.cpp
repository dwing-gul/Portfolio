#include "pch.h"
#include "CCollider3D.h"
#include "CTransform.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CRenderManager.h"
#include "CKeyManager.h"
#include "CScript.h"

vector<CMaterial*> g_vectorCollider3DMaterial;

CCollider3D::CCollider3D() :
	CComponent(COMPONENT_TYPE::COLLIDER3D),
	m_offsetPos(Vec3(0.f, 0.f, 0.f)),
	m_offsetScale(Vec3(1.f, 1.f, 1.f)),
	m_collisionCount(0),
	m_bAdjust(false),
	m_bCollision(false),
	m_bSphere(false),
	m_bOn(true),
	m_target(nullptr)
{
	m_mesh = CResourceManager::GetInst()->FindRes<CMesh>(L"CubeLineMesh");
	m_material = CResourceManager::GetInst()->FindRes<CMaterial>(L"Collider3DMaterial");
}

CCollider3D::CCollider3D(const CCollider3D& _origin) :
	CComponent(_origin),
	m_offsetPos(_origin.m_offsetPos),
	m_offsetScale(_origin.m_offsetScale),
	m_colliderWorldMatrix(_origin.m_colliderWorldMatrix),
	m_bSphere(_origin.m_bSphere),
	m_collisionCount(0),
	m_bAdjust(_origin.m_bAdjust),
	m_bCollision(false),
	m_bOn(true),
	m_target(nullptr)
{
	m_mesh = CResourceManager::GetInst()->FindRes<CMesh>(L"CubeLineMesh");
	m_material = CResourceManager::GetInst()->FindRes<CMaterial>(L"Collider3DMaterial");
}

CCollider3D::~CCollider3D()
{
}

void CCollider3D::update()
{
}

void CCollider3D::finalUpdate()
{
	Vec3 offsetPos = m_offsetPos / Transform()->GetWorldScale();

	// 충돌체의 y값 위치 조정
	if (m_bAdjust)
	{
		offsetPos.y *= Transform()->GetLocalScale().x;
	}

	Matrix trans = XMMatrixTranslation(offsetPos.x, offsetPos.y, offsetPos.z);
	Matrix scale = XMMatrixScaling(m_offsetScale.x, m_offsetScale.y, m_offsetScale.z);

	
	m_colliderWorldMatrix = scale * trans * Transform()->GetWorldMatrix();
	
	m_vecScript = Object()->GetScripts();
}

void CCollider3D::render()
{
	if (nullptr == m_mesh || nullptr == m_material)
		return;

	UpdateData();

	m_mesh->UpdateData(0);
	m_material->UpdateData();

	m_mesh->render(0);

	m_material->Clear();
}

void CCollider3D::UpdateData()
{
	static const CConstBuffer* CB = CDevice::GetInst()->GetCB(CONST_BUFFER::TRANSFORM);

	g_transform.worldMatrix = m_colliderWorldMatrix;
	g_transform.matrixWV = g_transform.worldMatrix * g_transform.viewMatrix;
	g_transform.matrixWVP = g_transform.matrixWV * g_transform.projectionMatrix;

	CB->SetData(&g_transform);
	CB->UpdateData(SHADER_STAGE::Vertex);
}

void CCollider3D::SetOffsetScale(Vec3 _scale)
{
	m_offsetScale = _scale;
	m_worldOffsetScale = m_offsetScale * Object()->Transform()->GetWorldScale();
}

void CCollider3D::SetSphere(bool _bSphere)
{
	m_bSphere = _bSphere;

	if (m_bSphere)
	{
		m_mesh = CResourceManager::GetInst()->FindRes<CMesh>(L"SphereMesh");
	}
	else
	{
		m_mesh = CResourceManager::GetInst()->FindRes<CMesh>(L"CubeLineMesh");
	}
}

void CCollider3D::OnColliderEnter(CCollider3D* _other)
{
	if (0 == m_collisionCount)
	{
		if (g_vectorCollider3DMaterial.empty())
		{
			g_vectorCollider3DMaterial.push_back(m_material->Clone());
		}

		m_material = g_vectorCollider3DMaterial.back();
		g_vectorCollider3DMaterial.pop_back();

		int a = 1;
		m_material->SetData(SHADER_PARAM::INT_0, &a);
	}

	for (int i = 0; i < m_vecScript.size(); i++)
	{
		m_vecScript[i]->OnColliderEnter(_other);
	}
	

	m_bCollision = true;
	m_target = _other;
	++m_collisionCount;
}

void CCollider3D::OnCollider(CCollider3D* _other)
{
	for (int i = 0; i < m_vecScript.size(); i++)
	{
		m_vecScript[i]->OnCollider(_other);
	}
}

void CCollider3D::OnColliderExit(CCollider3D* _other)
{
	--m_collisionCount;

	if (0 == m_collisionCount)
	{
		g_vectorCollider3DMaterial.push_back(m_material.Get());
		m_material = CResourceManager::GetInst()->FindRes<CMaterial>(L"Collider3DMaterial");
	}

	for (int i = 0; i < m_vecScript.size(); i++)
	{
		m_vecScript[i]->OnColliderExit(_other);
	}

	m_bCollision = false;
	m_target = nullptr;
}

void CCollider3D::SaveToScene(FILE* _file)
{
	CComponent::SaveToScene(_file);
	SaveToFile(&m_offsetPos, _file);
	SaveToFile(&m_offsetScale, _file);
	SaveToFile(&m_bAdjust, _file);
}

void CCollider3D::LoadFromScene(FILE* _file)
{
	CComponent::LoadFromScene(_file);
	LoadFromFile(&m_offsetPos, _file);
	LoadFromFile(&m_offsetScale, _file);
	LoadFromFile(&m_bAdjust, _file);
}
#include "pch.h"
#include "Collider2D.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "KeyManager.h"
#include "Device.h"
#include "ConstBuffer.h"
#include "Transform.h"

vector<Material*> g_vectorColliderMaterial;

Collider2D::Collider2D() :
	Component(COMPONENT_TYPE::COLLIDER2D),
	m_offsetPos(Vector2(0.f, 0.f)),
	m_offsetScale(Vector2(1.f, 1.f)),
	m_type(COLLIDER_TYPE::OBJECT),
	m_collisionCount(0),
	m_bShow(false),
	m_bTool(false),
	m_bCollision(false),
	m_hovered(0)
{
	m_mesh = ResourceManager::GetInst()->FindRes<Mesh>(L"RectLineMesh");
	m_material = ResourceManager::GetInst()->FindRes<Material>(L"Collider2DMaterial");
}

Collider2D::Collider2D(const Collider2D& _origin) :
	Component(_origin),
	m_offsetPos(_origin.m_offsetPos),
	m_offsetScale(_origin.m_offsetScale),
	m_colliderWorldMatrix(_origin.m_colliderWorldMatrix),
	m_type(_origin.m_type),
	m_mesh(nullptr),
	m_material(nullptr),
	m_collisionCount(0),
	m_bShow(false),
	m_bTool(false),
	m_bCollision(false)
{
	m_mesh = ResourceManager::GetInst()->FindRes<Mesh>(L"RectLineMesh");
	m_material = ResourceManager::GetInst()->FindRes<Material>(L"Collider2DMaterial");
}

Collider2D::~Collider2D()
{
}

void Collider2D::finalUpdate()
{
	if (m_bTool && RenderManager::GetInst()->GetMainCam() != RenderManager::GetInst()->GetToolCam())
	{
		m_bShow = false;
		m_bTool = false;
	}

	if (RenderManager::GetInst()->GetMainCam() == RenderManager::GetInst()->GetToolCam())
	{
		m_bTool = true;
		m_bShow = true;
	}
	else if (KEY_TAP(KEY::C))
	{
		m_bShow ? m_bShow = false : m_bShow = true;
	}

	Vector2 offsetPos = m_offsetPos / GetTransform()->GetWorldScale().XY();
	Matrix trans = XMMatrixTranslation(offsetPos.x, offsetPos.y, 0.f);
	Matrix scale = XMMatrixScaling(m_offsetScale.x, m_offsetScale.y, 1.f);

	m_colliderWorldMatrix = scale * trans * GetTransform()->GetWorldMatrix();
	m_colliderWorldMatrix._43 = 0.f;
}

void Collider2D::render()
{
	if (nullptr == m_mesh || nullptr == m_material)
		return;

	UpdateData();

	m_mesh->UpdateData();
	m_material->UpdateData();

	if (m_bShow)
	{
		m_mesh->render();
	}

	m_material->Clear();
}

void Collider2D::UpdateData()
{
	static const ConstBuffer* CB = Device::GetInst()->GetCB(CONST_BUFFER::TRANSFORM);

	g_transform.worldMatrix = m_colliderWorldMatrix;
	g_transform.matrixWV = g_transform.worldMatrix * g_transform.viewMatrix;
	g_transform.matrixWVP = g_transform.matrixWV * g_transform.projectionMatrix;

	CB->SetData(&g_transform);
	CB->UpdateData(SHADER_STAGE::Vertex);
}

void Collider2D::OnColliderEnter(Collider2D* _other)
{
	if (0 == m_collisionCount)
	{
		if (g_vectorColliderMaterial.empty())
		{
			g_vectorColliderMaterial.push_back(m_material->Clone());
		}

		m_material = g_vectorColliderMaterial.back();
		g_vectorColliderMaterial.pop_back();

		int collision = 1;
		m_material->SetData(SHADER_PARAM::INT_0, &collision);
	}

	m_bCollision = true;
	++m_collisionCount;
}

void Collider2D::OnCollider(Collider2D* _other)
{
}

void Collider2D::OnColliderExit(Collider2D* _other)
{
	m_bCollision = false;
	--m_collisionCount;

	if (0 == m_collisionCount)
	{
		m_material = ResourceManager::GetInst()->FindRes<Material>(L"Collider2DMaterial");
	}
}

void Collider2D::SaveToScene(FILE* _file)
{
	Component::SaveToScene(_file);
	SaveToFile(&m_offsetPos, _file);
	SaveToFile(&m_offsetScale, _file);
	SaveToFile(&m_type, _file);
}

void Collider2D::LoadFromScene(FILE* _file)
{
	Component::LoadFromScene(_file);
	LoadFromFile(&m_offsetPos, _file);
	LoadFromFile(&m_offsetScale, _file);
	LoadFromFile(&m_type, _file);
}

bool Collider2D::IsObjectHovered()
{
	if (nullptr == this)
		return false;

	float xHalf = m_colliderWorldMatrix._11 / 2.f;
	float yHalf = m_colliderWorldMatrix._22 / 2.f;
	Vector4 pos = Vector4(m_colliderWorldMatrix._41 - xHalf, m_colliderWorldMatrix._41 + xHalf, 
		m_colliderWorldMatrix._42 - yHalf, m_colliderWorldMatrix._42 + yHalf);
	Vector3 mousePos = KeyManager::GetInst()->GetMousePos();
	GameObject* camera = SceneManager::GetInst()->FindObjByName(L"Camera");

	if (nullptr != camera)
	{
		Vector3 cameraPos = camera->GetTransform()->GetLocalPos();
		mousePos += cameraPos;
	}

	if (pos.x < mousePos.x && mousePos.x < pos.y && pos.z < mousePos.y && mousePos.y < pos.w)
	{
		m_hovered = 1;
		if (g_vectorColliderMaterial.empty())
		{
			g_vectorColliderMaterial.push_back(m_material->Clone());
		}

		m_material = g_vectorColliderMaterial.back();
		g_vectorColliderMaterial.pop_back();

		m_material->SetData(SHADER_PARAM::INT_0, &m_hovered);
		return true;
	}

	if (1 == m_hovered)
	{
		m_hovered = 0;
		m_material = ResourceManager::GetInst()->FindRes<Material>(L"Collider2DMaterial");
	}
	
	return false;
}
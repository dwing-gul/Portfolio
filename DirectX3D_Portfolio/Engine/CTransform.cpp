#include "pch.h"
#include "CTransform.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform() :
	CComponent(COMPONENT_TYPE::TRANSFORM),
	m_localScale(Vec3(1.f, 1.f, 1.f)),
	m_boundingRange(0.f),
	m_bFrustumCheck(false),
	m_bLocalUpdateData(false),
	m_bSetWorld(false)
{
}

CTransform::~CTransform()
{
}

void CTransform::finalUpdate()
{
	Matrix scaleMatrix = XMMatrixScaling(m_localScale.x, m_localScale.y, m_localScale.z);
	
	Matrix rotXMatrix = XMMatrixRotationX(m_localRot.x);
	Matrix rotYMatrix = XMMatrixRotationY(m_localRot.y);
	Matrix rotZMatrix = XMMatrixRotationZ(m_localRot.z);

	Matrix rotMatrix = rotXMatrix * rotYMatrix * rotZMatrix;

	m_worldDir[(UINT)DIR_TYPE::RIGHT] = m_localDir[(UINT)DIR_TYPE::RIGHT] = XMVector3Normalize(XMVector3TransformNormal(Vec3::Right, rotMatrix));
	m_worldDir[(UINT)DIR_TYPE::UP] = m_localDir[(UINT)DIR_TYPE::UP] = XMVector3Normalize(XMVector3TransformNormal(Vec3::Up, rotMatrix));
	m_worldDir[(UINT)DIR_TYPE::FRONT] = m_localDir[(UINT)DIR_TYPE::FRONT] = XMVector3Normalize(XMVector3TransformNormal(Vec3::Front, rotMatrix));

	Matrix transMatrix = XMMatrixTranslation(m_localPos.x, m_localPos.y, m_localPos.z);

	m_worldMatrix = m_localMatrix = scaleMatrix * rotMatrix * transMatrix;

	if (m_bSetWorld)
	{
		m_worldMatrix = m_newWorldMatrix;
	}
	
	CGameObject* Obj = Object();

	if (Object()->GetParent())
	{
		const Matrix& parentWorldMatrix = Object()->GetParent()->Transform()->GetWorldMatrix();
		m_worldMatrix *= parentWorldMatrix;

		m_worldDir[(UINT)DIR_TYPE::RIGHT] = XMVector3Normalize(XMVector3TransformNormal(Vec3::Right, m_worldMatrix));
		m_worldDir[(UINT)DIR_TYPE::UP] = XMVector3Normalize(XMVector3TransformNormal(Vec3::Up, m_worldMatrix));
		m_worldDir[(UINT)DIR_TYPE::FRONT] = XMVector3Normalize(XMVector3TransformNormal(Vec3::Front, m_worldMatrix));
	}

	m_worldMatrixInverse = XMMatrixInverse(nullptr, m_worldMatrix);
}

void CTransform::UpdateData()
{
	static const CConstBuffer* CB = CDevice::GetInst()->GetCB(CONST_BUFFER::TRANSFORM);
	if (!m_bLocalUpdateData)
	{
		g_transform.worldMatrix = m_worldMatrix;
		g_transform.worldMatrixInverse = m_worldMatrixInverse;
		g_transform.matrixWV = g_transform.worldMatrix * g_transform.viewMatrix;
		g_transform.matrixWVP = g_transform.matrixWV * g_transform.projectionMatrix;
		g_transform.matrixVP = g_transform.viewMatrix * g_transform.projectionMatrix;

		CB->SetData(&g_transform);
		CB->UpdateData(SHADER_STAGE::GRAPHICS);
	}
	else
	{
		g_transform.worldMatrix = m_localMatrix;
		g_transform.worldMatrixInverse = m_worldMatrixInverse;
		g_transform.matrixWV = g_transform.worldMatrix * g_transform.viewMatrix;
		g_transform.matrixWVP = g_transform.matrixWV * g_transform.projectionMatrix;
		g_transform.matrixVP = g_transform.viewMatrix * g_transform.projectionMatrix;

		CB->SetData(&g_transform);
		CB->UpdateData(SHADER_STAGE::GRAPHICS);
	}
}

Vec3 CTransform::GetWorldScale()
{
	Vec3 worldScale = m_localScale;
	CGameObject* object = Object();

	while (object->GetParent())
	{
		worldScale *= object->GetParent()->Transform()->GetLocalScale();
		object = object->GetParent();
	}

	return worldScale;
}

void CTransform::SaveToScene(FILE* _file)
{
	CComponent::SaveToScene(_file);

	SaveToFile(&m_localPos, _file);
	SaveToFile(&m_localScale, _file);
	SaveToFile(&m_localRot, _file);
	SaveToFile(&m_boundingRange, _file);
	SaveToFile(&m_bFrustumCheck, _file);
}

void CTransform::LoadFromScene(FILE* _file)
{
	CComponent::LoadFromScene(_file);

	LoadFromFile(&m_localPos, _file);
	LoadFromFile(&m_localScale, _file);
	LoadFromFile(&m_localRot, _file);
	LoadFromFile(&m_boundingRange, _file);
	LoadFromFile(&m_bFrustumCheck, _file);
}
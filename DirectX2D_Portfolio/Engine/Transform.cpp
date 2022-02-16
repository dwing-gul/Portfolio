#include "pch.h"
#include "Transform.h"
#include "Device.h"
#include "ConstBuffer.h"

Transform::Transform() :
	Component(COMPONENT_TYPE::TRANSFORM),
	m_localPos(Vector3(0.f, 0.f, 0.f)),
	m_localScale(Vector3(1.f, 1.f, 1.f)),
	m_localRot(Vector3(0.f, 0.f, 0.f))
{
}

Transform::~Transform()
{
}

void Transform::finalUpdate()
{
	Matrix scaleMatrix = XMMatrixScaling(m_localScale.x, m_localScale.y, m_localScale.z);

	Matrix rotXMatrix = XMMatrixRotationX(m_localRot.x);
	Matrix rotYMatrix = XMMatrixRotationY(m_localRot.y);
	Matrix rotZMatrix = XMMatrixRotationZ(m_localRot.z);
	Matrix rotMatrix = rotXMatrix * rotYMatrix * rotZMatrix;

	m_worldDir[(UINT)DIR_TYPE::RIGHT] = m_localDir[(UINT)DIR_TYPE::RIGHT] = XMVector3Normalize(XMVector3TransformNormal(Vector3::Right, rotMatrix));
	m_worldDir[(UINT)DIR_TYPE::UP] = m_localDir[(UINT)DIR_TYPE::UP] = XMVector3Normalize(XMVector3TransformNormal(Vector3::Up, rotMatrix));
	m_worldDir[(UINT)DIR_TYPE::FRONT] = m_localDir[(UINT)DIR_TYPE::FRONT] = XMVector3Normalize(XMVector3TransformNormal(Vector3::Front, rotMatrix));

	Matrix transMatrix = XMMatrixTranslation(m_localPos.x, m_localPos.y, m_localPos.z);

	m_worldMatrix = m_localMatrix = scaleMatrix * rotMatrix * transMatrix;

	if (Object()->GetParent())
	{
		const Matrix& parentWorldMatrix = Object()->GetParent()->GetTransform()->GetWorldMatrix();
		m_worldMatrix *= parentWorldMatrix;

		m_worldDir[(UINT)DIR_TYPE::RIGHT] = XMVector3Normalize(XMVector3TransformNormal(Vector3::Right, m_worldMatrix));
		m_worldDir[(UINT)DIR_TYPE::UP] = XMVector3Normalize(XMVector3TransformNormal(Vector3::Up, m_worldMatrix));
		m_worldDir[(UINT)DIR_TYPE::FRONT] = XMVector3Normalize(XMVector3TransformNormal(Vector3::Front, m_worldMatrix));
	}
}

void Transform::UpdateData()
{
	static const ConstBuffer* CB = Device::GetInst()->GetCB(CONST_BUFFER::TRANSFORM);

	g_transform.worldMatrix = m_worldMatrix;
	g_transform.matrixWV = g_transform.worldMatrix * g_transform.viewMatrix;
	g_transform.matrixWVP = g_transform.matrixWV * g_transform.projectionMatrix;

	CB->SetData(&g_transform);
	CB->UpdateData(SHADER_STAGE::NONE_PIXEL);
}

Vector3 Transform::GetWorldScale()
{
	Vector3 worldScale = m_localScale;
	GameObject* object = Object();

	while (object->GetParent())
	{
		worldScale *= object->GetParent()->GetTransform()->GetLocalScale();
		object = object->GetParent();
	}

	return worldScale;
}

void Transform::SaveToScene(FILE* _file)
{
	Component::SaveToScene(_file);

	SaveToFile(&m_localPos, _file);
	SaveToFile(&m_localScale, _file);
	SaveToFile(&m_localRot, _file);
}

void Transform::LoadFromScene(FILE* _file)
{
	Component::LoadFromScene(_file);

	LoadFromFile(&m_localPos, _file);
	LoadFromFile(&m_localScale, _file);
	LoadFromFile(&m_localRot, _file);
}

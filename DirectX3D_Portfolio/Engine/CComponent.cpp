#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(COMPONENT_TYPE _type) :
	m_obj(nullptr),
	m_type(_type)
{
}

CComponent::CComponent(const CComponent& _origin) :
	CEntity(_origin),
	m_obj(nullptr),
	m_type(_origin.m_type)
{
}

CComponent::~CComponent()
{
}

void CComponent::SaveToScene(FILE* _file)
{
	CEntity::SaveToScene(_file);
}

void CComponent::LoadFromScene(FILE* _file)
{
	CEntity::LoadFromScene(_file);
}

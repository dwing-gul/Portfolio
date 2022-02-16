#include "pch.h"
#include "Component.h"

Component::Component(COMPONENT_TYPE _type) :
	m_obj(nullptr),
	m_type(_type)
{
}

Component::Component(const Component& _origin) :
	Entity(_origin),
	m_obj(nullptr),
	m_type(_origin.m_type)
{
}

Component::~Component()
{
}

void Component::SaveToScene(FILE* _file)
{
	Entity::SaveToScene(_file);
}

void Component::LoadFromScene(FILE* _file)
{
	Entity::LoadFromScene(_file);
}

#include "pch.h"
#include "Entity.h"

UINT Entity::g_nextID = 0;

Entity::Entity() :
	m_ID(g_nextID++)
{
}

Entity::Entity(const Entity& _origin) :
	m_name(_origin.m_name),
	m_ID(g_nextID++)
{
}

Entity::~Entity()
{
}

void Entity::SaveToScene(FILE* _file)
{
	SaveWStringToFile(m_name, _file);
}

void Entity::LoadFromScene(FILE* _file)
{
	LoadWStringFromFile(m_name, _file);
}
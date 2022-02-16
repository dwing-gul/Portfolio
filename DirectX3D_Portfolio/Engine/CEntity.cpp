#include "pch.h"
#include "CEntity.h"

UINT CEntity::g_nextID = 0;

CEntity::CEntity() :
    m_ID(g_nextID++)
{
}

CEntity::CEntity(const CEntity& _origin) :
    m_name(_origin.m_name),
    m_ID(g_nextID++)
{
}

CEntity::~CEntity()
{
}

void CEntity::SaveToScene(FILE* _file)
{
    SaveWStringToFile(m_name, _file);
}

void CEntity::LoadFromScene(FILE* _file)
{
    LoadWStringFromFile(m_name, _file);
}
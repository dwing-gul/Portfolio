#include "pch.h"
#include "CPrefab.h"
#include "CGameObject.h"
#include "CPathManager.h"

CPrefab::CPrefab(CGameObject* _proto) :
	CRes(RESOURCE_TYPE::PREFAB),
	m_protoObj(_proto)
{
	assert(m_protoObj);
}

CPrefab::~CPrefab()
{
	Safe_Delete_Ptr(m_protoObj);
}

#include "CCollider3D.h"
#include "CMeshRender.h"
#include "CTransform.h"

CGameObject* CPrefab::Instantiate()
{
	assert(m_protoObj);
	
	return m_protoObj->Clone();
}

void CPrefab::Save(const wstring& _relativePath)
{
	wstring filePath = CPathManager::GetInst()->GetContentPath();
	filePath += _relativePath;

	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"wb");
	assert(file);

	m_protoObj->SaveToScene(file);

	fclose(file);
}

int CPrefab::Load(const wstring& _filePath)
{
	FILE* file = nullptr;
	_wfopen_s(&file, _filePath.c_str(), L"rb");
	assert(file);

	m_protoObj = new CGameObject;
	m_protoObj->LoadFromScene(file, 0);

	fclose(file);

	return 0;
}
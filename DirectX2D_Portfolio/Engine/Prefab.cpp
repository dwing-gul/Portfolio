#include "pch.h"
#include "Prefab.h"
#include "GameObject.h"
#include "PathManager.h"

Prefab::Prefab(GameObject* _proto) :
    Res(RESOURCE_TYPE::PREFAB),
    m_protoObj(_proto)
{
    assert(m_protoObj);
}

Prefab::~Prefab()
{
    Safe_Delete(m_protoObj);
}

GameObject* Prefab::Instantiate()
{
    assert(m_protoObj);
    return m_protoObj->Clone();
}

void Prefab::Save(const wstring& _relativePath)
{
    wstring filePath = PathManager::GetInst()->GetContentPath();
    filePath += _relativePath;

    FILE* file = nullptr;
    _wfopen_s(&file, filePath.c_str(), L"wb");
    assert(file);

    m_protoObj->SaveToScene(file);

    fclose(file);
}

int Prefab::Load(const wstring& _filePath)
{
    FILE* file = nullptr;
    _wfopen_s(&file, _filePath.c_str(), L"rb");
    assert(file);

    m_protoObj = new GameObject;
    m_protoObj->LoadFromScene(file, 0);

    fclose(file);

    return S_OK;
}

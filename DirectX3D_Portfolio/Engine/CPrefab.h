#pragma once
#include "CRes.h"

class CGameObject;

class CPrefab :
    public CRes
{
private:
    CGameObject* m_protoObj;

public:
    CGameObject* Instantiate();
    CGameObject*& GetPrefabObject() { return m_protoObj; }

    virtual void Save(const wstring& _relativePath);
    virtual int Load(const wstring& _filePath);

    CLONE_DISABLE(CPrefab);

private:
    CPrefab() :
        CRes(RESOURCE_TYPE::PREFAB),
        m_protoObj(nullptr)
    {}

public:
    CPrefab(CGameObject* _proto);
    ~CPrefab();

    friend class CResourceManager;
};


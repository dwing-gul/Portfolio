#pragma once
#include "Res.h"

class GameObject;

class Prefab :
    public Res
{
private :
    GameObject* m_protoObj;

public :
    GameObject* Instantiate();

    GameObject*& GetPrefabObject() { return m_protoObj; }

    virtual void Save(const wstring& _relativePath);
    virtual int Load(const wstring& _filePath);

    CLONE_DISABLE(Prefab)

private :
    Prefab() :
        Res(RESOURCE_TYPE::PREFAB),
        m_protoObj(nullptr)
    {}

public :
    Prefab(GameObject* _proto);
    ~Prefab();

    friend class ResourceManager;
};


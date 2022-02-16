#pragma once
#include "CComponent.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CResourceManager.h"
#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

class CScript :
    public CComponent
{
protected:
    UINT m_scriptType;

public:
    virtual void finalUpdate() final {}
    virtual void render() {}

    static void CreateObject(CGameObject* _newObj, Vec3 _worldPos, int _layerIndex = 0);
    static void CreateObject(CGameObject* _newObj, int _layerIndex = 0);
    static void DeleteObject(CGameObject* _target);
    static void AddChild(CGameObject* _parent, CGameObject* _child);
    static void AddChildByName(const wstring& _parentName, CGameObject* _child);
    static void DisconnectWithParent(CGameObject* _child);
    static void Instantiate(Ptr<CPrefab> _prefab, Vec3 _worldPos, int _layerIndex = 0);

    UINT GetScriptType() { return m_scriptType; }

    virtual void OnColliderEnter(CCollider3D* _other) {}
    virtual void OnCollider(CCollider3D* _other) {}
    virtual void OnColliderExit(CCollider3D* _other) {}

    virtual void SaveToScene(FILE* _file) {}
    virtual void LoadFromScene(FILE* _file) {}

    virtual CScript* Clone() = 0;

public:
    CScript(UINT _type);
    ~CScript();
};


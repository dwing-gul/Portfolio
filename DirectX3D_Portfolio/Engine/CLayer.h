#pragma once
#include "CEntity.h"
class CGameObject;
class CScene;

class CLayer :
    public CEntity
{
private:
    CScene*                 m_ownerScene;
    vector<CGameObject*>    m_vecParentObj;
    vector<CGameObject*>    m_vecObj;
    int                     m_layerIndex;
    bool                    m_layerAction;

public:
    void start();
    void update();
    void lateUpdate();
    void finalUpdate();

    void AddObject(CGameObject* _obj, bool _bWithChild);
    void RegisterObj(CGameObject* _obj) { m_vecObj.push_back(_obj); }
    void DeregisterAsParent(CGameObject* _obj);

    const vector<CGameObject*>& GetParentObj() { return m_vecParentObj; }
    const vector<CGameObject*>& GetAllObj() { return m_vecObj; }

public:
    void SetLayerAction(bool action) { m_layerAction = action; }
    bool GetLayerAction() { return m_layerAction; }

private:
    void ResignObject(CGameObject* _obj);
    void RegisterAsParent(CGameObject* _obj) { m_vecParentObj.push_back(_obj); }

public:
    void SaveToScene(FILE* _file);
    void LoadFromScene(FILE* _file);

    CLONE_DISABLE(CLayer);

public:
    CLayer();
    ~CLayer();

    friend class CScene;
    friend class CEventManager;
};

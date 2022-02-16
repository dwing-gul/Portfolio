#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;

enum class SCENE_TYPE
{
    TOOL,
    LOADING_SCENE,
    PLAY_STAGE
};

class CScene :
    public CEntity
{
private:
    CLayer*                 m_arrLayer[MAX_LAYER];
    SCENE_TYPE              m_type;
    vector<CGameObject*>    m_vecTarget; // 타입을 통해 찾은 오브젝트들을 임시로 담을 벡터

public:
    void start();
    void update();
    void lateUpdate();
    void finalUpdate();

    void SetSceneType(SCENE_TYPE _type) { m_type = _type; }
    SCENE_TYPE GetSceneType() { return m_type; }
    CLayer* GetLayer(int _index)
    {
        assert(!(_index < 0 || MAX_LAYER <= _index));
        return m_arrLayer[_index];
    }

    CGameObject* FindObjByName(const wstring& _name);
    vector<CGameObject*> FindObjByName_Vector(const wstring& _name);
    const vector<CGameObject*>& FindObjByType(OBJECT_TYPE _type, bool _bChild = false);
    const vector<CGameObject*>& FindObjByType(OBJECT_TYPE _type, int _layerIndex, bool _bChild = false);
    const vector<CGameObject*>& FindObjByLayerIndex(int _layerIndex, bool _bChild = false);
    vector<CGameObject*>& FindObjByType_Ref(OBJECT_TYPE _type, bool _bChild = false);

private:
    void AddObject(CGameObject* _obj, int _layerIndex, bool _bWithChild = true);
    void DeregisterObject();
    void SearchObjBytype(CGameObject* _obj, OBJECT_TYPE _type);
    void SearchChild(CGameObject* _obj);

public:
    void SaveToScene(FILE* _file);
    void LoadFromScene(FILE* _file);

    CLONE_DISABLE(CScene);

public:
    CScene();
    ~CScene();

    friend class CSceneManager;
    friend class CEventManager;
};
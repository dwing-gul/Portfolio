#pragma once
#include "CEntity.h"
#include "CNaviTileNode.h"

class CComponent;
class CTransform;
class CMeshRender;
class CCamera;
class CCollider2D;
class CCollider3D;
class CAnimator2D;
class CAnimator3D;
class CLight2D;
class CLight3D;
class CTerrain;
class CTileMap;
class CNaviMesh;
class CParticleSystem;
class CScript;

class CGameObject :
    public CEntity
{
private:
    CComponent* m_arrComponent[(UINT)COMPONENT_TYPE::END];
    vector<CScript*> m_vecScript;
    CGameObject* m_parent;
    vector<CGameObject*> m_vecChild;
    vector<wstring> m_vecChildName;
    OBJECT_TYPE         m_objType;
    tRay m_ray; // 캐릭터와 터레인의 위치를 비교하기 위한 레이

    int m_layerIndex;
    UINT m_meshDataIndex;
    bool m_bDead;
    bool m_bUpdateCheck;
    bool m_bRenderCheck;
    bool m_bOn;
    bool m_bDynamic;
    bool m_bFont;
    bool m_bOffFrame;

    priority_queue<CNaviTileNode*, vector<CNaviTileNode*>, CCompare> m_openList; // 캐릭터 별로 가지는 오픈 리스트

public:
    void start();
    void update();
    void lateUpdate();
    void finalUpdate();
    void finalUpdate_ex();
    void render();
    void render_script(int _index);

    void SetObjectType(OBJECT_TYPE _type) { m_objType = _type; }
    void SetMeshDataIndex(UINT _index) { m_meshDataIndex = _index; }
    void SetOnOff(bool _bOn); 
    void SetObjOff() { SetOnOff(false); }
    void SetObjOn() { SetOnOff(true); }
    void SetDynamic(bool _bDynamic) { m_bDynamic = _bDynamic; }
    void AddComponent(CComponent* _component);
    void SubComponent(COMPONENT_TYPE _type); // 스크립트를 제외한 컴포넌트 삭제
    OBJECT_TYPE GetObjectType() { return m_objType; }
    CComponent* GetComponent(COMPONENT_TYPE _type) { return m_arrComponent[(UINT)_type]; }
    CComponent* GetScriptComponent(UINT eType);

    void AddChildName(const wstring& _name) { m_vecChildName.push_back(_name); }
    const vector<wstring>& GetChildsName() { return m_vecChildName; }
    CGameObject* GetParent() { return m_parent; }
    int GetLayerIndex() { return m_layerIndex; }
    UINT GetMeshDataIndex() { return m_meshDataIndex; }
    const vector<CGameObject*>& GetChilds() const { return m_vecChild; }
    vector<CGameObject*>& GetChilds_Ref() { return m_vecChild; }
    void SetFont(bool _font) { m_bFont = _font; }
    bool IsFont() { return m_bFont; }

    void ChildLayerMove(CGameObject* _parent, int _layerIndex);

    bool IsAncestor(CGameObject* _obj);

    bool IsAncestorByName(const wstring& _name);
    bool IsDead() { return m_bDead; }
    bool IsDynamic() { return m_bDynamic; }
    bool IsOn() { return m_bOn; }
    priority_queue<CNaviTileNode*, vector<CNaviTileNode*>, CCompare>& GetOpenList() { return m_openList; }
    const vector<CScript*>& GetScripts() { return m_vecScript; }
    vector<CScript*>& GetScripts_Ref() { return m_vecScript; }

    const tRay& GetRay() { return m_ray; }

    void SetUpdateAction() { m_bUpdateCheck = true; }
    bool GetUpdateAction() { return m_bUpdateCheck; }

    void SetRenderCheck(bool _bRender) { m_bRenderCheck = _bRender; }
    bool GetRenderCheck() { return m_bRenderCheck; }

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESH_RENDER);
    GET_COMPONENT(Camera, CAMERA);
    GET_COMPONENT(Collider2D, COLLIDER2D);
    GET_COMPONENT(Collider3D, COLLIDER3D);
    GET_COMPONENT(Animator2D, ANIMATOR2D);
    GET_COMPONENT(Animator3D, ANIMATOR3D);
    GET_COMPONENT(Light2D, LIGHT2D);
    GET_COMPONENT(Light3D, LIGHT3D);
    GET_COMPONENT(Terrain, TERRAIN);
    GET_COMPONENT(TileMap, TILEMAP);
    GET_COMPONENT(NaviMesh, NAVIMESH);
    GET_COMPONENT(ParticleSystem, PARTICLE);

    void RegisterAsPrefab(const wstring& _name = L"");

private:
    void SetDead();
    void RegisterLayer();
    void AddChild(CGameObject* _childObj);
    void DisconnectWithParent();
    void CalRay();

    vector<CGameObject*>& GetChild_Ref() { return m_vecChild; }

public:
    void SaveToScene(FILE* _file);
    void LoadFromScene(FILE* _file, int _depth);

    CLONE(CGameObject);

public:
    CGameObject();
    CGameObject(const CGameObject& _origin);
    virtual ~CGameObject();

    friend class CEventManager;
    friend class CLayer;
};


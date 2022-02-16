#pragma once
#include "CComponent.h"
#include "CNaviTileNode.h"

class CNaviMesh :
    public CComponent
{
private :
    vector<CNaviTileNode*> m_vecNaviMeshNode;

    Vec3 m_pointPos;
    bool m_bRotRenew;
    bool m_bRenew;
    bool m_bMove;
    bool m_bRot;
    bool m_bWireFrame;

    float m_rotAngle;
    float m_rotTime;

public :
    virtual void start();
    virtual void update();
    virtual void lateUpdate();
    virtual void finalUpdate();

    void SetRotRenew(bool _bRotRenew) { m_bRotRenew = _bRotRenew; }
    void SetRenew(bool _bRenew) { m_bRenew = _bRenew; }
    void SetMove(bool _bMove) { m_bMove = _bMove; }
    void SetWireFrame(bool _bWireFrame) { m_bWireFrame = _bWireFrame; }

    Vec3 GetPointPos() { return m_pointPos; }
    Vec3 GetMouseClickPos(CCamera* _cam);

    bool IsRotRenew() { return m_bRotRenew; }
    bool IsRenew() { return m_bRenew; }
    bool IsMove() { return m_bMove; }
    bool IsWireFrame() { return m_bWireFrame; }
    bool IsAboveTile(CGameObject* _obj, Vec3 _dir, float _dist = 15.f); // 캐릭터가 타일 위에 있는지 확인하는 함수
    CNaviTileNode* GetCurNode(CGameObject* _obj);
    CGameObject* GetObjToAboveOtherTile(CNaviTileNode* _targetNode)
    {
        if (nullptr != _targetNode->m_aboveObject)
            return _targetNode->m_aboveObject;

        return nullptr;
    }

    void FindPath(CNaviTileNode* _curNode, CGameObject* _character, Vec3 _pointPos, Matrix _worldMatrix, list<Vec3>& _waypoints);
    CNaviTileNode* FindAroundTileInfo(int _ID);
    void Reset();

public :
    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CNaviMesh);

public :
    CNaviMesh();
    CNaviMesh(const CNaviMesh& _origin);
    ~CNaviMesh();
};


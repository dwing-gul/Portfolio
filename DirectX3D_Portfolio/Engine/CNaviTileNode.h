#pragma once

class CCompare;
class CGameObject;

extern int g_nextTileID;

class CNaviTileNode
{
private:
    tVertex m_vertex[3];
    int m_ID;
    vector<int> m_aroundTile;
    CGameObject* m_aboveObject;

    CNaviTileNode* m_prevNode;
    float		m_fromParent; // 이전 노드에서 현재 노드까지의 거리
    float		m_toDest;	 // 현재 노드에서 목적지 까지의 거리
    float		m_final;	// 위에 두 값을 합친 값(우선순위 기준)

    bool		m_bMove;   // 이동 가능 불가능
    bool		m_bOpen;   // OpenList 에 들어갔는지
    bool		m_bClosed; // ClostList 에 들어있는지

public:
    const tVertex* GetTileVertex() { return m_vertex; }
    const vector<int>& GetAroundTile() { return m_aroundTile; }
    CGameObject* GetAboveObject() { return m_aboveObject; }
    const CNaviTileNode* GetPrevNode() { return m_prevNode; }

    void SetTileVertex(tVertex(&vertex)[3])
    {
        for (int i = 0; i < 3; ++i)
        {
            m_vertex[i] = vertex[i];
        }
    }
    void SetAroundTile(int _tileID) { m_aroundTile.push_back(_tileID); }
    void SetMove(bool _move) { m_bMove = _move; }

    bool FindAroundTile(CNaviTileNode* _target);
    bool FindAroundTile_Other(CNaviTileNode* _target, Matrix _world, Matrix _otherWorld);

    void Rebuild(priority_queue<CNaviTileNode*, vector<CNaviTileNode*>, CCompare>& _queue);
    void CalculateCost(CNaviTileNode* _curNode, CNaviTileNode* _origin, Matrix _worldMatrix, Vec3 _pointPos);
    void AddOpenList(priority_queue<CNaviTileNode*, vector<CNaviTileNode*>, CCompare>& _queue, CNaviTileNode* _aroundTile,
        CNaviTileNode* _origin, Matrix _worldMatrix, Vec3 _pointPos);

    bool IsMove() { return m_bMove; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CNaviTileNode)

public:
    CNaviTileNode();
    CNaviTileNode(const CNaviTileNode& _origin);
    ~CNaviTileNode();

    friend class CCompare;
    friend class CNaviMesh;
};

// Open List
class CCompare
{
public:
    bool operator() (CNaviTileNode* _pLeft, CNaviTileNode* _pRight)
    {
        return _pLeft->m_final > _pRight->m_final;
    }
};

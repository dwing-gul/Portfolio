#pragma once
#include <Engine/CScript.h>

enum class POINT_TYPE
{
    MOVE,
    LOOTING,
    HUNTING
};

struct tPointType
{
    Vec3        m_pointPos;
    POINT_TYPE  m_pointType;
    int         m_pointID;
};

class CGameObject;

class CAIPointScript :
    public CScript
{
private :
    vector<tPointType>  m_vecPoint;
    CGameObject*        m_owner;
    int                 m_curPointID;
    int                 m_prevPointID;
    bool                m_bArrive;
    bool                m_bNext;

public:
    void SetOwner(CGameObject* _owner) { m_owner = _owner; }
    void SetPointType(POINT_TYPE _type, int _pointID) { m_vecPoint[_pointID].m_pointType = _type; }
    void SetArrive(bool _arrive) { m_bArrive = _arrive; }
    void SetNext(bool _b) { m_bNext = _b; }
    void AddPointID();

    void SetPointPos(Vec3 _pos, int _pointID) { m_vecPoint[_pointID].m_pointPos = _pos; }
    void SetPointID(int _id, int _pointID) { m_vecPoint[_pointID].m_pointID = _id; }
    
    void SetMovePos();
    void SetPointAction();
    POINT_TYPE GetPointType(int _pointID) { return m_vecPoint[_pointID].m_pointType; }
    bool IsArrive() { return m_bArrive; }
    bool IsNext() { return m_bNext; }
    int GetFindPointID();
    Vec3 GetPointPos(int _pointID) { return m_vecPoint[_pointID].m_pointPos; }
    int GetCurPointID() { return m_curPointID; }
    int GetPrevPointID() { return m_prevPointID; }
    vector<tPointType>& GetVecPoint() { return m_vecPoint; }
    tPointType& GetPointTypeStruct(int _id) { return m_vecPoint[_id]; }

    void Reset() { m_bArrive = false; m_bNext = false; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CAIPointScript)

public:
    CAIPointScript();
    ~CAIPointScript();
};


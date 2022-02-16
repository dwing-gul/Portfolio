#pragma once
#include <Engine/Script.h>

class CameraScript :
    public Script
{
private :
    GameObject*     m_targetDoor;
    bool            m_bBattlePhase;
    bool            m_bEnter; // 문으로 들어가는 연출을 실행해야하는가?
    bool            m_bMove; // 카메라가 이동해야 하는가?
    float           m_leftEnd; // 왼쪽 끝
    float           m_rightEnd; // 오른쪽 끝
    int             m_floor;
    int             m_moveCount; // 이동이 한번만 이루어지게 하기 위한 카운트
    bool            m_bFirst;
    bool            m_bSkip;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetTargetDoor(GameObject* _door) { m_targetDoor = _door; }
    void SetBattlePhase(bool _battle) { m_bBattlePhase = _battle; }
    void SetEnter(bool _enter) { m_bEnter = _enter; }

    bool IsEnter() { return m_bEnter; }
    int GetFloor() { return m_floor; }

    CLONE(CameraScript)

public:
    CameraScript();
    ~CameraScript();
};


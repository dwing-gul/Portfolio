#pragma once
#include <Engine/Script.h>

class CameraScript :
    public Script
{
private :
    GameObject*     m_targetDoor;
    bool            m_bBattlePhase;
    bool            m_bEnter; // ������ ���� ������ �����ؾ��ϴ°�?
    bool            m_bMove; // ī�޶� �̵��ؾ� �ϴ°�?
    float           m_leftEnd; // ���� ��
    float           m_rightEnd; // ������ ��
    int             m_floor;
    int             m_moveCount; // �̵��� �ѹ��� �̷������ �ϱ� ���� ī��Ʈ
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


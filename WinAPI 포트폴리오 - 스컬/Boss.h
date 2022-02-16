#pragma once
#include "Monster.h"
class Boss :
    public Monster
{
private :
    vector<Boss*> m_vectorBossBodyPart; //���� ������ ������ ���� ��Ʈ
    Boss* m_leftHand;
    Boss* m_rightHand; // ������ ���
    Boss* m_parentPart; // �ֻ��� �θ� ���� ��Ʈ
    Vector2 m_finalPos; // ���� ��ġ;
    Vector2 m_prevPos;

    bool m_bLeft; // �޼��� ������ �� �������� ������ �� ����
    bool m_bGround; // ������ ����� �� �յ��� ���� �΋H���°�?
    bool m_bGroggy; // �׷α� �����ΰ�

    int m_handSelect; // 0�̸� ���� �ƴ�, 1�̸� �޼�, 2�̸� ������

public :
    void AddPart(Boss* _part) { m_vectorBossBodyPart.push_back(_part); _part->m_parentPart = this; }
    void SetLeftHands(Boss* _leftHand) { m_leftHand = _leftHand; }
    void SetRightHands(Boss* _rightHand) { m_rightHand = _rightHand; }
    void SetParentPart(Boss* _parentPart) { m_parentPart = _parentPart; }
    void SetLeft(bool _hand) { m_bLeft = _hand; }
    void SetGround(bool _ground) { m_bGround = _ground; }
    void SetPrevPos(Vector2 _prevPos) { m_prevPos = _prevPos; }
    void SetGroggy(bool _groggy) { m_bGroggy = _groggy; }

    void SweepHandSelect();
    void StampHandSelect();

    Boss* GetParentPart() { return m_parentPart; }
    int GetSelectedHand() { return m_handSelect; }
    Vector2 GetFinalPos() { return m_finalPos; }
    vector<Boss*> GetChildParts() { return m_vectorBossBodyPart; }
    Boss* GetLeftHand() { return m_leftHand; }
    Boss* GetRightHand() { return m_rightHand; }
    bool IsGround() { return m_bGround; }
    bool IsLeft() { return m_bLeft; }

    virtual void update();
    virtual void lateUpdate();
    virtual void render(HDC _dc);

    CLONE_DISABLE(Boss)
    
public :
    Boss();
    ~Boss();
};


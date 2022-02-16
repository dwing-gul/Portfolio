#pragma once
#include "Monster.h"
class Boss :
    public Monster
{
private :
    vector<Boss*> m_vectorBossBodyPart; //손을 제외한 보스의 몸쪽 파트
    Boss* m_leftHand;
    Boss* m_rightHand; // 보스의 양손
    Boss* m_parentPart; // 최상위 부모 보스 파트
    Vector2 m_finalPos; // 최종 위치;
    Vector2 m_prevPos;

    bool m_bLeft; // 왼손을 움직일 지 오른손을 움직일 지 결정
    bool m_bGround; // 스탬프 모션일 때 손들이 땅에 부딫혔는가?
    bool m_bGroggy; // 그로기 상태인가

    int m_handSelect; // 0이면 손이 아님, 1이면 왼손, 2이면 오른손

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


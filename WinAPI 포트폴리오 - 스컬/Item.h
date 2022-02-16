#pragma once
#include "Obj.h"

class Player;

class Item :
    public Obj
{
private :
    Player* m_owner;
    SUBJECT_TYPE m_itemType; // 정수인지 단순 아이템인지 확인하기 위한 변수
    ITEM_TYPE m_itemStyle; // 아이템인 경우 어떤 아이템인지 확인하기 위한 변수
    GRADE m_itemGrade; // 아이템의 등급을 알리는 변수
    ENGRAVE_TYPE m_arrayEngrave[2];

    bool m_bAdapt; // 아이템 효과를 1번만 적용시키기 위한 변수
    bool m_bAdaptSkil; // 각인 효과 중에서 1번만 적용 및 정수의 액티브 스킬을 발동할 수 있는지 확인하기 위한 변수
    bool m_bSoar; // 비상 각인 효과에서 사용할 변수

    float m_duration; // 정수 아이템의 지속시간
    float m_coolTime; // 정수 아이템 쿨타임

public:
    void SetOwner(Player* _player) { m_owner = _player; }
    void SetItemType(SUBJECT_TYPE _type) { m_itemType = _type; }
    void SetItemStyle(ITEM_TYPE _type) { m_itemStyle = _type; }
    void SetItemGrade(GRADE _grade) { m_itemGrade = _grade; }

    GRADE GetItemGrade() { return m_itemGrade; }
    ITEM_TYPE GetItemStyle() { return m_itemStyle; }

    void AdaptItem();
    void AdaptEngrave();
    void UseEssence();
    void PassiveEssense();

    void DestroyItem(ITEM_TYPE _type);
    void ReleaseEngrave();

    virtual void update();
    virtual void lateUpdate();

    CLONE(Item)

public:
    Item();
    virtual ~Item();
};


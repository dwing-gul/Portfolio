#pragma once
#include "Obj.h"

class Player;

class Item :
    public Obj
{
private :
    Player* m_owner;
    SUBJECT_TYPE m_itemType; // �������� �ܼ� ���������� Ȯ���ϱ� ���� ����
    ITEM_TYPE m_itemStyle; // �������� ��� � ���������� Ȯ���ϱ� ���� ����
    GRADE m_itemGrade; // �������� ����� �˸��� ����
    ENGRAVE_TYPE m_arrayEngrave[2];

    bool m_bAdapt; // ������ ȿ���� 1���� �����Ű�� ���� ����
    bool m_bAdaptSkil; // ���� ȿ�� �߿��� 1���� ���� �� ������ ��Ƽ�� ��ų�� �ߵ��� �� �ִ��� Ȯ���ϱ� ���� ����
    bool m_bSoar; // ��� ���� ȿ������ ����� ����

    float m_duration; // ���� �������� ���ӽð�
    float m_coolTime; // ���� ������ ��Ÿ��

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


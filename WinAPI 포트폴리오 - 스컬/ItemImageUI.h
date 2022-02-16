#pragma once
#include "UI.h"
class ItemImageUI :
    public UI
{
private :
    SUBJECT_TYPE m_itemType; // �������� �ܼ� ���������� Ȯ���ϱ� ���� ����
    ITEM_TYPE m_itemStyle; // �������� ��� � ���������� Ȯ���ϱ� ���� ����
    GRADE m_itemGrade; // �������� ����� �˸��� ����
    SKUL m_skulType; // ���� �������� �� � �������� Ȯ���ϱ� ���� ����

    bool m_bCreate;
    bool m_bDestroy;
    float m_timer;

public :
    void SetItemType(SUBJECT_TYPE _type) { m_itemType = _type; }
    void SetItemStyle(ITEM_TYPE _type) { m_itemStyle = _type; }
    void SetItemGrade(GRADE _grade) { m_itemGrade = _grade; }
    void SetSkulType(SKUL _type) { m_skulType = _type; }

    GRADE GetItemGrade() { return m_itemGrade; }

    void ItemUISetting(UI* _parentUI);

    virtual void update();
    virtual void lateUpdate();
    virtual void render(HDC _dc);

    CLONE(ItemImageUI)

public :
    ItemImageUI();
    ~ItemImageUI();
};


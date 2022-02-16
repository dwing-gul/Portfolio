#pragma once
#include "UI.h"
class ItemImageUI :
    public UI
{
private :
    SUBJECT_TYPE m_itemType; // 정수인지 단순 아이템인지 확인하기 위한 변수
    ITEM_TYPE m_itemStyle; // 아이템인 경우 어떤 아이템인지 확인하기 위한 변수
    GRADE m_itemGrade; // 아이템의 등급을 알리는 변수
    SKUL m_skulType; // 스컬 아이템일 때 어떤 스컬인지 확인하기 위한 변수

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


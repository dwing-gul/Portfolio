#pragma once
#include "Obj.h"
class Subject :
    public Obj
{
private :
    SUBJECT_TYPE m_subjectType; // 맵에 있는 오브젝트의 타입
    ITEM_TYPE m_itemType; // 오브젝트의 타입이 아이템일 때 어떤 아이템을 띄우는지를 확인하는 변수
    GRADE m_itemGrade;
    SKUL m_skul;

    wstring m_relativePath; // UI에 넣어줄 아이템의 이미지 위치

public:
    void SetSubjectType(SUBJECT_TYPE _type) { m_subjectType = _type; }
    void SetItemType(ITEM_TYPE _type) { m_itemType = _type; }
    void SetItemGrade(GRADE _grade) { m_itemGrade = _grade; }
    void SetRelativePath(const wstring& _path) { m_relativePath = _path; }
    void SetSkul(SKUL _skul) { m_skul = _skul; }

    SUBJECT_TYPE GetSubjectType() { return m_subjectType; }

    virtual void update();
    virtual void render(HDC _dc);

    virtual void OnColliderEnter(Obj* _other);
    virtual void OnCollider(Obj* _other);

    CLONE(Subject)

public:
    Subject();
    virtual ~Subject();
};


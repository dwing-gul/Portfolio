#pragma once
#include "Obj.h"
class Subject :
    public Obj
{
private :
    SUBJECT_TYPE m_subjectType; // �ʿ� �ִ� ������Ʈ�� Ÿ��
    ITEM_TYPE m_itemType; // ������Ʈ�� Ÿ���� �������� �� � �������� �������� Ȯ���ϴ� ����
    GRADE m_itemGrade;
    SKUL m_skul;

    wstring m_relativePath; // UI�� �־��� �������� �̹��� ��ġ

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


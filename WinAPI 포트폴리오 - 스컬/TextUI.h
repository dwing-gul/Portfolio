#pragma once
#include "UI.h"
class TextUI :
    public UI
{
private :
    wstring m_text;
    wstring m_target; // 문장 변동을 위한 타겟

    bool m_bCurHP; // 현재 hp 인지 확인하는 변수
    bool m_bMaxHP;
    bool m_bBlack;

    SUBJECT_TYPE m_subjectType;
    ITEM_TYPE m_itemType;
    SKUL m_skulType;

public :
    void SetText(const wstring& _text) { m_text = _text; }
    void SetItemType(ITEM_TYPE _type) { m_itemType = _type; }
    void SetSubjectType(SUBJECT_TYPE _type) { m_subjectType = _type; }
    void SetSkulType(SKUL _type) { m_skulType = _type; }
    void IsCurHP(bool _hp) { m_bCurHP = _hp; }
    void IsMaxHP(bool _hp) { m_bMaxHP = _hp; }
    void IsBlack(bool _textColor) { m_bBlack = _textColor; }

    SUBJECT_TYPE GetSubjectType() { return m_subjectType; }
    ITEM_TYPE GetItemType() { return m_itemType; }
    SKUL GetSkulType() { return m_skulType; }

    virtual void update();
    virtual void render(HDC _dc);
    CLONE(TextUI);

public:
    TextUI();
    virtual ~TextUI();
};


#pragma once
#include "CUIScript.h"
class CButtonUIScript :
    public CUIScript
{
private :
    SKILL_TYPE m_skillType;
    bool m_bClicked;
    bool m_bDisable;
    bool m_bDay;
    bool m_bDayChange;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetDay(bool _day) { m_bDay = _day; m_bDayChange = true; } // true´Â ³·, false´Â ¹ã
    void SetSkillType(SKILL_TYPE _type) { m_skillType = _type; }

    bool IsDay() { return m_bDay; } // ³·ÀÎÁö ¹ãÀÎÁö È®ÀÎ

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CButtonUIScript)

public:
    CButtonUIScript();
    ~CButtonUIScript();
};


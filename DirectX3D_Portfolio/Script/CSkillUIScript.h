#pragma once
#include "CUIScript.h"

class CSkillUIScript :
    public CUIScript
{
private :
    CHARACTER_TYPE m_ownerType;
    SKILL_TYPE m_skillType;
    int m_skillMaxLevel;
    int m_skillCurLevel;
    bool m_bUse; // 스킬을 사용했는지

    bool m_bLevelUp; // 스킬 레벨업이 적용된 상태인가?
    bool m_bStep; // 스킬 레벨 단계를 나타내기 위한 것인가?

    // 쿨타임 나타내기 위한 용도 -> 스킬 이미지에서만 사용
    float m_ratio;
    BAR_MODE m_barMode;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetSkillType(SKILL_TYPE _type) { m_skillType = _type; }
    void SetOwnerType(CHARACTER_TYPE _type) { m_ownerType = _type; }
    void SetLevelStep(bool _step) { m_bStep = _step; }

    virtual void PlayerChange();

    void ChangeSkillImage();

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CSkillUIScript)

public:
    CSkillUIScript();
    ~CSkillUIScript();
};


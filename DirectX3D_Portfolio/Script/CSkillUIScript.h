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
    bool m_bUse; // ��ų�� ����ߴ���

    bool m_bLevelUp; // ��ų �������� ����� �����ΰ�?
    bool m_bStep; // ��ų ���� �ܰ踦 ��Ÿ���� ���� ���ΰ�?

    // ��Ÿ�� ��Ÿ���� ���� �뵵 -> ��ų �̹��������� ���
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


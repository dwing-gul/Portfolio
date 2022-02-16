#pragma once
#include <Engine/Script.h>
#include "ScriptManager.h"

class GameObject;
class MonsterScript;

class UIScript :
    public Script
{
private :
    MonsterScript*      m_targetScript;
    GameObject*         m_focusedUI;
    GameObject*         m_prevTargetUI;
    GameObject*         m_curTargetUI;
    float               m_originXPos;
    float               m_originYPos;
    bool                m_bChange;
    bool                m_bMonsterInfo;
    bool                m_bFirst;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetTargetScript(MonsterScript* _target) { m_targetScript = _target; }
    void SetMonsterInfo(bool _info) { m_bMonsterInfo = _info; }

    GameObject* GetCurTargetUI() { return m_curTargetUI; }
    void SetCurTargetUI(GameObject* _ui) { m_curTargetUI = _ui; }
    GameObject* GetCurFocusedUI() { return m_focusedUI; }

private :
    GameObject* GetFocusedUI();
    virtual GameObject* GetTargetedUI(GameObject* _parentUI);
    virtual void UIEvent(GameObject* _parentUI);

    CLONE(UIScript)

protected :
    void SetOriginYPos(float _yPos) { m_originYPos = _yPos; }
    void SetOriginXPos(float _xPos) { m_originXPos = _xPos; }

public:
    UIScript();
    UIScript(SCRIPT_TYPE _type);
    ~UIScript();
};


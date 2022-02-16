#pragma once
#include <Engine/Script.h>
#include "PlayerScript.h"

class GameObject;

class SkillButtonScript :
    public Script
{
private :
    PlayerScript*   m_ownerScript;
    int             m_position;
    SKILL_TYPE      m_type;
    wstring         m_ownerName;
    bool            m_bPass; // 패스 버튼인가?
    bool            m_bMove; // 이동 버튼인가?
    bool            m_bPortrait; // 플레이어 초상화
    bool            m_bSelect;

public :
    virtual void start();
    virtual void update();
    virtual void lateUpdate();

    void SetPosition(int _pos) { m_position = _pos; }
    void SetOwnerName(const wstring& _name) { m_ownerName = _name; }
    void SetOwnerByName(const wstring& _playerName);
    void SetType(SKILL_TYPE _type) { m_type = _type; }
    void SetPass(bool _bPass) { m_bPass = _bPass; }
    void SetMove(bool _bMove) { m_bMove = _bMove; }
    void SetPortrait(bool _bPortrait) { m_bPortrait = _bPortrait; }
    void SetSelect(bool _bSelect) { m_bSelect = _bSelect; }

    bool IsSelect() { return m_bSelect; }

    int GetPosition() { return m_position; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

private:
    void SetSkillButtonTexture();
    void SetMoveTexture();
    void SetPassTexture();
    void SetPortrait();

    CLONE(SkillButtonScript)

public :
    SkillButtonScript();
    ~SkillButtonScript();
};


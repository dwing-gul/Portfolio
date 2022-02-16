#pragma once
#include "UIScript.h"

enum class TEXT_TARGET_TYPE
{
    GENERAL,
    PLAYER,
    MONSTER,
    ITEM,
    END
};

enum class TEXT_USAGE
{
    NAME,
    JOB,
    HP,
    STRESS,
    ACCURACY,
    CRITICAL,
    DAMAGE,
    DODGE,
    PROTECTION,
    SPEED,
    RESISTANCE_STUN,
    RESISTANCE_BLIGHT,
    RESISTANCE_BLEED,
    RESISTANCE_DEBUFF,
    RESISTANCE_MOVE,
    ITEM_COUNT,
    GENERAL,
    END
};

class GameObject;

class TextUIScript :
    public UIScript
{
private :
    GameObject*         m_target; // 텍스트 출력 대상
    TEXT_TARGET_TYPE    m_targetType;
    TEXT_USAGE          m_usage;
    wstring             m_text;
    Vector2             m_textPos;
    float               m_fontSize;
    int                 m_redColor;
    int                 m_greenColor;
    int                 m_blueColor;
    int                 m_alphaColor;
    int                 m_damage; // 데미지 표시를 위한 부분
    UINT                m_fontColor;
    bool                m_bShow; // 텍스트가 보여야 하는 순간인가?

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();
    virtual void render();

    void SetTarget(GameObject* _target) { m_target = _target; }
    void SetTargetType(TEXT_TARGET_TYPE _type) { m_targetType = _type; }
    void SetUsage(TEXT_USAGE _usage) { m_usage = _usage; }
    void SetText(const wstring& _text) { m_text = _text; }
    void SetTextPos(Vector2 _pos) { m_textPos = _pos; }
    void SetFontSize(float _size) { m_fontSize = _size; }
    void SetFontColor_Red(int _color) { m_redColor = _color; }
    void SetFontColor_Green(int _color) { m_greenColor = _color; }
    void SetFontColor_Blue(int _color) { m_blueColor = _color; }
    void SetFontColor_Alpha(int _color) { m_alphaColor = _color; }
    void SetFontColor(UINT _color) { m_fontColor = _color; }
    void SetDamageNumber(int _damage) { m_damage = _damage; }
    void SetShow(bool _show) { m_bShow = _show; }

    GameObject* GetTarget() { return m_target; }
    TEXT_TARGET_TYPE GetTargetType() { return m_targetType; }
    TEXT_USAGE GetUsage() { return m_usage; }
    const wstring& GetText() { return m_text; }
    Vector2 GetTextPos() { return m_textPos; }
    float GetFontSize() { return m_fontSize; }
    int GetFontColor_Red() { return m_redColor; }
    int GetFontColor_Green() { return m_greenColor; }
    int GetFontColor_Blue() { return m_blueColor; }
    int GetFontColor_Alpha() { return m_alphaColor; }
    int GetDamageNumber() { return m_damage; }
    bool IsShow() { return m_bShow; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(TextUIScript)

public:
    TextUIScript();
    ~TextUIScript();
};


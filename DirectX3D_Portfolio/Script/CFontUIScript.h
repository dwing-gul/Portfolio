#pragma once
#include "CUIScript.h"

enum class FONT_TYPE
{
    GENERAL,
    DAY,
    ATTACK_POWER,
    DEFENSE,
    EXTRA_ATTACK,
    DAMAGE_REDUCTION,
    SKILL_DMG_INC,
    SKILL_DMG_REDUCTION,
    MOVE_SPEED,
    ATTACK_SPEED,
    HP,
    SP,
    USER_NUM,
    TIME,
    LEVEL,
    TRACKING_LEVEL,
    END
};

class CFontUIScript :
    public CUIScript
{
private :
    wstring             m_text;
    FONT_TYPE           m_fontType;
    Vector2             m_textPos;
    float               m_fontSize;
    int                 m_redColor;
    int                 m_greenColor;
    int                 m_blueColor;
    int                 m_alphaColor;
    int                 m_numberText; // 데미지 표시를 위한 부분
    UINT                m_fontColor;
    bool                m_bShow; // 텍스트가 보여야 하는 순간인가?
    bool                m_bNum; // 숫자 텍스트인가?
    bool                m_bTime; // 시간에 관한 텍스트인가?
    CGameObject*        m_cam;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();
    virtual void render();

    void SetText(const wstring& _text) { m_text = _text; }
    void SetFontType(FONT_TYPE _type) { m_fontType = _type; }
    void SetTextPos(Vector2 _pos) { m_textPos = _pos; }
    void SetFontSize(float _size) { m_fontSize = _size; }
    void SetFontColor_Red(int _color) { m_redColor = _color; }
    void SetFontColor_Green(int _color) { m_greenColor = _color; }
    void SetFontColor_Blue(int _color) { m_blueColor = _color; }
    void SetFontColor_Alpha(int _color) { m_alphaColor = _color; }
    void SetFontColor(UINT _color) { m_fontColor = _color; }
    void SetNumberText(int _num) { m_numberText = _num; m_bNum = true; }
    void SetShow(bool _show) { m_bShow = _show; }
    void SetTime(int _time) { m_numberText = _time; m_bNum = true; m_bTime = true; }

    const wstring& GetText() { return m_text; }
    Vector2 GetTextPos() { return m_textPos; }
    FONT_TYPE GetFontType() { return m_fontType; }
    float GetFontSize() { return m_fontSize; }
    int GetFontColor_Red() { return m_redColor; }
    int GetFontColor_Green() { return m_greenColor; }
    int GetFontColor_Blue() { return m_blueColor; }
    int GetFontColor_Alpha() { return m_alphaColor; }
    bool IsShow() { return m_bShow; }

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CFontUIScript)

public:
    CFontUIScript();
    CFontUIScript(const CFontUIScript& _origin);
    ~CFontUIScript();
};


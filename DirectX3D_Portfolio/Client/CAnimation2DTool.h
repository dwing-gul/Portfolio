#pragma once
#include "CGUI.h"
#include <Engine\struct.h>

class CTexture;

class CAnimation2DTool :
    public CGUI
{
public:
    virtual void init();
    virtual void update();

    void TexLoad();
    void AnimSave(wstring wstrAnimName);
    void AnimLoad(int& iSize);
private:
    void Listinit();
    void AnimPlay(int& iFrmID);
    void ChangeToolVersion();

private:
    CTexture*               m_pTex;
    vector<string>          m_vecTexList;
    vector<tAnimFrame>      m_vecAnimInfo;
    wstring                 m_strTexName;

    float                   m_fAccTime;
    bool                    m_bPlay;
    bool                    m_bRepeat;
public:
    CAnimation2DTool();
    ~CAnimation2DTool();
};
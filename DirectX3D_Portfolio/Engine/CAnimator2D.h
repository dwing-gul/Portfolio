#pragma once
#include "CComponent.h"
#include "CTexture.h"

class CAnimation2D;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnimation2D*> m_mapAnim;
    CAnimation2D* m_curAnim;

public:
    CAnimation2D* FindAnim(const wstring& _name);
    void CreateAnim(const wstring& _name, Ptr<CTexture> _atlas, Vec2 _leftTop, Vec2 _frameSize, Vec2 _baseSize, float _duration, int _frameCount);
    void CreateAnimation_Tool(const wstring& _strName, Ptr<CTexture> _pAtlas, vector<tAnimFrame>& _vecAnimFrm);

    void LoadAnim(const wstring& _relativePath);

    map<wstring, CAnimation2D*>& GetAnimations() { return m_mapAnim; }
    CAnimation2D* GetCurAnim() { return m_curAnim; }

    void Play(const wstring& _name, bool _bRepeat = true);
    virtual void UpdateData();
    static void Clear();

    virtual void lateUpdate();

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CAnimator2D);

public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _origin);
    ~CAnimator2D();
};



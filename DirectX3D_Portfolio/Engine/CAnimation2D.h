#pragma once
#include "CEntity.h"
#include "CTexture.h"

class CAnimator2D;

class CAnimation2D :
    public CEntity
{
private:
    CAnimator2D* m_animator;
    Ptr<CTexture> m_texture;
    vector<tAnimFrame> m_vecAnimFrame;
    Vec2 m_baseSize;
    int m_curFrameIndex;
    float m_accTime;
    bool m_bFinish;
    bool m_bRepeat;

public:
    void lateUpdate();

    void Create(Ptr<CTexture> _texture, Vec2 _leftTop, Vec2 _frameSize, Vec2 _baseSize, float _duration, int _frameCount);
    void Create_Tool(Ptr<CTexture> _pTex, vector<tAnimFrame>& _vecAnimFrm);

    void SetRepeat(bool _bRepeat) { m_bRepeat = _bRepeat; }
    void SetTexture(Ptr<CTexture> _texture) { m_texture = _texture; }
    void SetBaseSize(Vec2 _baseSize) { m_baseSize = _baseSize; }

    Ptr<CTexture> GetTexture() { return m_texture; }
    Vec2 GetBaseSize() { return m_baseSize; }
    vector<tAnimFrame>& GetVectorAnimFrame() { return m_vecAnimFrame; }

    bool IsFinish() { return m_bFinish; }
    bool IsRepeat() { return m_bRepeat; }
    void Reset()
    {
        m_accTime = 0.f;
        m_curFrameIndex = 0;
        m_bFinish = false;
    }
    void UpdateData();

    void SetAnimFrmInfo(tAnimFrame _tAnimFrm, int iIndex) { m_vecAnimFrame[iIndex] = _tAnimFrm; }
    void SetFrmIdx(int iFrmid) { m_curFrameIndex = iFrmid; }
    void SetFinish(bool bFinsh) { m_bFinish = bFinsh; }

    int     GetFrmIdx() { return m_curFrameIndex; }
    float   GetAccTime() { return m_accTime; }

    void Save(const wstring& _relativePath, const wstring& _name);
    void Load(const wstring& _relativeFileName);

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(CAnimation2D);

public:
    CAnimation2D();
    CAnimation2D(const CAnimation2D& _origin);
    ~CAnimation2D();

    friend class CAnimator2D;
};


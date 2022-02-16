#pragma once
#include "Entity.h"
#include "Texture.h"

struct tAnimFrame
{
    Vector2 leftTop;
    Vector2 frameSize;
    Vector2 offset;
    float   duration;
};

class Animator2D;

class Animation2D :
    public Entity
{
private :
    Animator2D*         m_animator;
    Ptr<Texture>        m_texture;
    vector<tAnimFrame>  m_vectorAnimFrame;
    Vector2             m_baseSize;
    int                 m_curFrameIndex;
    float               m_accTime;
    bool                m_bFinish;
    bool                m_bRepeat;

public :
    void lateUpdate();

    void Create(Ptr<Texture> _texture, Vector2 _leftTop, Vector2 _frameSize, Vector2 _baseSize, float _duration, int _frameCount);
    void SetRepeat(bool _bRepeat) { m_bRepeat = _bRepeat; }
    void SetTexture(Ptr<Texture> _texture) { m_texture = _texture; }
    void SetBaseSize(Vector2 _baseSize) { m_baseSize = _baseSize; }

    Ptr<Texture> GetTexture() { return m_texture; }
    Vector2 GetBaseSize() { return m_baseSize; }
    vector<tAnimFrame>& GetVectorAnimFrame() { return m_vectorAnimFrame; }
    bool IsRepeat() { return m_bRepeat; }
    bool IsFinish() { return m_bFinish; }
    void Reset()
    {
        m_accTime = 0.f;
        m_curFrameIndex = 0;
        m_bFinish = false;
    }
    virtual void UpdateData();

    void Save(const wstring& _relativePath, const wstring& _name);
    void Load(const wstring& _relativePath);

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(Animation2D);

public:
    Animation2D();
    Animation2D(const Animation2D& _origin);
    ~Animation2D();

    friend class Animator2D;
};
#pragma once
#include "Component.h"

class Animation2D;

class Animator2D :
    public Component
{
private :
    map<wstring, Animation2D*> m_mapAnim;
    Animation2D*               m_curAnim;

public :
    Animation2D* FindAnim(const wstring& _name);
    void CreateAnim(const wstring& _name, Ptr<Texture> _atlasTexture, Vector2 _leftTop, Vector2 _frameSize, Vector2 _baseSize, float _duration, int _frameCount);
    void LoadAnim(const wstring& _relativePath);

    map<wstring, Animation2D*>& GetAnimations() { return m_mapAnim; }
    Animation2D* GetCurAnim() { return m_curAnim; }

    void Play(const wstring& _name, bool _bRepeat = true);
    virtual void UpdateData();
    static void Clear();

    virtual void lateUpdate();

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

    CLONE(Animator2D);

public:
    Animator2D();
    Animator2D(const Animator2D& _origin);
    ~Animator2D();
};
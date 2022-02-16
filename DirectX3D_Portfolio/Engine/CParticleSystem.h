#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CTexture.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;

class CParticleSystem :
    public CComponent
{
private:
    Ptr<CMesh>                  m_mesh;
    Ptr<CMaterial>              m_material;
    vector<Ptr<CTexture>>       m_vectorParticleTexture;
    UINT                        m_textureIndex;
    CStructuredBuffer*          m_particleBuffer;
    Ptr<CParticleUpdateShader>  m_updateShader;

    Vec4                        m_startColor;
    Vec4                        m_endColor;
    Vec3                        m_startScale;
    Vec3                        m_endScale;
    float                       m_startSpeed;
    float                       m_endSpeed;
    float                       m_minLifeTime;
    float                       m_maxLifeTime;

    UINT                        m_spawnRate;
    UINT                        m_maxParticleCount;

    Vec3                        m_area;
    float                       m_accTime;

public:
    virtual void start();
    virtual void update();
    virtual void lateUpdate();
    void render();

private:
    UINT CalSpawnCount();

    virtual void SaveToScene(FILE* _file);
    virtual void LoadFromScene(FILE* _file);

public:
    void SetTextureIndex(UINT _index) { m_textureIndex = _index; }
    void SetStartColor(Vec4 _color) { m_startColor = _color; }
    void SetEndColor(Vec4 _color) { m_endColor = _color; }
    void SetStartScale(Vec3 _scale) { m_startScale = _scale; }
    void SetEndScale(Vec3 _scale) { m_endScale = _scale; }
    void SetStartSpeed(float _speed) { m_startSpeed = _speed; }
    void SetEndSpeed(float _speed) { m_endSpeed = _speed; }
    void SetMinLifeTime(float _life) { m_minLifeTime = _life; }
    void SetMaxLifeTime(float _life) { m_maxLifeTime = _life; }
    void SetSpawnRate(UINT _rate) { m_spawnRate = _rate; }
    void SetMaxParticleCount(UINT _count) { m_maxParticleCount = _count; }
    void SetArea(Vec3 _area) { m_area = _area; }

    void SetUpdateShader(Ptr<CParticleUpdateShader> pUpdateShader) { m_updateShader = pUpdateShader; }
    void SetUpdateShader(wstring _wstrKey);
    void SetAnimation(const wstring& _strRelativeFilePath, bool bRepeat);

    UINT GetTextureIndex() { return m_textureIndex; }
    Vec4 GetStartColor() { return m_startColor; }
    Vec4 GetEndColor() { return m_endColor; }
    Vec3 GetStartScale() { return m_startScale; }
    Vec3 GetEndScale() { return m_endScale; }
    float GetStartSpeed() { return m_startSpeed; }
    float GetEndSpeed() { return m_endSpeed; }
    float GetMinLifeTime() { return m_minLifeTime; }
    float GetMaxLifeTime() { return m_maxLifeTime; }
    UINT GetSpawnRate() { return m_spawnRate; }
    UINT GetMaxParticleCount() { return m_maxParticleCount; }
    Vec3 GetArea() { return m_area; }
    vector<Ptr<CTexture>>& GetVectorTexture() { return m_vectorParticleTexture; }

    CLONE(CParticleSystem);

public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _origin);
    ~CParticleSystem();
};


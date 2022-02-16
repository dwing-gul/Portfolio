#pragma once
#include "CComputeShader.h"

struct tSharedData
{
    UINT spawnCount;
    UINT curCount;
    UINT padding[2];
};

class CParticleUpdateShader :
    public CComputeShader
{
private:
    CStructuredBuffer* m_particleBuffer;
    CStructuredBuffer* m_sharedBuffer;
    UINT m_spawnCount;

    Vec4 m_objPos;
    Vec3 m_area;
    float m_startSpeed;
    float m_endSpeed;
    float m_minLifeTime;
    float m_maxLifeTime;

public:
    void SetParticleBuffer(CStructuredBuffer* _buffer) { m_particleBuffer = _buffer; }
    void SetWorldPos(Vec3 _pos) { m_objPos = _pos; }
    void SetArea(Vec3 _area) { m_area = _area; }
    void SetSpawnCount(UINT _count) { m_spawnCount = _count; }
    void SetStartSpeed(float _speed) { m_startSpeed = _speed; }
    void SetEndSpeed(float _speed) { m_endSpeed = _speed; }
    void SetMinLife(float _life) { m_minLifeTime = _life; }
    void SetMaxLife(float _life) { m_maxLifeTime = _life; }

    virtual void UpdateData();
    virtual void Clear();
    virtual void Excute();

public:
    CParticleUpdateShader();
    ~CParticleUpdateShader();
};
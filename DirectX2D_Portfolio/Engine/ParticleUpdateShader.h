#pragma once
#include "ComputeShader.h"

struct tSharedData
{
    UINT spawnCount;
    UINT curCount;
    UINT padding[2];
};

class StructuredBuffer;

class ParticleUpdateShader :
    public ComputeShader
{
private :
    StructuredBuffer*    m_particleBuffer;
    StructuredBuffer*    m_sharedBuffer;
    UINT                 m_spawnCount;
    Vector3              m_objPos;
    Vector3              m_area;
    float                m_startSpeed;
    float                m_endSpeed;
    float                m_minLifeTime;
    float                m_maxLifeTime;

public :
    void SetParticleBuffer(StructuredBuffer* _buffer) { m_particleBuffer = _buffer; }
    void SetSpawnCount(UINT _count) { m_spawnCount = _count; }
    void SetWorldPos(Vector3 _pos) { m_objPos = _pos; }
    void SetArea(Vector3 _area) { m_area = _area; }
    void SetStartSpeed(float _speed) { m_startSpeed = _speed; }
    void SetEndSpeed(float _speed) { m_endSpeed = _speed; }
    void SetMinLife(float _time) { m_minLifeTime = _time; }
    void SetMaxLife(float _time) { m_maxLifeTime = _time; }

    virtual void UpdateData();
    virtual void Clear();
    virtual void Execute();

public :
    ParticleUpdateShader();
    ~ParticleUpdateShader();
};


#pragma once
#include "Obj.h"
class RigidBody :
    public Obj
{
private :
    Vector2 m_force;
    Vector2 m_accel;
    Vector2 m_velocity;
    float m_mass;

    float m_maxSpeed;
    float m_subSpeed;
    bool m_bFriction;

public :
    virtual void update();
    virtual void lateUpdate();

    void AddForce(Vector2 _force) { m_force += _force; }
    void AddVelocity(Vector2 _velocity) { m_velocity += _velocity; }
    void SetVelocity(Vector2 _velocity) { m_velocity = _velocity; }
    void SetMaxSpeed(float _speed) { m_maxSpeed = _speed; }
    void SetSubSpeed(float _speed) { m_subSpeed = _speed; }
    void SetFriction(bool _bFriction) { m_bFriction = _bFriction; }

    Vector2 GetVelocity() { return m_velocity; }

public :
    RigidBody();
    virtual ~RigidBody();
};


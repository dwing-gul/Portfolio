#include "stdafx.h"
#include "RigidBody.h"
#include "TimeManager.h"

RigidBody::RigidBody() :
	m_mass(1.f),
	m_maxSpeed(400.f),
	m_subSpeed(500.f),
	m_bFriction(true)
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::update()
{
	m_force = Vector2();
}

void RigidBody::lateUpdate()
{
	m_accel = m_force / m_mass;

	m_velocity += m_accel * DELTATIME;

	if (m_bFriction && m_force.IsZero())
	{
		Vector2 reverse = -m_velocity;
		if (Vector2(0.f, 0.f) != reverse)
			reverse.normalize();

		float sub = m_subSpeed * DELTATIME;
		float speed = m_velocity.length();

		if (speed < sub)
		{
			m_velocity = Vector2(0.f, 0.f);
		}
		else
		{
			m_velocity += reverse * sub;
		}
	}

	if (m_maxSpeed < m_velocity.length())
	{
		if(Vector2(0.f, 0.f) != m_velocity)
			m_velocity.normalize();
		m_velocity *= m_maxSpeed;
	}

	Vector2 pos = GetPos();
	pos += m_velocity * DELTATIME;
	SetPos(pos);

	Obj::lateUpdate();
}


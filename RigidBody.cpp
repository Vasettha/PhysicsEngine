#include "RigidBody.h"

RigidBody::RigidBody()
	: m_position(0.0f, 0.0f), m_orientation(0.0f),
	m_linearVelocity(0.0f, 0.0f), m_angularVelocity(0.0f),
	m_mass(1.0f), m_inertia(1.0f),
	m_forceAccumulator(0.0f, 0.0f), m_torqueAccumulator(0.0f),
	m_isStatic(false)
{
}

#include "RigidBody.h"

RigidBody::RigidBody(const Collider& collider)
	: m_position(0.0f, 0.0f), m_orientation(0.0f),
	m_linearVelocity(0.0f, 0.0f), m_angularVelocity(0.0f),
	m_mass(1.0f), m_inertia(1.0f),
	m_forceAccumulator(0.0f, 0.0f), m_torqueAccumulator(0.0f),
	m_isStatic(false), m_collider(collider)
{
}

void RigidBody::setPosition(const sf::Vector2f& pos)
{
	m_position = pos;
}

void RigidBody::setOrientation(const float& angle)
{
	m_orientation = angle;
}

void RigidBody::setLinearVelocity(const sf::Vector2f& vel)
{
	m_linearVelocity = vel;
}

void RigidBody::setAngularVelocity(const float& avel)
{
	m_angularVelocity = avel;
}

#include "RigidBody.h"

RigidBody::RigidBody(const Collider& collider, sf::Vector2f position)
	: m_position(position), m_orientation(0.0f),
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

const Collider& RigidBody::getCollider() const
{
	return m_collider;
}

const sf::Vector2f& RigidBody::getPosition() const
{
	return m_position;
}

const float& RigidBody::getOrientation() const
{
	return m_orientation;
}

const sf::Vector2f& RigidBody::getLinearVelocity() const
{
	return m_linearVelocity;
}

const float& RigidBody::getAngularVelocity() const
{
	return m_angularVelocity;
}

const sf::Vector2f& RigidBody::getCenter() const
{
	switch(m_collider.getShapeType())
	{
	case(Collider::ShapeType::CIRCLE):
	{
		return sf::Vector2f(m_position.x + m_collider.getRadius(), m_position.y + m_collider.getRadius());
		break;
	}
	case(Collider::ShapeType::RECTANGLE):
	{
		return m_position + m_collider.getHalfSides();
		break;
	}
	}
	
}

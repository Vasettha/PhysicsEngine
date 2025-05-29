#include "Collider.h"


// Constructor for circle
Collider::Collider(ShapeType shapeType, float radius, float resistitution = 1.0f, float friction = 0.0f )
	: m_shapeType(shapeType), u_radius(radius), m_restitution(resistitution), m_friction(friction), m_localOffset(sf::Vector2f(radius,radius))
{

}

// Constructor for rectangles
Collider::Collider(ShapeType shapeType, sf::Vector2f halfSides, float resistitution = 1.0f, float friction = 0.0f)
	: m_shapeType(shapeType), u_halfSides(halfSides), m_restitution(resistitution), m_friction(friction), m_localOffset(halfSides)
{

}

float Collider::getRadius() const
{
	if (m_shapeType == ShapeType::CIRCLE)
		return u_radius;
	else
		return -1.0;
}

sf::Vector2f Collider::getHalfSides() const
{
	if (m_shapeType == ShapeType::RECTANGLE)
		return u_halfSides;
	else
		return sf::Vector2f(-1.f,-1.f);
}

float Collider::getRestitution() const
{
	return m_restitution;
}

float Collider::getFriction() const
{
	return m_friction;
}

Collider::ShapeType Collider::getShapeType() const
{
	return m_shapeType;
}

void Collider::setRadius(const float& radius)
{
	if (m_shapeType == ShapeType::CIRCLE)
	{
		u_radius = radius;
		//calculateMass();
	}
}

void Collider::setHalfSides(const sf::Vector2f& halfSides)
{
	if (m_shapeType == ShapeType::RECTANGLE)
	{
		u_halfSides = halfSides;
		//calculateMass();
	}

}

void Collider::setRestitution(const float& restitution)
{
	m_restitution = restitution;
}

void Collider::setFriction(const float& friction)
{
	m_friction = friction;
}


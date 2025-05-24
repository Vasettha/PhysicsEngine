#include "Collider.h"


// Constructor for circle
Collider::Collider(ShapeType shapeType, float radius, float resistitution = 1.0f, float friction = 0.0f , Density density = Density::LIGHT)
	: m_shapeType(shapeType), u_radius(radius), m_restitution(resistitution), m_friction(friction), m_localOffset(sf::Vector2f(radius,radius)), m_density(density)
{

}

// Constructor for rectangles
Collider::Collider(ShapeType shapeType, sf::Vector2f halfSides, float resistitution = 1.0f, float friction = 0.0f, Density density = Density::LIGHT)
	: m_shapeType(shapeType), u_halfSides(halfSides), m_restitution(resistitution), m_friction(friction), m_localOffset(halfSides), m_density(density)
{

}

float Collider::getMass() const
{
	return m_mass;
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

Collider::Density Collider::getDensity() const
{
	return m_density;
}

void Collider::setRadius(const float& radius)
{
	if (m_shapeType == ShapeType::CIRCLE)
	{
		u_radius = radius;
		calculateMass();
	}
}

void Collider::setHalfSides(const sf::Vector2f& halfSides)
{
	if (m_shapeType == ShapeType::RECTANGLE)
	{
		u_halfSides = halfSides;
		calculateMass();
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

void Collider::setDensity(const Collider::Density& density)
{
	m_density = density;
	calculateMass();
}

float Collider::massPerPixel() const
{
	float mpp = 0.0f;
	switch (m_density)
	{
	case (Density::LIGHT):
		mpp = 100.0f;
		break;
	case(Density::MID):
		mpp = 200.0f;
		break;
	case(Density::HEAVY):
		mpp = 300.0f;
		break;
	}

	return mpp;
}

void Collider::calculateMass()
{
	switch (m_shapeType)
	{
	case ShapeType::CIRCLE:

		m_mass = PhysicsMath::areaCircle(u_radius) * massPerPixel();
		break;

	case ShapeType::RECTANGLE:

		m_mass = PhysicsMath::areaRectangle(u_halfSides) * massPerPixel();
		break;
	}
}

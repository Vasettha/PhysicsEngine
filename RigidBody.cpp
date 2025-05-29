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

void RigidBody::setDensity(const RigidBody::Density& density)
{
	m_density = density;
	calculateMass();
}

void RigidBody::setStatic(const bool& isStatic)
{
	m_isStatic = isStatic;
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

const RigidBody::Density& RigidBody::getDensity() const
{
	return m_density;
}

const float& RigidBody::getInvMass()
{
	if (m_isStatic) return 0.0f;

	return 1.f / getMass();
}

const float& RigidBody::getInvInertia()
{
	if (m_isStatic) return 0.0f;

	return 1.f / getInertia();
}

const float& RigidBody::getMass() 
{
	calculateMass();
	return m_mass;
}

const float& RigidBody::getInertia() 
{
	calculateInertia();
	return m_inertia;
}

const bool& RigidBody::getIsStatic()
{
	return m_isStatic;
}

const RigidBody::AABB& RigidBody::getAABB() const
{
	RigidBody::AABB aabb;
	switch (m_collider.getShapeType())
	{
	case(Collider::ShapeType::CIRCLE):
	{
		sf::Vector2f cirCent = getPosition();
		float rad = m_collider.getRadius();

		aabb.left = cirCent.x - rad;
		aabb.right = cirCent.x + rad;
		aabb.top = cirCent.y - rad;
		aabb.bottom = cirCent.y + rad;

		break;
	}
	case(Collider::ShapeType::RECTANGLE):
	{
		sf::Vector2f rectCent = getPosition();
		sf::Vector2f rectHalfSides = m_collider.getHalfSides();
		float rotationRadians = getOrientation() * M_PI / 180;

		// Four corner of the rectangle
		sf::Vector2f corners[4] =
		{
			{-rectHalfSides.x, -rectHalfSides.y},
			{-rectHalfSides.x, rectHalfSides.y},
			{ rectHalfSides.x, -rectHalfSides.y},
			{ rectHalfSides.x, rectHalfSides.y},
		};

		// Rotate it and find max min

		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::lowest();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::lowest();

		float sinA = std::sin(rotationRadians);
		float cosA = std::cos(rotationRadians);

		for (int i = 0; i < 4; i++)
		{
			sf::Vector2f worldCorner = sf::Vector2f((corners[i].x * cosA) - (corners[i].y * sinA),
				(corners[i].x * sinA) + (corners[i].y * cosA)) + rectCent;

			minX = std::min(minX, worldCorner.x);
			maxX = std::max(maxX, worldCorner.x);
			minY = std::min(minY, worldCorner.y);
			maxY = std::max(maxY, worldCorner.y);
		}

		aabb.left = minX;
		aabb.right = maxX;
		aabb.top = minY;
		aabb.bottom = maxY;

		break;
	}
	}

	return aabb;
}

void RigidBody::calculateMass()
{
	if (m_isStatic)
	{
		m_mass = std::numeric_limits<float>::max();
		return;
	}

	switch (m_collider.getShapeType())
	{
	case Collider::ShapeType::CIRCLE:

		m_mass = PhysicsMath::areaCircle(m_collider.getRadius()) * massPerPixel();
		break;

	case Collider::ShapeType::RECTANGLE:

		m_mass = PhysicsMath::areaRectangle(m_collider.getHalfSides()) * massPerPixel();
		break;
	}
}

void RigidBody::calculateInertia()
{
	if (m_isStatic)
	{
		m_inertia = std::numeric_limits<float>::max();
		return;
	}

	switch (m_collider.getShapeType())
	{
	case Collider::ShapeType::CIRCLE:

		m_inertia = 0.5f * getMass() * m_collider.getRadius() * m_collider.getRadius();
		break;

	case Collider::ShapeType::RECTANGLE:

		//TODO
		break;
	}
	
}

float RigidBody::massPerPixel()
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


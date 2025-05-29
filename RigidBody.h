#pragma once

#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Collider.h"

class RigidBody
{
public:

	struct AABB
	{
		// X
		float left;
		float right;
		// Y
		float top;
		float bottom;
	};

	enum class Density {
		LIGHT,
		MID,
		HEAVY
	};

	RigidBody(const Collider& collider, 
		sf::Vector2f position = sf::Vector2f(0.0f, 0.0f));
	//~RigidBody();

	void setPosition(const sf::Vector2f& pos);
	void setOrientation(const float& angle);
	void setLinearVelocity(const sf::Vector2f& vel);
	void setAngularVelocity(const float& avel);
	void setDensity(const RigidBody::Density& density);
	void setStatic(const bool& isStatic);
	
	const Collider& getCollider() const;

	const sf::Vector2f& getPosition() const;
	const float& getOrientation() const;
	const sf::Vector2f& getLinearVelocity() const;
	const float& getAngularVelocity() const;
	const RigidBody::Density& getDensity() const;
	const float& getInvMass();
	const float& getInvInertia();
	const float& getMass();
	const float& getInertia();
	const bool& getIsStatic();


	const RigidBody::AABB& getAABB() const;
	

private:

	sf::Vector2f m_position;
	float m_orientation;

	sf::Vector2f m_linearVelocity;
	float m_angularVelocity;

	float m_inertia;
	float m_mass;

	sf::Vector2f m_forceAccumulator;
	float m_torqueAccumulator;

	bool m_isStatic;
	RigidBody::Density m_density;

	Collider m_collider;

	void calculateMass();
	void calculateInertia();
	float massPerPixel();
	
};


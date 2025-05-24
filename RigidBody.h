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

	RigidBody(const Collider& collider);
	//~RigidBody();

private:

	void setPosition(const sf::Vector2f& pos);
	void setOrientation(const float& angle);
	void setLinearVelocity(const sf::Vector2f& vel);
	void setAngularVelocity(const float& avel);

	sf::Vector2f m_position;
	float m_orientation;

	sf::Vector2f m_linearVelocity;
	float m_angularVelocity;

	float m_mass;
	float m_inertia;

	sf::Vector2f m_forceAccumulator;
	float m_torqueAccumulator;

	bool m_isStatic;

	Collider m_collider;
	
};


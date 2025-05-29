#pragma once

#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "RigidBody.h"
#include "CollisionDetector.h"
#include "PhysicsMath.h"

class ColliderResolver
{
public:

	ColliderResolver();

	void resolveCollision(std::vector<CollisionDetector::ContactManifold>& contactManifolds, 
		std::vector<RigidBody>& bodies, float deltaTime);

private:

	void resolveSingleCollision(CollisionDetector::ContactManifold& contactManifold, 
		float deltaTime);
	void applyImpulse(RigidBody& A, RigidBody& B, const sf::Vector2f& normal,
		float depth, const sf::Vector2f& contactPoint);
	void positionalCorrection(RigidBody& A, RigidBody& B, const sf::Vector2f& normal,
		float depth);

	float m_positionalCorrectionFactor; // How much to correct penetration 
	float m_slop; // A small tolerance to avoid "jiggling" from very tiny penetrations
	sf::Clock m_clock;
};


#pragma once

// A simple collision detector that implements a broad and narrow phase
// Using Spatial hashing and SAT 

#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>

#include "RigidBody.h"
#include "PhysicsMath.h"


class CollisionDetector	
{
public:

	struct ContactManifold
	{
		const RigidBody* A;
		const RigidBody* B;
		sf::Vector2f normal; 
		float depth;
		sf::Vector2f contactPoint;
	};


	// Spacial hashing
	CollisionDetector(float cellSize = 64.f);

	void detectAll(const std::vector<RigidBody>& bodies);

private:

	void broadPhase(const std::vector<RigidBody>& bodies);
	void narrowPhase(const std::vector<RigidBody>& bodies);

	float m_cellSize;
	std::unordered_map<sf::Vector2f, std::vector<int>> m_spatialMap;
	std::set<std::pair<int, int>> m_collisionPairs;
	std::vector<ContactManifold> m_contactManifolds;

};


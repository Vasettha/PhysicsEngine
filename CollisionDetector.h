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
		RigidBody* A;
		RigidBody* B;
		sf::Vector2f normal; 
		float depth;
		sf::Vector2f contactPoint;
	};


	// Spacial hashing
	CollisionDetector(float cellSize = 128.f);

	void detectAll(std::vector<RigidBody>& bodies);
	std::vector<CollisionDetector::ContactManifold>& getContactManifold();

private:

	void broadPhase(std::vector<RigidBody>& bodies);
	void narrowPhase(std::vector<RigidBody>& bodies);

	float m_cellSize;
	std::unordered_map<sf::Vector2f, std::vector<int>> m_spatialMap;
	std::set<std::pair<int, int>> m_collisionPairs;
	std::vector<CollisionDetector::ContactManifold> m_contactManifolds;

};


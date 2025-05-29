#pragma once

#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <optional>
#include <vector>

#include "PhysicsMath.h"
#include "Collider.h"
#include "RigidBody.h"
#include "CollisionDetector.h"
#include "ColliderResolver.h"

class World
{
public:

	World();
	~World();

	void run();

private:
	// Window
	sf::VideoMode m_videoMode;
	sf::RenderWindow m_window;
	// Event
	std::optional<sf::Event> m_ev;

	// Main Functions
	void init();
	void update();
	void render();

	// Secondary Functions
	void pollEvents();
	void updateMousePos();
	void updateObjects(const float& deltaTime);

	sf::Clock m_clock;

	void addCircle(float radius, float resistitution = 1.0f, 
		float friction = 0.0f, 
		sf::Vector2f position = sf::Vector2f(0.0f, 0.0f));
	void addRectangle(sf::Vector2f sides, float resistitution = 1.0f,
		float friction = 0.0f, 
		sf::Vector2f position = sf::Vector2f(0.0f, 0.0f));
	void removeObj();

	// Variables
	bool m_isRunning = true;
	sf::Vector2i m_mousePosWindow;
	sf::Vector2f m_mousePosView;

	std::vector<RigidBody> m_objects;
	CollisionDetector m_detector;
	ColliderResolver m_resolver;

};
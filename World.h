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

	void addCircle(float radius, float resistitution, 
		float friction, Collider::Density density);
	void addRectangle(sf::Vector2f sides, float resistitution,
		float friction, Collider::Density density);
	void removeObj();

	// Variables
	bool m_isRunning = true;
	sf::Vector2i m_mousePosWindow;
	sf::Vector2f m_mousePosView;

	std::vector<RigidBody> m_objects;

};
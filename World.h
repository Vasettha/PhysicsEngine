#pragma once

#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <optional>

#include "CircleObject.h"
#include "PhysicsMath.h"

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

	// Objects
	CircleObject m_circle;

	// Main Functions
	void init();
	void update();
	void render();

	// Secondary Functions
	void pollEvents();
	void updateMousePos();
	void clickAndCarry();
	void onPressedClickAndShoot();
	void onReleaseClickAndShoot();
	void updateObjects();
	void frictionUpdate();
	void gravityUpdate();

	// World logic
	bool withinCircleBound(const CircleObject& cir);

	// Variables
	bool m_isRunning = true;
	sf::Vector2i m_mousePosWindow;
	sf::Vector2f m_mousePosView;

	//Click and carry
	bool m_leftMousePressed = false;
	bool m_leftMouseDrag = false;
	std::optional<sf::Vector2f> m_leftDragOffset;

	//Click and shoot
	sf::Vector2f m_rightDragStart;

	//Friction
	float friction = 0.99f;

	//Gravity
	float gravity = 0.1f;

	// Collision with border
	void handleCircleBorderCollision(CircleObject& cir);
	
};


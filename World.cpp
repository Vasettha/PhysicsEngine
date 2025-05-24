#include "World.h"

World::World()
{
	m_objects.reserve(50);
}

World::~World()
{
}

void World::run()
{
	init();

	while(m_isRunning)
	{
		update();
		render();
	}
}

void World::init()
{
	// Init Variables 

	// Init Window
	m_videoMode = sf::VideoMode(sf::Vector2u(800, 600));
	m_window = sf::RenderWindow(m_videoMode, "Physics Engine", sf::Style::Titlebar | sf::Style::Close);
	m_window.setFramerateLimit(60);

	// Init Font
	// Init Text
	// Init Objects


}

void World::update()
{
	// Handle user events
	pollEvents();
	updateMousePos();
}

void World::render()
{
	m_window.clear();

	m_window.display();
}


// ---------------------------------------

void World::pollEvents()
{
	// Event poll
	while (m_ev = m_window.pollEvent())
	{
		if (m_ev->is<sf::Event::Closed>())
		{
			m_window.close();
			m_isRunning = false;
		}
		else if (auto keyPressed = m_ev->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
			{
				m_window.close();
				m_isRunning = false;
			}
		}
	}
}

void World::updateMousePos()
{
	m_mousePosWindow = sf::Mouse::getPosition(m_window);
	m_mousePosView = m_window.mapPixelToCoords(m_mousePosWindow);
}

void World::addCircle(float radius, float resistitution = 1.0f, 
	float friction = 0.0f, Collider::Density density = Collider::Density::LIGHT)
{
	m_objects.emplace_back(Collider(Collider::ShapeType::CIRCLE, radius,
		resistitution, friction, density));
}

void World::addRectangle(sf::Vector2f sides, float resistitution = 1.0f, 
	float friction = 0.0f, Collider::Density density = Collider::Density::LIGHT)
{
	m_objects.emplace_back(Collider(Collider::ShapeType::RECTANGLE, sides, 
		resistitution, friction, density));
}

void World::removeObj()
{
	m_objects.pop_back();
}

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

	addCircle(20.0f, 1.0f, 0.0f, Collider::Density::LIGHT, sf::Vector2f(50.0f, 100.0f));
	addRectangle(sf::Vector2f(40.0f, 40.0f), 1.0f, 0.0f, Collider::Density::LIGHT, sf::Vector2f(500.0f, 100.0f));

}

void World::update()
{
	// Handle user events
	pollEvents();
	updateMousePos();
	
	updateObjects();
}

void World::render()
{
	m_window.clear();

	for (auto& obj: m_objects)
	{
		switch(obj.getCollider().getShapeType())
		{

		case (Collider::ShapeType::CIRCLE):
		{
			sf::CircleShape circle((float)obj.getCollider().getRadius());
			circle.setPosition(obj.getPosition());
			circle.setFillColor(sf::Color::Green);
			m_window.draw(circle);
			break;
		}

		case (Collider::ShapeType::RECTANGLE):
		{
			sf::RectangleShape rectangle(sf::Vector2f(obj.getCollider().getHalfSides().x * 2, obj.getCollider().getHalfSides().y * 2));
			rectangle.setPosition(obj.getPosition());
			rectangle.setFillColor(sf::Color::Red);
			m_window.draw(rectangle);
			break;
		}

		}
	}

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

void World::updateObjects()
{
	// Update position based on object's velocities

	// Detect collision

	// Resolve collision

}

void World::addCircle(float radius, float resistitution, 
	float friction, Collider::Density density, sf::Vector2f position)
{
	m_objects.emplace_back(Collider(Collider::ShapeType::CIRCLE, radius,
		resistitution, friction, density), position);
}

void World::addRectangle(sf::Vector2f sides, float resistitution, 
	float friction, Collider::Density density, sf::Vector2f position)
{
	m_objects.emplace_back(Collider(Collider::ShapeType::RECTANGLE, sides, 
		resistitution, friction, density),position);
}

void World::removeObj()
{
	m_objects.pop_back();
}

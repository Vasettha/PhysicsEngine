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

	//addRectangle(sf::Vector2f(40.0f, 40.0f), 1.0f, 0.0f, sf::Vector2f(50.0f, 100.0f));
	addCircle(40.0f, 1.0f, 0.0f, sf::Vector2f(50.0f, 100.0f));
	m_objects.back().setLinearVelocity(sf::Vector2f(1, 0));
	addCircle(40.0f, 1.0f, 0.0f, sf::Vector2f(400.0f, 100.0f));


}

void World::update()
{
	float deltaTime = m_clock.restart().asSeconds();
	// Handle user events
	pollEvents();
	updateMousePos();
	
	updateObjects(deltaTime);
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
			circle.setOrigin(sf::Vector2f(obj.getCollider().getRadius(), obj.getCollider().getRadius()));
			circle.setPosition(obj.getPosition());
			circle.setFillColor(sf::Color::Green);
			m_window.draw(circle);
			break;
		}

		case (Collider::ShapeType::RECTANGLE):
		{
			sf::RectangleShape rectangle(sf::Vector2f(obj.getCollider().getHalfSides().x * 2, obj.getCollider().getHalfSides().y * 2));
			rectangle.setOrigin(sf::Vector2f(obj.getCollider().getHalfSides().x * 2, obj.getCollider().getHalfSides().y * 2));
			rectangle.setPosition(obj.getPosition());
			rectangle.setFillColor(sf::Color::Red);
			rectangle.setRotation(sf::Angle(sf::degrees(180)));
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

void World::updateObjects(const float& deltaTime)
{
	// Update position based on object's velocities
	for (auto& obj:m_objects)
	{
		obj.setPosition(obj.getPosition() + obj.getLinearVelocity());
		obj.setOrientation(obj.getOrientation() + obj.getAngularVelocity());
	}
	// Detect collision
	m_detector.detectAll(m_objects);

	// Resolve collision
	m_resolver.resolveCollision(m_detector.getContactManifold(), m_objects, deltaTime);

}

void World::addCircle(float radius, float resistitution, 
	float friction, sf::Vector2f position)
{
	m_objects.emplace_back(Collider(Collider::ShapeType::CIRCLE, radius,
		resistitution, friction), position);
}

void World::addRectangle(sf::Vector2f sides, float resistitution, 
	float friction, sf::Vector2f position)
{
	m_objects.emplace_back(Collider(Collider::ShapeType::RECTANGLE, sides, 
		resistitution, friction),position);
}

void World::removeObj()
{
	m_objects.pop_back();
}

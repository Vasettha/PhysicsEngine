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

// Perimeter circles acting as a wall with gaps
	float perimeter_circle_radius = 10.0f;
	// Spacing: current circle diameter (2*radius) + gap (2*radius) = 4*radius
	float spacing = 4.0f * perimeter_circle_radius;

	// Top border
	for (float x = perimeter_circle_radius; x <= 800.0f - perimeter_circle_radius; x += spacing)
	{
		addCircle(perimeter_circle_radius, 1.0f, 0.5f, sf::Vector2f(x, perimeter_circle_radius));
		m_objects.back().setStatic(true);
	}

	// Bottom border
	for (float x = perimeter_circle_radius; x <= 800.0f - perimeter_circle_radius; x += spacing)
	{
		addCircle(perimeter_circle_radius, 1.0f, 0.5f, sf::Vector2f(x, 600.0f - perimeter_circle_radius));
		m_objects.back().setStatic(true);
	}

	// Left border (avoiding corners already covered by top/bottom)
	for (float y = perimeter_circle_radius + spacing; y <= 600.0f - perimeter_circle_radius - spacing; y += spacing)
	{
		addCircle(perimeter_circle_radius, 1.0f, 0.5f, sf::Vector2f(perimeter_circle_radius, y));
		m_objects.back().setStatic(true);
	}

	// Right border (avoiding corners already covered by top/bottom)
	for (float y = perimeter_circle_radius + spacing; y <= 600.0f - perimeter_circle_radius - spacing; y += spacing)
	{
		addCircle(perimeter_circle_radius, 1.0f, 0.5f, sf::Vector2f(800.0f - perimeter_circle_radius, y));
		m_objects.back().setStatic(true);
	}


	// Central large stationary circle (like a central obstacle)
	addCircle(60.0f, 0.8f, 0.6f, sf::Vector2f(400.0f, 300.0f));
	m_objects.back().setLinearVelocity(sf::Vector2f(0.f, 0.f));
	m_objects.back().setAngularVelocity(0.f);

	// Active circles for chaotic interactions (increased number and varied initial states)
	// 1. Kicking circle (large, fast)
	addCircle(40.0f, 0.9f, 0.4f, sf::Vector2f(100.0f, 200.0f));
	m_objects.back().setLinearVelocity(sf::Vector2f(250.f, 70.f));
	m_objects.back().setAngularVelocity(0.f);

	// 2. Smaller circle for chained reactions
	addCircle(20.0f, 0.9f, 0.3f, sf::Vector2f(300.0f, 250.0f));
	m_objects.back().setLinearVelocity(sf::Vector2f(0.f, 0.f));
	m_objects.back().setAngularVelocity(0.f);

	// 3. Another small circle
	addCircle(25.0f, 0.85f, 0.35f, sf::Vector2f(500.0f, 350.0f));
	m_objects.back().setLinearVelocity(sf::Vector2f(-100.f, -50.f));
	m_objects.back().setAngularVelocity(0.f);

	// 4. Circle from top-left, aiming for a cluster
	addCircle(30.0f, 0.9f, 0.4f, sf::Vector2f(150.0f, 150.0f));
	m_objects.back().setLinearVelocity(sf::Vector2f(180.f, 120.f));
	m_objects.back().setAngularVelocity(0.f);

	// 5. Circle from bottom-right, aiming for chaos
	addCircle(35.0f, 0.8f, 0.5f, sf::Vector2f(650.0f, 450.0f));
	m_objects.back().setLinearVelocity(sf::Vector2f(-150.f, -100.f));
	m_objects.back().setAngularVelocity(0.f);

	// Additional small circles for more chaotic motion
	addCircle(15.0f, 0.95f, 0.2f, sf::Vector2f(200.0f, 400.0f));
	m_objects.back().setLinearVelocity(sf::Vector2f(100.f, -200.f));
	m_objects.back().setAngularVelocity(0.f);

	addCircle(18.0f, 0.8f, 0.3f, sf::Vector2f(550.0f, 180.0f));
	m_objects.back().setLinearVelocity(sf::Vector2f(-180.f, 80.f));
	m_objects.back().setAngularVelocity(0.f);

	addCircle(22.0f, 0.9f, 0.25f, sf::Vector2f(400.0f, 500.0f));
	m_objects.back().setLinearVelocity(sf::Vector2f(0.f, -150.f));
	m_objects.back().setAngularVelocity(0.f);

	addCircle(28.0f, 0.7f, 0.4f, sf::Vector2f(100.0f, 50.0f));
	m_objects.back().setLinearVelocity(sf::Vector2f(200.f, 200.f));
	m_objects.back().setAngularVelocity(0.f);

	addCircle(17.0f, 0.88f, 0.32f, sf::Vector2f(700.0f, 500.0f));
	m_objects.back().setLinearVelocity(sf::Vector2f(-250.f, -100.f));
	m_objects.back().setAngularVelocity(0.f);
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
	m_window.clear(sf::Color::Blue);

	for (auto& obj: m_objects)
	{
		switch(obj.getCollider().getShapeType())
		{

		case (Collider::ShapeType::CIRCLE):
		{
			float radius = obj.getCollider().getRadius();
			sf::CircleShape circle(radius);
			circle.setOrigin(sf::Vector2f(radius, radius));
			circle.setPosition(obj.getPosition());
			circle.setFillColor(sf::Color::Green);
			m_window.draw(circle);

			// Draw a line to indicate orientation
			float angleRad = obj.getOrientation() * static_cast<float>(M_PI) / 180.0f; // Convert degrees to radians
			sf::Vector2f lineEndPoint(
				obj.getPosition().x + radius * std::cos(angleRad),
				obj.getPosition().y + radius * std::sin(angleRad)
			);

			sf::Vertex line[] =
			{
				sf::Vertex(obj.getPosition(), sf::Color::Black), // Start at the center
				sf::Vertex(lineEndPoint, sf::Color::Black)      // End at the edge based on orientation
			};
			m_window.draw(line, 2, sf::PrimitiveType::Lines);
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
		std::cout << obj.getPosition().x << obj.getPosition().y << '\n';
		obj.setPosition(obj.getPosition() + obj.getLinearVelocity()*deltaTime);
		obj.setOrientation(obj.getOrientation() + obj.getAngularVelocity()*deltaTime);
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

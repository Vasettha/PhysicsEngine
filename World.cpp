#include "World.h"

World::World(): m_circle(20.0f, sf::Vector2f(40.0f, 40.0f), sf::Color::Blue)
{
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
	m_circle.setVelocity(sf::Vector2f(0.0f, 0.0f));

}

void World::update()
{
	// Handle user events
	pollEvents();
	updateMousePos();
	clickAndCarry();

	updateObjects();
	frictionUpdate();
	gravityUpdate();

	handleCircleBorderCollision(m_circle);
}

void World::render()
{
	m_window.clear();

	m_circle.render(m_window);

	m_window.display();
}

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
		else if (auto mousePressed = m_ev->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mousePressed->button == sf::Mouse::Button::Left)
			{
				m_leftMousePressed = true;
			}
			if (mousePressed->button == sf::Mouse::Button::Right)
			{
				onPressedClickAndShoot();
			}
		}
		else if (auto mousePressed = m_ev->getIf<sf::Event::MouseButtonReleased>())
		{
			if (mousePressed->button == sf::Mouse::Button::Left)
			{
				m_leftMousePressed = false;
				m_leftMouseDrag = false;
				m_leftDragOffset.reset();
			}
			if (mousePressed->button == sf::Mouse::Button::Right)
			{
				onReleaseClickAndShoot();
			}
		}
	}
}

void World::updateMousePos()
{
	m_mousePosWindow = sf::Mouse::getPosition(m_window);
	m_mousePosView = m_window.mapPixelToCoords(m_mousePosWindow);
}

void World::clickAndCarry()
{
	if(m_leftMousePressed)
	{
		if (withinCircleBound(m_circle))
		{
			if (!m_leftMouseDrag)
			{
				m_circle.setVelocity(sf::Vector2f(0.0f, 0.0f));
				m_leftMouseDrag = true;
				m_leftDragOffset = PhysicsMath::getDisplacement(m_mousePosView, m_circle.getCenter());
			}
		}
		if (m_leftDragOffset)
			m_circle.setCenterPosition(sf::Vector2f(m_mousePosView - m_leftDragOffset.value()));
	}
}

void World::onPressedClickAndShoot()
{
	m_rightDragStart = m_mousePosView;
}

void World::onReleaseClickAndShoot()
{
	sf::Vector2f disp = PhysicsMath::getDisplacement(m_rightDragStart, m_mousePosView);
	m_circle.setVelocity(sf::Vector2f(disp.x,disp.y));
}

void World::updateObjects()
{
	m_circle.updatePosition();
}

void World::frictionUpdate()
{
	sf::Vector2f newVelocity(m_circle.getVelocity().x * friction, m_circle.getVelocity().y * friction);
	//if (newVelocity.x < 0.5) newVelocity.x = 0;
	//if (newVelocity.y < 0.5) newVelocity.y = 0;
	m_circle.setVelocity(newVelocity);
}

void World::gravityUpdate()
{
	sf::Vector2f newVelocity(m_circle.getVelocity().x, m_circle.getVelocity().y + gravity);
	m_circle.setVelocity(newVelocity);
}

bool World::withinCircleBound(const CircleObject& cir)
{
	float cirRad = cir.getRadius();

	// Offset cirPos to get the mid point
	sf::Vector2f cirPos = m_circle.getCenter();

	// Compute the absolute distance between our 2 points
	float distance = PhysicsMath::getDistance(cirPos, m_mousePosView);

	if (distance < cirRad)
	{
		return true;
	}

	return false;
}

void World::handleCircleBorderCollision(CircleObject& cir)
{
	sf::Vector2f cirPos = cir.getCenter();
	
	// Top border
	sf::Vector2f topPoint = sf::Vector2f(cirPos.x, cirPos.y - cir.getRadius());
	if (topPoint.y < 0)
	{
		// Bounce off top border
		cir.setVelocity(sf::Vector2f(cir.getVelocity().x,-cir.getVelocity().y));
	}
	
	// Bottom border
	sf::Vector2f bottomPoint = sf::Vector2f(cirPos.x, cirPos.y + cir.getRadius());
	if (bottomPoint.y > m_window.getSize().y)
	{
		// Bounce off Bottom border
		cir.setVelocity(sf::Vector2f(cir.getVelocity().x, -cir.getVelocity().y));
	}

	// Right border
	sf::Vector2f rightPoint = sf::Vector2f(cirPos.x + cir.getRadius(), cirPos.y );
	if (rightPoint.x > m_window.getSize().x)
	{
		// Bounce off Right border
		cir.setVelocity(sf::Vector2f(-cir.getVelocity().x, cir.getVelocity().y));
	}

	// Left border
	sf::Vector2f leftPoint = sf::Vector2f(cirPos.x - cir.getRadius(), cirPos.y);
	if (topPoint.x < 0)
	{
		// Bounce off Left border
		cir.setVelocity(sf::Vector2f(-cir.getVelocity().x, cir.getVelocity().y));
	}

}

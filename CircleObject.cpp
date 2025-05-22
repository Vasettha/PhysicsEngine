#include "CircleObject.h"

CircleObject::CircleObject(float radius, sf::Vector2f pos, sf::Color color)
{
	m_circle.setRadius(radius);
	m_circle.setPosition(pos);
	m_circle.setFillColor(color);

	m_velocity = sf::Vector2f(0.0f, 0.0f);
}

CircleObject::~CircleObject()
{
}

void CircleObject::render(sf::RenderTarget& window)
{
	window.draw(m_circle);
}

sf::Vector2f CircleObject::getPosition() const
{
	return m_circle.getPosition();
}

float CircleObject::getRadius() const
{
	return m_circle.getRadius();
}

sf::Vector2f CircleObject::getCenter() const
{
	return m_circle.getPosition() + sf::Vector2f(m_circle.getRadius(),m_circle.getRadius());
}

void CircleObject::setPosition(const sf::Vector2f& pos)
{
	m_circle.setPosition(pos);
}

void CircleObject::setCenterPosition(sf::Vector2f pos)
{
	m_circle.setPosition(pos - sf::Vector2f(m_circle.getRadius(), m_circle.getRadius()));
}

void CircleObject::setVelocity(const sf::Vector2f& vel) 
{
	m_velocity = vel;
}

sf::Vector2f CircleObject::getVelocity()
{
	return m_velocity;
}

void CircleObject::updatePosition()
{
	m_circle.setPosition(getPosition() + getVelocity());
}

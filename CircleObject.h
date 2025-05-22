#pragma once

#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class CircleObject
{
public:
	CircleObject(float radius, sf::Vector2f pos, sf::Color color);
	~CircleObject();

	void render(sf::RenderTarget& window);
	
	// Helper 
	sf::Vector2f getPosition() const;
	sf::Vector2f getCenter() const;
	float getRadius() const;

	void setPosition(const sf::Vector2f& pos);
	void setCenterPosition(sf::Vector2f pos);

	void setVelocity(const sf::Vector2f& vel);
	sf::Vector2f getVelocity();

	void updatePosition();
private:
	sf::CircleShape m_circle;
	sf::Vector2f m_velocity;
};


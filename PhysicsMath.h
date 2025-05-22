#pragma once

//#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

namespace PhysicsMath
{
	float dotProduct(const sf::Vector2f& A, const sf::Vector2f& B);
	sf::Vector2f getDisplacement(const sf::Vector2f& A, const sf::Vector2f& B);
	sf::Vector2f getUnitDirection(const sf::Vector2f& A, const sf::Vector2f& B);
	float getDistance(const sf::Vector2f& A, const sf::Vector2f& B);
	float resultant(const sf::Vector2f& A, const sf::Vector2f& B);
}
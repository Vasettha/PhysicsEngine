#pragma once

//#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <math.h>
#include <corecrt_math_defines.h>

namespace PhysicsMath
{
	float areaCircle(const float& radius);
	float areaRectangle(const sf::Vector2f& halfSides);

	float distance(const sf::Vector2f& pointA, const sf::Vector2f& pointB);

	float dotProduct(const sf::Vector2f& vecA, const sf::Vector2f& vecB);

	sf::Vector2f normalize(const sf::Vector2f& vec);

}

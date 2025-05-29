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
	float crossProduct(const sf::Vector2f& vecA, const sf::Vector2f& vecB);

	sf::Vector2f rotate(const sf::Vector2f& vec, float angleRadians);
	sf::Vector2f normalize(const sf::Vector2f& vec);

}


namespace std {
    template <>
    struct hash<sf::Vector2f> {
        size_t operator()(const sf::Vector2f& v) const {
            // A simple hash combination function for two floats
            // You can use a more robust hash if collisions become an issue,
            // but this is often sufficient.
            // Using a standard technique to combine hashes of individual components
            auto h1 = hash<float>{}(v.x);
            auto h2 = hash<float>{}(v.y);

            // Combine hashes:
            // A common way to combine two hashes: (h1 ^ (h2 << 1))
            // Or boost's hash_combine: h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2))
            return h1 ^ (h2 << 1); // Simple combination
        }
    };
}
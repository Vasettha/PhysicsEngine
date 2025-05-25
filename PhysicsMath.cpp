#include "PhysicsMath.h"

float PhysicsMath::areaCircle(const float& radius)
{
	
	return pow(radius, 2) * M_PI;
}

float PhysicsMath::areaRectangle(const sf::Vector2f& halfSides)
{
	return halfSides.x * halfSides.y * 4;
}

float PhysicsMath::distance(const sf::Vector2f& pointA, const sf::Vector2f& pointB)
{
	//Euclidean Distance Formula
	return pow((pointA.x - pointB.x),2) + pow((pointA.y - pointB.y),2);
}

float PhysicsMath::dotProduct(const sf::Vector2f& vecA, const sf::Vector2f& vecB)
{
	return (vecA.x * vecB.x) + (vecA.y * vecB.y);
}

sf::Vector2f PhysicsMath::normalize(const sf::Vector2f& vec)
{
	float distance = sqrt(vec.x * vec.x + vec.y * vec.y);
	if (distance == 0) return (sf::Vector2f(0.0f, 0.0f));

	return sf::Vector2f(vec.x/distance,vec.y/distance);
}

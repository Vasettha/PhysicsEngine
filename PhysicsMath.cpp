#include "PhysicsMath.h"

float PhysicsMath::dotProduct(const sf::Vector2f& A, const sf::Vector2f& B)
{
	return (A.x * B.x) + (A.y * B.y);
}

sf::Vector2f PhysicsMath::getDisplacement(const sf::Vector2f& A, const sf::Vector2f& B)
{
	// Displacement vector
	sf::Vector2f dir(A.x - B.x, A.y - B.y);

	return dir;
}

sf::Vector2f PhysicsMath::getUnitDirection(const sf::Vector2f& A, const sf::Vector2f& B)
{
	//Displacement vector
	sf::Vector2f dir(A.x - B.x, A.y - B.y);

	// Normalize
	dir = dir.normalized();
	return sf::Vector2f();
}

float PhysicsMath::getDistance(const sf::Vector2f& A, const sf::Vector2f& B)
{
	return sqrt(pow((A.x - B.x), 2) + pow((A.y - B.y), 2));
}

float PhysicsMath::resultant(const sf::Vector2f& A, const sf::Vector2f& B)
{
	
	return sqrt(pow(A.x + B.x, 2) + pow(A.y + B.y, 2));
}

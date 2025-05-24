#include "PhysicsMath.h"

float PhysicsMath::areaCircle(const float& radius)
{
	
	return pow(radius, 2) * M_PI;
}

float PhysicsMath::areaRectangle(const sf::Vector2f& halfSides)
{
	return halfSides.x * halfSides.y * 4;
}

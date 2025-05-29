#pragma once

#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "PhysicsMath.h"

class Collider
{
public:

	enum class ShapeType
	{
		CIRCLE, RECTANGLE
	};

	// Constructor for circle
	Collider(ShapeType shapeType, float radius,
		float resistitution, float friction);

	//Constructor for Rectangle
	Collider(ShapeType shapeType, sf::Vector2f halfSides,
		float resistitution, float friction);

	// Functions

	float getRadius() const;
	sf::Vector2f getHalfSides() const;
	float getRestitution() const;
	float getFriction() const;
	Collider::ShapeType getShapeType() const;

	void setRadius(const float& radius);
	void setHalfSides(const sf::Vector2f& halfSides);
	void setRestitution(const float& restitution);
	void setFriction(const float& friction);

private:

	union { sf::Vector2f u_halfSides; float u_radius;};
	ShapeType m_shapeType;
	float m_restitution; // Bounciness (0.0 - 1.0)
	float m_friction; // Resistance to sliding 0.0 -> ++

	sf::Vector2f m_localOffset; 

};
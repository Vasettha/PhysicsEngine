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

	enum class Density {
		LIGHT,
		MID,
		HEAVY
	};

	// Constructor for circle
	Collider(ShapeType shapeType, float radius,
		float resistitution, float friction,
		Density density);

	//Constructor for Rectangle
	Collider(ShapeType shapeType, sf::Vector2f halfSides,
		float resistitution, float friction, 
		Density density);

	// Functions
	float getMass() const;

	float getRadius() const;
	sf::Vector2f getHalfSides() const;
	float getRestitution() const;
	float getFriction() const;
	Collider::Density getDensity() const;

	void setRadius(const float& radius);
	void setHalfSides(const sf::Vector2f& halfSides);
	void setRestitution(const float& restitution);
	void setFriction(const float& friction);
	void setDensity(const Collider::Density& density);

private:

	union { sf::Vector2f u_halfSides; float u_radius;};
	ShapeType m_shapeType;
	float m_restitution; // Bounciness (0.0 - 1.0)
	float m_friction; // Resistance to sliding 0.0 -> ++
	Density m_density;
	float m_mass;

	float massPerPixel() const;
	void calculateMass();

	sf::Vector2f m_localOffset; 

};
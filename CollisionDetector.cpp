#include "CollisionDetector.h"

CollisionDetector::CollisionDetector(float cellSize)
	: m_cellSize(cellSize)
{
	m_contactManifolds.reserve(50);
}

void CollisionDetector::detectAll(const std::vector<RigidBody>& bodies)
{
	broadPhase(bodies);
	narrowPhase(bodies);
}

void CollisionDetector::broadPhase(const std::vector<RigidBody>& bodies)
{
	// Put all the bodies's AABB into the right spacial bin

	m_spatialMap.clear();
	m_collisionPairs.clear();

	for(int i = 0; i < bodies.size(); i++)
	{
		RigidBody::AABB aabb = bodies[i].getAABB();

		int left = aabb.left / m_cellSize;
		int right = aabb.right / m_cellSize;
		int top = aabb.top / m_cellSize;
		int bottom = aabb.bottom / m_cellSize;
		
		for (int x = left; x <= right; x++)
		{
			for (int y = top; y <= bottom; y++)
			{
				m_spatialMap[sf::Vector2f(x,y)].push_back(i);
			}
		}

	}

	// Figure out the pairs to check

	for(auto& cell:m_spatialMap)
	{
		if (cell.second.size() < 2)
		{
			break;
		}

		for (int i = 0; i < cell.second.size(); i++)
		{
			for (int j = i + 1; j < cell.second.size(); j++)
			{
				int id_i = cell.second[i];
				int id_j = cell.second[j];

				if (id_i < id_j)
				{
					m_collisionPairs.insert(std::pair<int, int>(id_i, id_j));
				}
				else
				{
					m_collisionPairs.insert(std::pair<int, int>(id_j, id_i));
				}
			}
		}
	}
}

void CollisionDetector::narrowPhase(const std::vector<RigidBody>& bodies)
{
	// Determine if 2 objects collider. If it does, generate contact manifold

	for (auto& pair: m_collisionPairs)
	{
		const RigidBody& A = bodies[pair.first];
		const RigidBody& B = bodies[pair.second];

		Collider::ShapeType stA = A.getCollider().getShapeType();
		Collider::ShapeType stB = B.getCollider().getShapeType();

		

		if (stA == Collider::ShapeType::CIRCLE && stB == Collider::ShapeType::CIRCLE)
		{
			// Circle-Circle 
			sf::Vector2f diff = A.getCenter() - B.getCenter();

			float distSq = PhysicsMath::dotProduct(diff, diff);
			float combinedRadius = A.getCollider().getRadius() + B.getCollider().getRadius();
			
			float combinedRadiusSq = combinedRadius * combinedRadius;

			if (distSq < combinedRadiusSq) // then a collision happen
			{
				float dist = sqrt(distSq);

				// Create the Contact Manifold
				CollisionDetector::ContactManifold contactManifold;

				contactManifold.A = &A;
				contactManifold.B = &B;
				contactManifold.normal = PhysicsMath::normalize(diff);
				contactManifold.depth = combinedRadius - dist;
				contactManifold.contactPoint = A.getCenter() + 
					contactManifold.normal * A.getCollider().getRadius();

				m_contactManifolds.push_back(contactManifold);
			}

			

		}
		else if (stA == Collider::ShapeType::CIRCLE && stB == Collider::ShapeType::RECTANGLE 
			|| stA == Collider::ShapeType::RECTANGLE && stB == Collider::ShapeType::CIRCLE)
		{
			const RigidBody* circleBody = nullptr;
			const RigidBody* rectangleBody = nullptr;

			if (stA == Collider::ShapeType::RECTANGLE && stB == Collider::ShapeType::CIRCLE)
			{
				rectangleBody = &A;
				circleBody = &B;
			}
			else
			{
				circleBody = &A;
				rectangleBody = &B;
			}

			// Circle-Rectangle, using OBB

			// Convert to local rectangle coordinate system 
			sf::Vector2f translatedCircleCenter = circleBody->getCenter() - rectangleBody->getCenter();

			// "unrotate" the rectangle
			float inverseRotationRadians = - (rectangleBody->getOrientation() * M_PI / 180);

			float sinA = std::sin(inverseRotationRadians);
			float cosA = std::cos(inverseRotationRadians);

			sf::Vector2f localCircleCenter(
				translatedCircleCenter.x * cosA - translatedCircleCenter.y * sinA,
				translatedCircleCenter.x * sinA + translatedCircleCenter.y * cosA
			);

			// Find the closest local rotated x and y 
			sf::Vector2f rectHalfSides = rectangleBody->getCollider().getHalfSides();
			sf::Vector2f LocalClosest(
				std::clamp(localCircleCenter.x, -rectHalfSides.x, rectHalfSides.x),
				std::clamp(localCircleCenter.y, -rectHalfSides.y, rectHalfSides.y)
				); // Closest point of the rectangle

			// Check if distance from closest point < radius
			float distanceX = localCircleCenter.x - LocalClosest.x;
			float distanceY = localCircleCenter.y - LocalClosest.y;
			float distanceSq = (distanceX * distanceX) + (distanceY * distanceY);

			if (distanceSq < (circleBody->getCollider().getRadius() * circleBody->getCollider().getRadius()))
			{
				// Then a collision happens
				
				// Transform the closest point to world coordinate
				float rotationRadians = rectangleBody->getOrientation() * M_PI / 180;

				float sinA = std::sin(rotationRadians);
				float cosA = std::cos(rotationRadians);

				sf::Vector2f reRotateClosest (
					LocalClosest.x * cosA - LocalClosest.y * sinA,
					LocalClosest.x * sinA + LocalClosest.y * cosA
				);

				sf::Vector2f worldRectangleClosestPoint = rectangleBody->getCenter() + reRotateClosest;

				// Find the direction of the collision based on the closest point
				sf::Vector2f collisionNormal = circleBody->getCenter() - worldRectangleClosestPoint;

				// Create the Contact Manifold
				CollisionDetector::ContactManifold contactManifold;

				contactManifold.A = circleBody;
				contactManifold.B = rectangleBody;
				contactManifold.normal = PhysicsMath::normalize(collisionNormal);
				contactManifold.depth = circleBody->getCollider().getRadius() - sqrt(distanceSq);
				contactManifold.contactPoint = worldRectangleClosestPoint;

				m_contactManifolds.push_back(contactManifold);
			}
			
		}
		else if (stA == Collider::ShapeType::RECTANGLE && stB == Collider::ShapeType::RECTANGLE)
		{
			// Rectangle-Rectangle 

		}


	}
}

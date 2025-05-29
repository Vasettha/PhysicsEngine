#include "CollisionDetector.h"

CollisionDetector::CollisionDetector(float cellSize)
	: m_cellSize(cellSize)
{
	m_contactManifolds.reserve(50);
}

void CollisionDetector::detectAll(std::vector<RigidBody>& bodies)
{
	broadPhase(bodies);
	narrowPhase(bodies);
}

std::vector<CollisionDetector::ContactManifold>& CollisionDetector::getContactManifold()
{
	return m_contactManifolds;
}

void CollisionDetector::broadPhase(std::vector<RigidBody>& bodies)
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

void CollisionDetector::narrowPhase(std::vector<RigidBody>& bodies)
{
	// Determine if 2 objects collider. If it does, generate contact manifold

	for (auto& pair: m_collisionPairs)
	{
		RigidBody& A = bodies[pair.first];
		RigidBody& B = bodies[pair.second];

		Collider::ShapeType stA = A.getCollider().getShapeType();
		Collider::ShapeType stB = B.getCollider().getShapeType();

		

		if (stA == Collider::ShapeType::CIRCLE && stB == Collider::ShapeType::CIRCLE)
		{
			// Circle-Circle 
			sf::Vector2f diff = A.getPosition() - B.getPosition();

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
				contactManifold.contactPoint = A.getPosition() + 
					contactManifold.normal * A.getCollider().getRadius();

				m_contactManifolds.push_back(contactManifold);
			}

			

		}
		else if (stA == Collider::ShapeType::CIRCLE && stB == Collider::ShapeType::RECTANGLE 
			|| stA == Collider::ShapeType::RECTANGLE && stB == Collider::ShapeType::CIRCLE)
		{
			RigidBody* circleBody = nullptr;
			RigidBody* rectangleBody = nullptr;

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
			sf::Vector2f translatedCircleCenter = circleBody->getPosition() - rectangleBody->getPosition();

			// "unrotate" the rectangle
			float inverseRotationRadians = - (rectangleBody->getOrientation() * M_PI / 180);

			sf::Vector2f localCircleCenter(PhysicsMath::rotate(translatedCircleCenter, inverseRotationRadians));

			// Find the closest local rotated x and y 
			sf::Vector2f rectHalfSides = rectangleBody->getCollider().getHalfSides();
			sf::Vector2f localClosest(
				std::clamp(localCircleCenter.x, -rectHalfSides.x, rectHalfSides.x),
				std::clamp(localCircleCenter.y, -rectHalfSides.y, rectHalfSides.y)
				); // Closest point of the rectangle

			// Check if distance from closest point < radius
			float distanceX = localCircleCenter.x - localClosest.x;
			float distanceY = localCircleCenter.y - localClosest.y;
			float distanceSq = (distanceX * distanceX) + (distanceY * distanceY);

			if (distanceSq < (circleBody->getCollider().getRadius() * circleBody->getCollider().getRadius()))
			{
				// Then a collision happens
				
				// Transform the closest point to world coordinate
				float rotationRadians = rectangleBody->getOrientation() * M_PI / 180;

				sf::Vector2f reRotateClosest (PhysicsMath::rotate(localClosest,rotationRadians));

				sf::Vector2f worldRectangleClosestPoint = rectangleBody->getPosition() + reRotateClosest;

				// Find the direction of the collision based on the closest point
				sf::Vector2f collisionNormal = circleBody->getPosition() - worldRectangleClosestPoint;

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
			// Using SAT

			sf::Vector2f centA = A.getPosition();
			sf::Vector2f centB = B.getPosition();

			float angleRadA = A.getOrientation() * M_PI/180;
			float angleRadB = B.getOrientation() * M_PI/180;

			sf::Vector2f halfSidesA = A.getCollider().getHalfSides();
			sf::Vector2f halfSidesB = B.getCollider().getHalfSides();

			std::array<sf::Vector2f, 4> verticesA;
			std::array<sf::Vector2f, 4> verticesB;

			verticesA[0] = centA + PhysicsMath::rotate(sf::Vector2f(halfSidesA.x, halfSidesA.y), angleRadA);
			verticesA[1] = centA + PhysicsMath::rotate(sf::Vector2f(halfSidesA.x, -halfSidesA.y), angleRadA);
			verticesA[2] = centA + PhysicsMath::rotate(sf::Vector2f(-halfSidesA.x, halfSidesA.y), angleRadA);
			verticesA[3] = centA + PhysicsMath::rotate(sf::Vector2f(-halfSidesA.x, -halfSidesA.y), angleRadA);

			verticesB[0] = centB + PhysicsMath::rotate(sf::Vector2f(halfSidesB.x, halfSidesB.y), angleRadB);
			verticesB[1] = centB + PhysicsMath::rotate(sf::Vector2f(halfSidesB.x, -halfSidesB.y), angleRadB);
			verticesB[2] = centB + PhysicsMath::rotate(sf::Vector2f(-halfSidesB.x, halfSidesB.y), angleRadB);
			verticesB[3] = centB + PhysicsMath::rotate(sf::Vector2f(-halfSidesB.x, -halfSidesB.y), angleRadB);

			std::array<sf::Vector2f, 4> axes;

			axes[0] = PhysicsMath::normalize(PhysicsMath::rotate(sf::Vector2f(1.0f, 0.0f), angleRadA));
			axes[1] = PhysicsMath::normalize(PhysicsMath::rotate(sf::Vector2f(0.0f, 1.0f), angleRadA));

			axes[2] = PhysicsMath::normalize(PhysicsMath::rotate(sf::Vector2f(1.0f, 0.0f), angleRadB));
			axes[3] = PhysicsMath::normalize(PhysicsMath::rotate(sf::Vector2f(0.0f, 1.0f), angleRadB));

			float minOverlap = std::numeric_limits<float>::max();
			sf::Vector2f collisionNormal;
			bool collided = true;

			for (auto& axis: axes)
			{
				
				
				float maxA = std::numeric_limits<float>::lowest();
				float minA = std::numeric_limits<float>::max();
				float maxB = std::numeric_limits<float>::lowest();
				float minB = std::numeric_limits<float>::max();

				for(auto& vecA: verticesA)
				{
					float dot = PhysicsMath::dotProduct(axis, vecA);
					if (dot < minA) minA = dot;
					if (dot > maxA) maxA = dot;	
				}

				for (auto& vecB : verticesB)
				{
					float dot = PhysicsMath::dotProduct(axis, vecB);
					if (dot < minB) minB = dot;
					if (dot > maxB) maxB = dot;
				}

				if (minA > maxB || maxA < minB)
				{
					// Then there is a separating axis!
					collided = false;
					break;
				}
				else
				{
					// There is an overlap
					float currentOverlap = std::min(maxA, maxB) - std::max(minA, minB);
					if (currentOverlap < minOverlap) 
					{
						minOverlap = currentOverlap;
						collisionNormal = axis;
					}
				}
			}

			if (collided)
			{
				// Generate Contact manifold
				CollisionDetector::ContactManifold contactManifold;
				contactManifold.A = &A;
				contactManifold.B = &B;
				contactManifold.normal = collisionNormal;
				contactManifold.depth = minOverlap;
				//contactManifold.contactPoint = ; TODO

				m_contactManifolds.push_back(contactManifold);
			}


		}


	}
}

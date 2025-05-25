#include "ColliderResolver.h"

ColliderResolver::ColliderResolver()
	: m_positionalCorrectionFactor(0.2), m_slop(0.01)
{
}

void ColliderResolver::resolveCollision(std::vector<CollisionDetector::ContactManifold>& contactManifolds, std::vector<RigidBody>& bodies, float deltaTime)
{
}

void ColliderResolver::resolveSingleCollision(CollisionDetector::ContactManifold& contactManifolds, float deltaTime)
{
}

void ColliderResolver::applyImpulse(RigidBody& A, RigidBody& B, const sf::Vector2f& normal, float depth, float restitution, float friction)
{
}

void ColliderResolver::positionalCorrection(RigidBody& A, RigidBody& B, const sf::Vector2f& normal, float depth)
{
}

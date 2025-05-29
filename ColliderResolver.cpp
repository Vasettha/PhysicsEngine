#include "ColliderResolver.h"

ColliderResolver::ColliderResolver()
	: m_positionalCorrectionFactor(0.2), m_slop(0.01)
{
}

void ColliderResolver::resolveCollision(std::vector<CollisionDetector::ContactManifold>& contactManifolds, std::vector<RigidBody>& bodies, float deltaTime)
{

}

void ColliderResolver::resolveSingleCollision(CollisionDetector::ContactManifold& contactManifold, float deltaTime)
{
	positionalCorrection(*contactManifold.A,*contactManifold.B,contactManifold.normal,contactManifold.depth);
	applyImpulse(*contactManifold.A, *contactManifold.B, contactManifold.normal, contactManifold.depth);
}

void ColliderResolver::applyImpulse(RigidBody& A, RigidBody& B, const sf::Vector2f& normal, float depth)
{
	float restitution = std::min(A.getCollider().getRestitution(), B.getCollider().getRestitution());
	float friction = std::min(A.getCollider().getFriction(), B.getCollider().getFriction());

	float invMassA = A.getInvMass();
	float invMassB = B.getInvMass();



}

void ColliderResolver::positionalCorrection(RigidBody& A, RigidBody& B, const sf::Vector2f& normal, float depth)
{
	float invMassA = A.getInvMass();
	float invMassB = B.getInvMass();

	if (invMassA + invMassB == 0) return; // If both are static

	float correction = (std::max(depth - m_slop, 0.0f) / (invMassA + invMassB)
		* m_positionalCorrectionFactor);

	A.setPosition(A.getPosition() - normal * correction * invMassA);
	B.setPosition(B.getPosition() + normal * correction * invMassB);
	
}

#include "ColliderResolver.h"

ColliderResolver::ColliderResolver()
	: m_positionalCorrectionFactor(0.2), m_slop(0.01)
{
}

void ColliderResolver::resolveCollision(std::vector<CollisionDetector::ContactManifold>& contactManifolds,
	std::vector<RigidBody>& bodies, float deltaTime)
{
	for(auto& manifold:contactManifolds)
	{
		resolveSingleCollision(manifold, deltaTime);
	}
}

void ColliderResolver::resolveSingleCollision(CollisionDetector::ContactManifold& contactManifold, float deltaTime)
{
	positionalCorrection(*contactManifold.A,*contactManifold.B,contactManifold.normal,contactManifold.depth);
	applyImpulse(*contactManifold.A, *contactManifold.B, contactManifold.normal, contactManifold.depth, contactManifold.contactPoint);
}

void ColliderResolver::applyImpulse(RigidBody& A, RigidBody& B, const sf::Vector2f& normal, float depth, const sf::Vector2f& contactPoint)
{
	float restitution = std::min(A.getCollider().getRestitution(), B.getCollider().getRestitution());
	float friction = std::min(A.getCollider().getFriction(), B.getCollider().getFriction());

	float invMassA = A.getInvMass();
	float invMassB = B.getInvMass();

	// Linear
	sf::Vector2f relativeVelocity = A.getLinearVelocity() - B.getLinearVelocity();
	float velNormal = PhysicsMath::dotProduct(relativeVelocity, normal);

	float impulse = -(1 + restitution) * velNormal / (invMassA + invMassB);
	A.setLinearVelocity(A.getLinearVelocity() - impulse * invMassA * normal);
	B.setLinearVelocity(B.getLinearVelocity() + impulse * invMassB * normal);

	// Angular
	sf::Vector2f rA = contactPoint - A.getPosition();
	sf::Vector2f rB = contactPoint - B.getPosition();

	A.setAngularVelocity(A.getAngularVelocity() -
		(impulse * A.getInvInertia() * PhysicsMath::crossProduct(rA, normal)));
	B.setAngularVelocity(B.getAngularVelocity() -
		(impulse * B.getInvInertia() * PhysicsMath::crossProduct(rB, normal)));
		


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

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

	float impulseScalar = -(1 + restitution) * velNormal / (invMassA + invMassB);
	A.setLinearVelocity(A.getLinearVelocity() + impulseScalar * invMassA * normal);
	B.setLinearVelocity(B.getLinearVelocity() - impulseScalar * invMassB * normal);

	// Angular
	sf::Vector2f rA = contactPoint - A.getPosition();
	sf::Vector2f rB = contactPoint - B.getPosition();

	A.setAngularVelocity(A.getAngularVelocity() -
		(impulseScalar * A.getInvInertia() * PhysicsMath::crossProduct(rA, normal)));
	B.setAngularVelocity(B.getAngularVelocity() -
		(impulseScalar * B.getInvInertia() * PhysicsMath::crossProduct(rB, normal)));
		


}

void ColliderResolver::positionalCorrection(RigidBody& A, RigidBody& B, const sf::Vector2f& normal, float depth)
{
	float invMassA = A.getInvMass();
	float invMassB = B.getInvMass();

	float penetration = std::max(depth - m_slop, 0.0f);
	float totalInvMass = invMassA + invMassB;

	if (totalInvMass == 0.0f) return; // Avoid division by zero if both are static

	sf::Vector2f correctionOffset = normal * (penetration / totalInvMass) * m_positionalCorrectionFactor;

	A.setPosition(A.getPosition() + correctionOffset * invMassA); 
	B.setPosition(B.getPosition() - correctionOffset * invMassB); 
	
}

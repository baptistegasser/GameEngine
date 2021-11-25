#pragma once

#include "core/Component.h"
#include "PxPhysicsAPI.h"
#include "ForceMode.h"

class RigidBody : public Pitbull::Component {
public:
	/// <summary>
	/// The different type of physx actor that can be created
	/// </summary>
	enum class RigidActorType {
		Static,
		Dynamic,
		Kinematic
	};

	~RigidBody() override;

	/// <summary>
	/// Specific function used to update the component before the fixed update.
	/// </summary>
	void PreFixedTick() const;
	void FixedTick(const float& DeltaTime) override;

	/// <summary>
	/// Adds a force to the Rigidbody.
	/// </summary>
	void AddForce(const physx::PxVec3& Force, const ForceMode& ForceMode = ForceMode::Force) const;
	/// <summary>
	/// Adds a torque to the Rigidbody.
	/// </summary>
	void AddTorque(const physx::PxVec3& Torque, const ForceMode& ForceMode) const;
	/// <summary>
	/// Set the Rigidbody's velocity.
	///	Not recommended, please use \ref AddForce.
	/// </summary>
	void SetVelocity(const physx::PxVec3& Velocity) const;
	/// <summary>
	/// Set the Rigidbody's mass, assuming constant density.
	/// </summary>
	void SetMass(float Mass) const;
	/// <summary>
	/// Enable or disable gravity on the actor.
	/// </summary>
	void SetFollowGravity(bool FollowGravity) const;
	/// <summary>
	/// Set the target so the actor follow a path to it.
	/// </summary>
	void setKinematicTarget(const physx::PxTransform& Target) const;

protected:
	friend class Pitbull::Actor;
	friend class PhysicManager;
	RigidBody(Pitbull::Actor* Parent, RigidActorType ActorType);

private:
	RigidActorType ActorType;
	physx::PxRigidActor* RigidActor;

	/// <summary>
	/// Get the current actor as a dynamic one.
	///	\warning This will fail if used on a static actor.
	/// </summary>
	[[nodiscard]] physx::PxRigidDynamic* GetAsDynamic() const noexcept;
};

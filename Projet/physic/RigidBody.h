#pragma once

#include "core/Component.h"
#include "PxPhysicsAPI.h"
#include "ForceMode.h"

#include "math/Vec3f.h"
#include "math/Transform.h"

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

	void Init() override;
	~RigidBody() override = default;

	void PrePhysicSimulation() noexcept;
	void PostPhysicSimulation() noexcept;

	const bool IsStatic() const noexcept;
	const bool IsDynamic() const noexcept;
	const bool IsKinematic() const noexcept;

	/// <summary>
	/// Adds a force to the Rigidbody.
	/// </summary>
	void AddForce(const Math::Vec3f& Force, const ForceMode& ForceMode = ForceMode::Force) const;
	/// <summary>
	/// Adds a torque to the Rigidbody.
	/// </summary>
	void AddTorque(const Math::Vec3f& Torque, const ForceMode& ForceMode = ForceMode::Force) const;
	/// <summary>
	/// Set the Rigidbody's velocity.
	///	Not recommended, please use \ref AddForce.
	/// </summary>
	void SetVelocity(const Math::Vec3f& Velocity) const;
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
	void setKinematicTarget(const Math::Transform& Target) const;

	/// <summary>
	/// Get the Rigidbody's velocity.
	/// </summary>
	const Math::Vec3f GetVelocity() const;

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

	/// <summary>
	/// Update the shapes associated to the physx actor.
	/// </summary>
	void UpdateActorShapes() noexcept;

	/// <summary>
	/// Store the previous scale to detect scale changes.
	/// This is needed as changing/recreating shapes is costly.
	/// </summary>
	Math::Vec3f PreviousScale;
};

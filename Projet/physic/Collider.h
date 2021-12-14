#pragma once

#include <functional>

#include "core/Component.h"
#include "PhysicMaterial.h"
#include "physic/Contact.h"
#include "math/Vec3f.h"

#include "PxPhysicsAPI.h"

/// <summary>
/// Base class for all colliders.
/// </summary>
class Collider : public Pitbull::Component {
public:
	// Default values to prevent duplication in sub-classes.
	const static PhysicMaterial DefaultMaterial;
	const static Math::Vec3f DefaultOffset;

	/// <summary>
	/// The material defining the physic properties applied to a collision with this collider.
	/// </summary>
	PhysicMaterial Material;
	/// <summary>
	/// The offset of this collider from the actor center.
	/// </summary>
	Math::Vec3f Offset;

	std::function<void(Contact)> OnContactCallBack = [](const Contact& Contact) -> void {};

	/// <summary>
	/// Create a collider with an offset and material.
	/// </summary>
	/// <param name="Offset">The offset from the parent center, default to center (0.f,0.f,0.f)</param>
	/// <param name="Material">The collider physic material, default to no bounciness and mild friction</param>
	/// <param name="Scale">The base scale, default to unit size (1.f,1.f,1.f)</param>
	Collider(Pitbull::Actor* Parent, const PhysicMaterial& Material = DefaultMaterial, const Math::Vec3f& Offset = DefaultOffset);
	~Collider() override = default;

	/// <summary>
	/// Method called when two actors enter in collision.
	/// </summary>
	/// <param name="Contact">The contact information.</param>
	virtual void OnContact(const Contact& Contact) const
	{
		OnContactCallBack(Contact);
	}

	/// <summary>
	/// Get the geometry of this collider, scaled appropriatly.
	/// The desired scale is defined by \ref Scale, modifiable by \ref SetScale
	/// \warning This should be called only on scale change 
	/// </summary>
	/// <returns>The physx geometry object of this collider</returns>
	physx::PxGeometry* GetPxGeometry(const Math::Vec3f& Scale) noexcept;
	/// <summary>
	/// Get the material of this collider.
	/// </summary>
	/// <returns>The physx material object of this collider</returns>
	physx::PxMaterial* GetPxMaterial() noexcept;

protected:
	/// <summary>
	/// Abstract method that return the actual geometry specific to the
	/// collider inheriting this base class.
	/// The scale should appropriatly calculated base on the geometry implementation.
	/// </summary>
	virtual physx::PxGeometry* GetGeometryImpl(const Math::Vec3f& Scale) const noexcept = 0;
};

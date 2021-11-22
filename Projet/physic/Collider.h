#pragma once

#include "core/Component.h"
#include "PhysicMaterial.h"
#include "Contact.h"

#include "PxPhysicsAPI.h"

class Collider : public Pitbull::Component {
public:
	~Collider() override = default;

	virtual void OnContact(const Contact& Contact) const {
		std::cout << 1;
	}

	physx::PxGeometry* GetPxGeometry();
	physx::PxMaterial* GetPxMaterial();

protected:
	Collider(Pitbull::Actor* Parent, const PhysicMaterial& Material);
	virtual physx::PxGeometry&& GetGeometryImpl() const = 0;

private:
	std::unique_ptr<physx::PxGeometry> Geometry;
	PhysicMaterial Material;
};

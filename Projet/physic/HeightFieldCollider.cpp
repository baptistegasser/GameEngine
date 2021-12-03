#include "stdafx.h"
#include "HeightFieldCollider.h"
#include "render/Terrain.h"
#include "PhysicManager.h"

using namespace physx;

PxGeometry* HeightFieldCollider::GetGeometryImpl(const Math::Vec3f& Scale) const noexcept
{
	return new PxHeightFieldGeometry(HeightField, PxMeshGeometryFlags(),
		Scale.x, Scale.y, Scale.z
	);
}

HeightFieldCollider::HeightFieldCollider(Pitbull::Actor* Parent, const PhysicMaterial& Material, const ATerrain* Terrain)
	: Collider{ Parent, Material, DefaultOffset }
{
	unsigned int i{ 0 };

	const auto Samples = new PxHeightFieldSample[Terrain->VertexCount];
	for (int x = 0; x < Terrain->Width; x++) // No idea why I have to flip
		for (int z = 0; z < Terrain->Height; z++)
		{
			Samples[i++] = PxHeightFieldSample{
			static_cast<short>(Terrain->GetVertex(x, z).Position.y),
			0,
			0
			};
		}

	PxHeightFieldDesc HeightFieldDesc;
	HeightFieldDesc.format = PxHeightFieldFormat::eS16_TM;
	HeightFieldDesc.nbColumns = Terrain->Height;
	HeightFieldDesc.nbRows = Terrain->Width;
	HeightFieldDesc.samples.data = Samples;
	HeightFieldDesc.samples.stride = sizeof(PxHeightFieldSample);

	auto& PhysicManager = PhysicManager::GetInstance();
	HeightField = PhysicManager.Cooking->createHeightField(HeightFieldDesc, PhysicManager.Physics->getPhysicsInsertionCallback());

	delete[] Samples;
}

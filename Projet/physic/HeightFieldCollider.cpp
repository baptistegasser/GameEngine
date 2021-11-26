#include "stdafx.h"
#include "HeightFieldCollider.h"
#include "render/Landscape.h"
#include "PhysicManager.h"

using namespace physx;

PxGeometry* HeightFieldCollider::GetGeometryImpl() const
{
	return new PxHeightFieldGeometry(HeightField, PxMeshGeometryFlags(),
		HeightScale, RowScale, ColumnScale
	);
}

HeightFieldCollider::HeightFieldCollider(Pitbull::Actor* Parent, const PhysicMaterial& Material, const Landscape* Landscape)
	: Collider{ Parent, Material }
	, HeightScale(Landscape->Scale.z)
	, ColumnScale(Landscape->Scale.x)
	, RowScale(Landscape->Scale.y)
{
	Samples = new PxHeightFieldSample[Landscape->Vertices.size()];

	unsigned int i{ 0 };
	for (const auto& Vertex : Landscape->Vertices)
	{
		Samples[i++] = PxHeightFieldSample{ static_cast<short>(Vertex.Position.z) }; //TODO materials
	}

	PxHeightFieldDesc HeightFieldDesc;
	HeightFieldDesc.format = PxHeightFieldFormat::eS16_TM;
	HeightFieldDesc.nbColumns = Landscape->Height;
	HeightFieldDesc.nbRows = Landscape->Width;
	HeightFieldDesc.samples.data = Samples;
	HeightFieldDesc.samples.stride = sizeof(PxHeightFieldSample);

	auto& PhysicManager = PhysicManager::GetInstance();
	HeightField = PhysicManager.Cooking->createHeightField(HeightFieldDesc, PhysicManager.Physics->getPhysicsInsertionCallback());
}

#include "stdafx.h"
#include "ContactFilter.h"

using namespace physx;

PxFilterFlags ContactFilter::filterShader(PxFilterObjectAttributes attributes0,
	PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// let triggers through
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	// Default behaviour : notify components
	pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eMODIFY_CONTACTS;

	return PxFilterFlag::eDEFAULT;
}

void ContactFilter::setupFilter(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask)
{
	PxFilterData filterData;
	filterData.word0 = filterGroup;
	filterData.word1 = filterMask;

	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = (PxShape**)malloc(sizeof(PxShape*) * numShapes);
	actor->getShapes(shapes, numShapes);

	for (PxU32 i = 0; i < numShapes; i++) {
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}

	free(shapes);
}

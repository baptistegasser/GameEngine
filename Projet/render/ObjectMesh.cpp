#include "stdafx.h"
#include "ObjectMesh.h"
#include "render/MeshLoader.h"

ObjectMesh::ObjectMesh(const wchar_t* MeshName)
{
	OMBMeshLoader::Load(MeshName, *this);
}

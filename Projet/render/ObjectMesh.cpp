#include "stdafx.h"
#include "ObjectMesh.h"

#include "resources/Resource.h"
#include "render/MeshLoader.h"
#include "util/Util.h"

ObjectMesh::ObjectMesh(const wchar_t* MeshName)
{
	OMBMeshLoader::Load(MeshName, *this);
}

ObjectMesh::~ObjectMesh()
{
	DX_RELEASE(PVertexBuffer);
	DX_RELEASE(PIndexBuffer);
}

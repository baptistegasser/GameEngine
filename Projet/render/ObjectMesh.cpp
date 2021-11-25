#include "stdafx.h"
#include "ObjectMesh.h"
#include "render/MeshLoader.h"
#include "util/Util.h"

ObjectMesh::ObjectMesh(const wchar_t* MeshName)
{
	OMBMeshLoader::Load(MeshName, *this);
}

ObjectMesh::~ObjectMesh()
{
	PM3D::DXRelacher(PVertexBuffer);
	PM3D::DXRelacher(PIndexBuffer);
}

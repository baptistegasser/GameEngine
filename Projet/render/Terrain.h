#pragma once

#include "stdafx.h"
#include <vector>

#include "ShaderTerrain.h"
#include "core/Actor.h"
#include "Vertex.h"
#include "ObjectMesh.h"

class PhysicMaterial;

class ATerrain : public Pitbull::Actor
{
public:
	using vertex_t = Vertex;
	using index_t = unsigned int;

	int Height;
	int Width;
	DirectX::XMFLOAT3 Scale;
	float TextureCoefficient = 20.f;
	bool IsTunnel = false;

	size_t VertexCount;
	size_t PolyCount;

	std::vector<vertex_t> Vertices;
	std::vector<index_t> Indexes;

	ID3D11Buffer* PVertexBuffer;
	ID3D11Buffer* PIndexBuffer;
	ShaderTerrain* MeshShader;

	Texture* Texture1;
	Texture* Texture2;
	Texture* Texture3;

	ATerrain(const wchar_t* FileName, DirectX::XMFLOAT3 Scale, ShaderTerrain* Shader, const PhysicMaterial& Material, bool BackFaceCulling = false);
	~ATerrain() override;
	void LateTick(const float ElapsedTime) override;

	[[nodiscard]] const vertex_t& GetVertex(int x, int z) const;
private:
	DirectX::XMMATRIX matWorld;
	ShadersParamsTerrain ShaderParams;

	void ComputeNormal(int x, int z);
	void ComputeNormals();
	void GenerateMesh();
	void ComputeIndexes();
	[[nodiscard]] DirectX::XMVECTOR GetVertexPositionVector(int x, int z);
	[[nodiscard]] vertex_t& GetVertex(int x, int z);

	bool BackFaceCulling;
};

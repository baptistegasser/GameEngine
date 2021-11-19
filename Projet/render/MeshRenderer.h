#pragma once

#include "core/Component.h"
#include "Material.h"
#include "Shader.h"

class MeshRenderer : public Pitbull::Component {
public:
	MeshRenderer(Pitbull::Actor* Parent, const std::string& MeshFileName);
	MeshRenderer(Pitbull::Actor* Parent, const std::string& MeshFileName, Shader& MeshShader);
	~MeshRenderer() = default;

	void Init() override;
	void Tick(const float& DeltaTime) override;

private:
	std::string MeshFileName;
	XMMATRIX matWorld;
	Shader MeshShader;
	ID3D11Buffer* PIndexBuffer;
	ID3D11Buffer* PVertexBuffer;
	int SubsetCount;
	std::vector<int> SubsetMaterialIndex;
	std::vector<int> SubsetIndex;
	std::vector<Material> Materials;
};

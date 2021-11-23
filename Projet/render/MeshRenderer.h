#pragma once

#include "core/Component.h"
#include "ObjectMesh.h"
#include "Shader.h"

class MeshRenderer : public Pitbull::Component {
public:
	MeshRenderer(Pitbull::Actor* Parent, ObjectMesh* Mesh);
	MeshRenderer(Pitbull::Actor* Parent, ObjectMesh* Mesh, Shader* MeshShader);
	~MeshRenderer() = default;

	void Tick(const float& DeltaTime) override;

	ShadersParams ShaderParams;

private:
	DirectX::XMMATRIX matWorld;
	ObjectMesh* Mesh;
	Shader* MeshShader;
};

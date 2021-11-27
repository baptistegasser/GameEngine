#pragma once

#include "core/Component.h"
#include "ObjectMesh.h"
#include "Shader.h"
#include "Light.h"

class MeshRenderer : public Pitbull::Component {
public:
	MeshRenderer(Pitbull::Actor* Parent, ObjectMesh* Mesh);
	MeshRenderer(Pitbull::Actor* Parent, ObjectMesh* Mesh, Shader* MeshShader);
	~MeshRenderer() = default;

	void LateTick(const float& ElapsedTime) override;

	ShadersParams ShaderParams;

private:
	DirectX::XMMATRIX matWorld;
	ObjectMesh* Mesh;
	Shader* MeshShader;
};

#pragma once

#include "core/Component.h"
#include "ObjectMesh.h"
#include "Shader.h"

class MeshRenderer : public Pitbull::Component {
public:
	MeshRenderer(Pitbull::Actor* Parent, ObjectMesh* Mesh);
	MeshRenderer(Pitbull::Actor* Parent, ObjectMesh* Mesh, Shader* MeshShader);
	~MeshRenderer() override = default;

	void LateTick(const float& ElapsedTime) override;

	ObjectMesh* Mesh;

private:
	DirectX::XMMATRIX matWorld;
	Shader* MeshShader;

	void RenderShadowMap();
};

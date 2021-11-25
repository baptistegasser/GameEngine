#pragma once

#include "core/Component.h"
#include "ObjectMesh.h"
#include "Shader.h"
#include "Light.h"

class MeshRenderer : public Pitbull::Component {
public:
	MeshRenderer(Pitbull::Actor* Parent, ObjectMesh* Mesh, Light& Light);
	MeshRenderer(Pitbull::Actor* Parent, ObjectMesh* Mesh, Light& Light, Shader* MeshShader);
	~MeshRenderer() = default;

	void Tick(const float& DeltaTime) override;

	ShadersParams ShaderParams;

	LightShader LightParams;

	LightShader LightParams3;

private:
	DirectX::XMMATRIX matWorld;
	ObjectMesh* Mesh;
	Shader* MeshShader;
	Light LightShader1;
};

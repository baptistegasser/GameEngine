#pragma once

#include "core/Actor.h"
#include "ObjectMesh.h"
#include "Shader.h"
#include "Light.h"
#include "math/Transform.h"

class Skybox : public Pitbull::Actor {
public:
	Skybox(Math::Transform* ToFollow, ObjectMesh* Mesh);
	Skybox(Math::Transform* ToFollow, ObjectMesh* Mesh, Shader* MeshShader);
	~Skybox() = default;

	/// <summary>
	/// Render the skybox aroud the transform ToFollow
	/// </summary>
	/// <param name="ElapsedTime"></param>
	void LateTick(const float ElapsedTime) override;

	ShadersParams ShaderParams;

private:
	DirectX::XMMATRIX matWorld;
	ObjectMesh* Mesh;
	Shader* MeshShader;
	Math::Transform* ToFollow;
};

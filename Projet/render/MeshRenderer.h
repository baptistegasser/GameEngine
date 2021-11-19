#pragma once

#include "core/Component.h"
#include "Material.h"
#include "Shader.h"

class MeshRenderer : public Pitbull::Component {
public:
	~MeshRenderer() = default;

private:
	Shader Shader;
	std::vector<Material> Materials;
};

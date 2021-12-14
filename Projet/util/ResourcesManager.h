#pragma once

#include <map>
#include <memory>
#include <stdexcept>
#include <string>

// Don't include manadged ressources in case they might need the
// ResourcesManager which would lead to circulard dependancies
#include "render/Shader.h"
#include "render/Texture.h"
#include "render/ObjectMesh.h"
#include "render/Sprite.h"

class ResourcesManager {
private:
	std::map<const std::string, std::unique_ptr<Shader>> Shaders;
	std::map<const std::string, std::unique_ptr<Texture>> Textures;
	std::map<const std::string, std::unique_ptr<ObjectMesh>> Meshes;
	std::map<const std::string, std::unique_ptr<ShaderSprite>> Sprites;

public:
	ResourcesManager();
	~ResourcesManager();
	void Cleanup() noexcept;

	Shader* GetShader(const wchar_t* ShaderName);
	ShaderSprite* GetShaderSprite(const wchar_t* ShaderName);
	Texture* GetTexture(const std::wstring& TextureName);
	ObjectMesh* GetMesh(const wchar_t* MeshName);

private:
	static ULONG_PTR GDIToken;
};

#pragma once

#include <map>
#include <memory>
#include <stdexcept>
#include <string>

// Don't include manadged ressources in case they might need the
// ResourcesManager which would lead to circulard dependancies
struct Shader;
struct ObjectMesh;
struct Texture;
//struct ShaderSprite;

class ResourcesManager {
private:
	std::map<const std::string, std::unique_ptr<Shader>> Shaders;
	std::map<const std::string, std::unique_ptr<Texture>> Textures;
	std::map<const std::string, std::unique_ptr<ObjectMesh>> Meshes;
	//std::map<const std::string, std::unique_ptr<ShaderSprite>> ShadersSprite;

public:
	~ResourcesManager();
	void Cleanup() noexcept;

	Shader* GetShader(const wchar_t* ShaderName);
	//ShaderSprite* GetShaderBillBoard(const wchar_t* ShaderName);
	Texture* GetTexture(const std::wstring& TextureName);
	ObjectMesh* GetMesh(const wchar_t* MeshName);
};

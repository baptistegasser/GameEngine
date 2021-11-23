#pragma once

#include <map>
#include <memory>
#include <stdexcept>
#include <string>

// Don't include manadged ressources in case they might need the
// ResourcesManager which would lead to circulard dependancies
struct Shader;
struct ObjectMesh;
namespace PM3D {
	class CTexture;
}

class ResourcesManager {
private:
	std::map<const std::string, std::unique_ptr<Shader>> Shaders;
	std::map<const std::string, std::unique_ptr<PM3D::CTexture>> Textures;
	std::map<const std::string, std::unique_ptr<ObjectMesh>> Meshes;

public:
	~ResourcesManager();
	void Cleanup() noexcept;

	Shader* GetShader(const wchar_t* ShaderName);
	PM3D::CTexture* GetTexture(const std::wstring& TextureName);
	ObjectMesh* GetMesh(const wchar_t* MeshName);
};

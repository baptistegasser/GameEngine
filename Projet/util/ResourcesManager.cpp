#include "stdafx.h"
#include "ResourcesManager.h"

#include "util/Util.h"
#include "render/Shader.h"
#include "render/Texture.h"
#include "render/ObjectMesh.h"

ResourcesManager::~ResourcesManager()
{
	Cleanup();
}

void ResourcesManager::Cleanup() noexcept
{
	// Remove all ressources -> call respective destructor
	Shaders.clear();
	Textures.clear();
}

Shader* ResourcesManager::GetShader(const wchar_t* ShaderName)
{
	const std::string Name = wchar2str(ShaderName);
	auto PShader = Shaders[Name].get();

	if (!PShader) {
		PShader = new Shader{ ShaderName };
		Shaders[Name].reset(PShader);
	}

	return PShader;
}

PM3D::CTexture* ResourcesManager::GetTexture(const std::wstring& TextureName)
{
	const std::string Name = wstr2str(TextureName);
	auto PTexture = Textures[Name].get();

	if (!PTexture) {
		PTexture = new PM3D::CTexture{ TextureName };
		Textures[Name].reset(PTexture);
	}

	return PTexture;
}

ObjectMesh* ResourcesManager::GetMesh(const wchar_t* MeshName)
{
	const std::string Name = wchar2str(MeshName);
	auto PMesh = Meshes[Name].get();

	if (!PMesh) {
		PMesh = new ObjectMesh{ MeshName };
		Meshes[Name].reset(PMesh);
	}

	return PMesh;
}

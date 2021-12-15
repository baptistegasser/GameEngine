#include "stdafx.h"
#include "ResourcesManager.h"

#include "util/Util.h"

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

Texture* ResourcesManager::GetTexture(const std::wstring& TextureName)
{
	const std::string Name = wstr2str(TextureName);
	auto PTexture = Textures[Name].get();

	if (!PTexture) {
		PTexture = new Texture{ TextureName };
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

Effect* ResourcesManager::GetEffect(const wchar_t* EffectName)
{
	const std::string Name = wchar2str(EffectName);
	auto PEffect = Effects[Name].get();

	if (!PEffect) {
		PEffect = new Effect{ EffectName };
		Effects[Name].reset(PEffect);
	}

	return PEffect;
}

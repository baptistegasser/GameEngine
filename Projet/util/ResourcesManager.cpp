#include "stdafx.h"
#include "ResourcesManager.h"

#include "render/TextRenderer.h"
#include "util/Util.h"

ResourcesManager::~ResourcesManager()
{
	Cleanup();
}

void ResourcesManager::Cleanup() noexcept
{
	// Remove all ressources -> call respective destructor
	Shaders.clear();
	ShadersTerrain.clear();
	Effects.clear();
	Textures.clear();
	Sprites.clear();
	Meshes.clear();
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

ShaderSprite* ResourcesManager::GetShaderSprite(const wchar_t* ShaderName)
{
	const std::string Name = wchar2str(ShaderName);
	auto PShader = Sprites[Name].get();

	if (!PShader) {
		PShader = new ShaderSprite{ ShaderName };
		Sprites[Name].reset(PShader);
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

ShaderTerrain* ResourcesManager::GetShaderTerrain(const wchar_t* MeshName)
{
	const std::string Name = wchar2str(MeshName);
	auto PShaderTerrain = ShadersTerrain[Name].get();

	if (!PShaderTerrain) {
		PShaderTerrain = new ShaderTerrain{ MeshName };
		ShadersTerrain[Name].reset(PShaderTerrain);
	}

	return PShaderTerrain;
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

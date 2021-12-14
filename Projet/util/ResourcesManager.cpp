#include "stdafx.h"
#include "ResourcesManager.h"

#include "render/TextRenderer.h"
#include "util/Util.h"

ULONG_PTR ResourcesManager::GDIToken = 0;

ResourcesManager::ResourcesManager()
{
	const Gdiplus::GdiplusStartupInput  startupInput(nullptr, TRUE, TRUE);
	Gdiplus::GdiplusStartupOutput startupOutput{};

	GdiplusStartup(&GDIToken, &startupInput, &startupOutput);
}

ResourcesManager::~ResourcesManager()
{
	Cleanup();
}

void ResourcesManager::Cleanup() noexcept
{
	// Remove all ressources -> call respective destructor
	Shaders.clear();
	Textures.clear();
	Sprites.clear();
	Gdiplus::GdiplusShutdown(GDIToken);
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

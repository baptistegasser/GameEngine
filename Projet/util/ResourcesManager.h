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
#include "render/Effect.h"

class ResourcesManager {
private:
	std::map<const std::string, std::unique_ptr<Shader>> Shaders;
	std::map<const std::string, std::unique_ptr<Effect>> Effects;
	std::map<const std::string, std::unique_ptr<Texture>> Textures;
	std::map<const std::string, std::unique_ptr<ObjectMesh>> Meshes;

public:
	~ResourcesManager();
	void Cleanup() noexcept;

	Shader* GetShader(const wchar_t* ShaderName);
	template<typename ... EffectName>
	Shader* GetShaderWithEffects(const wchar_t* ShaderName, EffectName &...EffectsNames);
	Texture* GetTexture(const std::wstring& TextureName);
	ObjectMesh* GetMesh(const wchar_t* MeshName);

private:
	Effect* GetEffect(const wchar_t* EffectName);
};

template<typename ...EffectName>
inline Shader* ResourcesManager::GetShaderWithEffects(const wchar_t* ShaderName, EffectName &...EffectsNames)
{

	std::vector<Effect*> ShaderEffects;

	for (const auto& Name : { EffectsNames... }) {
		ShaderEffects.push_back(GetEffect(Name));
	}

	const std::string Name = wchar2str(ShaderName);
	auto PShader = Shaders[Name].get();

	if (!PShader) {
		PShader = new Shader{ ShaderName, ShaderEffects };
		Shaders[Name].reset(PShader);
	}

	return PShader;
}

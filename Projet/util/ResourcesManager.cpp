#include "stdafx.h"
#include "ResourcesManager.h"

#include "util/Util.h"
#include "render/Shader.h"

ResourcesManager::~ResourcesManager()
{
	CleanShaders();
}

void ResourcesManager::CleanShaders() noexcept
{
	for (const auto& Entry : Resources) {
		if (Entry.second.Is(ResourceType::Shader)) {
			delete Entry.second.Value;
		}
	}
}

Shader* ResourcesManager::GetShader(const wchar_t* ShaderName)
{
	const std::string ResourceName = whchar2str(ShaderName);
	auto& Entry = Resources[ResourceName];

	switch (Entry.Type) {
	case ResourceType::None:
	{
		auto* NewShader = new Shader{ ShaderName };
		Entry.Type = ResourceType::Shader;
		Entry.Value = NewShader;
		return NewShader;
	}
	case ResourceType::Shader:
		return Entry.GetValueAs<Shader>();
	default:
		throw std::logic_error("The name '"+ ResourceName +"' is not associated with a shader.");
	}
}

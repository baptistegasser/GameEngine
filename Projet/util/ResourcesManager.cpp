#include "stdafx.h"
#include "ResourcesManager.h"

#include "util/Util.h"
#include "render/Shader.h"
#include "render/Texture.h"

ResourcesManager::~ResourcesManager()
{
	// Remove all ressources, auto call destructor
	Resources.clear();
}

Shader* ResourcesManager::GetShader(const wchar_t* ShaderName)
{
	const std::string ResourceName = wchar2str(ShaderName);
	auto& Entry = Resources[ResourceName];

	switch (Entry.Type) {
	case ResourceType::None:
	{
		auto NewShader = new Shader{ ShaderName };
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

PM3D::CTexture* ResourcesManager::GetTexture(const std::wstring& TextureName)
{
	const std::string ResourceName = wstr2str(TextureName);
	auto& Entry = Resources[ResourceName];

	switch (Entry.Type) {
	case ResourceType::None:
	{
		auto NewTexture= new PM3D::CTexture{ TextureName };
		Entry.Type = ResourceType::Texture;
		Entry.Value = NewTexture;
		return NewTexture;
	}
	case ResourceType::Texture:
		return Entry.GetValueAs<PM3D::CTexture>();
	default:
		throw std::logic_error("The name '" + ResourceName + "' is not associated with a texture.");
	}
}

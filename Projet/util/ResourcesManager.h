#pragma once

#include "Singleton.h"

#include <map>
#include <stdexcept>
#include <string>

// Don't include manadged ressources in case they might need the
// ResourcesManager which would lead to circulard dependancies
struct Shader;
namespace PM3D {
	class CTexture;
}

class ResourcesManager : public Singleton<ResourcesManager> {
private:
	enum class ResourceType {
		None,
		Shader,
		Texture
	};

	struct ResourceEntry {
		ResourceType Type = ResourceType::None;
		void* Value = nullptr;

		template <class T>
		T* GetValueAs() noexcept
		{
			return static_cast<T*>(Value);
		}

		~ResourceEntry()
		{
			if (Type != ResourceType::None) {
				delete Value;
			}
		}
	};

	std::map<std::string, ResourceEntry> Resources;

public:
	~ResourcesManager();

	Shader* GetShader(const wchar_t* ShaderName);
	PM3D::CTexture* GetTexture(const std::wstring& TextureName);
};

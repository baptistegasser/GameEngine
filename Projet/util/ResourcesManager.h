#pragma once

#include "Singleton.h"

#include <map>
#include <stdexcept>
#include <string>

// Don't include manadged ressources in case they might need the
// ResourcesManager which would lead to circulard dependancies
struct Shader;

class ResourcesManager : public Singleton<ResourcesManager> {
private:
	enum class ResourceType {
		None,
		Shader
	};
	struct ResourceEntry {
		ResourceType Type = ResourceType::None;
		void* Value = nullptr;

		inline bool Is(const ResourceType& Type) const noexcept
		{
			return this->Type == Type;
		}

		template <class T>
		T* GetValueAs() noexcept
		{
			return static_cast<T*>(Value);
		}
	};

	std::map<std::string, ResourceEntry> Resources;

public:
	~ResourcesManager();
	void CleanShaders() noexcept;

	Shader* GetShader(const wchar_t* ShaderName);
};

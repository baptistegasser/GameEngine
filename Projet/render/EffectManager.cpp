#include "stdafx.h"
#include "EffectManager.h"

#include "EngineD3D11.h"

EffectManager::~EffectManager() noexcept
{
	Cleanup();
}

void EffectManager::Cleanup() noexcept
{
	Effects.clear();
}

bool EffectManager::HasEffectActivated() const noexcept
{
	for (auto& pair : Effects) {
		if (pair.second.Activated)
			return true;
	}

	return false;
}

Effect* EffectManager::GetActivatedEffect() noexcept
{
	for (auto& pair : Effects) {
		if (pair.second.Activated)
			return pair.second.Effect;
	}

	return nullptr;
}

void EffectManager::AddEffect(const wchar_t* Name)
{
	auto& ResourcesManager = EngineD3D11::GetInstance().ResourcesManager;
	AddEffect(ResourcesManager.GetEffect(Name));
}

void EffectManager::AddEffect(Effect* Effect)
{
	const std::string Name = wchar2str(Effect->FileName);

	if (HasEffect(Name)) {
		throw std::runtime_error{ "The effect " + Name + " is already managed."};
	}

	Effects[Name] = EffectNode{ Effect, false };
}

void EffectManager::ActivateEffect(const std::string& Name)
{
	// Assert has this effect
	if (!HasEffect(Name)) {
		throw std::runtime_error{ "Can't activate unmanaged effect " + Name + " !" };
	}

	// Activate if name match
	for (auto& pair : Effects) {
		pair.second.Activated = pair.first == Name;
	}
}

void EffectManager::DeactivateEffect()
{
	for (auto& pair : Effects) {
		pair.second.Activated = false;
	}
}

void EffectManager::ActivateRandomEffect()
{
	int Launch = De(prng);
	switch (Launch)
	{
	case 1 :
		ActivateEffect(wchar2str(L".\\shaders\\Effect_RadialBlur.fx"));
		break;
	case 2 :
		ActivateEffect(wchar2str(L".\\shaders\\Effect_Blur.fx"));
		break;
	case 3 :
		ActivateEffect(wchar2str(L".\\shaders\\Effect_Waves.fx"));
		break;
	case 4 :
		ActivateEffect(wchar2str(L".\\shaders\\Effect_Sharpen.fx"));
		break;
	default:
		break;
	}
}

bool EffectManager::HasEffect(const std::string& Name) const noexcept
{
	return Effects.find(Name) != Effects.end();
}

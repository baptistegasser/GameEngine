#include "stdafx.h"
#include "EffectManager.h"

#include "EngineD3D11.h"

bool EffectManager::HasOneEffectActivated() const noexcept
{
	for (auto& pair : Effects) {
		if (pair.second.Activated)
			return true;
	}

	return false;
}

std::vector<Effect*> EffectManager::GetActivatedEffects() noexcept
{
	std::vector<Effect*> v;
	for (auto& pair : Effects) {
		if (pair.second.Activated)
			v.push_back(pair.second.Effect);
	}

	return v;
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
		if (pair.first == Name)
			pair.second.Activated = true;
	}
}

bool EffectManager::HasEffect(const std::string& Name) const noexcept
{
	return Effects.find(Name) != Effects.end();
}

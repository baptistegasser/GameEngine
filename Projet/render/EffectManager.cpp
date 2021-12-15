#include "stdafx.h"
#include "EffectManager.h"

#include "MoteurWindows.h"

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
	auto& ResourcesManager = PM3D::CMoteurWindows::GetInstance().GetResourcesManager();
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

bool EffectManager::HasEffect(const std::string& Name) const noexcept
{
	return Effects.find(Name) != Effects.end();
}
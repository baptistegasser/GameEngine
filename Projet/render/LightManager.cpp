#include "stdafx.h"
#include "LightManager.h"

void LightManager::RegisterLight(LightComponent *Light)
{
	// Assert only one directional light
	if (Light->Type == Light::LightType::Directional) {
		if (HasDirectionalLight()) {
			throw std::runtime_error{ "Only one directional light is allowed at a time." };
		}

		DirectionalLight = Light;
		return;
	}

	this->Add(Light);
}

std::vector<Light> LightManager::GetLights() const noexcept
{
	// Get all lights by using the root bounding volume
	return GetLights(Root.Boundary);
}

std::vector<Light> LightManager::GetLights(const BoundingVolume& Boundary) const noexcept
{
	// Get lights
	const auto FoundLights = Find(Boundary);
	// Create output and reserve appropriate size
	std::vector<Light> Lights;
	Lights.reserve(FoundLights.size());

	// Convert component lights to their raw light data
	static const auto PtrToValue = [](const DataPtr& Ptr) { return Ptr->GetRawLight(); };
	std::transform(begin(FoundLights), end(FoundLights), std::back_inserter(Lights), PtrToValue);

	// Add directional if present
	if (HasDirectionalLight()) {
		Lights.push_back(PtrToValue(DirectionalLight));
	}

	return Lights;
}

DirectionalLight* LightManager::GetDirectionalLight() noexcept
{
	return static_cast<::DirectionalLight*>(DirectionalLight);
}

bool LightManager::HasDirectionalLight() const noexcept
{
	return DirectionalLight != nullptr;
}

#include "stdafx.h"
#include "LightManager.h"

void LightManager::RegisterLight(LightComponent *Light)
{
	// Assert only one directionnal light
	if (Light->Type == Light::LightType::Directionnal) {
		const auto it = std::find_if(
			begin(Datas),
			end(Datas),
			[](const DataType& Light) {
				return Light->Type == Light::LightType::Directionnal;
			}
		);
		if (it != Datas.end()) {
			throw std::runtime_error{ "Only one directionnal light is allowed at a time." };
		}
	}

	this->Add(std::move(DataType{ Light }));
}

std::size_t LightManager::GetLightCount() const noexcept
{
	return Datas.size();
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

	return Lights;
}

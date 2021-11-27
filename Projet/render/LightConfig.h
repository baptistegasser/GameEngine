#pragma once

#include "Light.h"

#include <vector>

/// <summary>
/// Store the lighting configuration for a scene.
/// </summary>
class LightConfig
{
	AmbientLight Ambient;
	DirectionalLight Directional;
	std::vector<PointLight> PointLights;
	/// <summary>
	/// Flag that indicate a change in light config which probably implicate
	///	to update the shader rendering lights.
	/// </summary>
	bool ConfigChanged = true;

public:
	LightConfig() = default;

	void SetAmbient(const AmbientLight& NewAmbient);
	void SetDirectional(const DirectionalLight& NewDirectional);
	void AddPointLight(const PointLight& Light);

	bool Changed() const noexcept;
	const AmbientLight& GetAmbient() const noexcept;
	const DirectionalLight& GetDirectional() const noexcept;
	const std::vector<PointLight>& GetPointLights() const noexcept;
};

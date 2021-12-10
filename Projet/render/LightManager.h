#pragma once

#include "math/Octree.h"
#include "math/Vec3f.h"
#include "Light.h"

/// <summary>
/// Functor to convert retrieve a position from a ptr to a Light.
/// </summary>
struct LightToPos
{
	Math::Vec3f operator()(const std::unique_ptr<LightComponent>& Light) const noexcept
	{
		return Light->Position;
	}
};

/// <summary>
/// Manager charged to keep track of lights.
/// </summary>
class LightManager : Octree<LightComponent, LightToPos>
{
public:
	using LightList = std::vector<Light>;

	LightManager(const BoundingBox& Boundary) : Octree{Boundary} {}

	Math::Vec3f AmbientColor{ 0.5f };

	void RegisterLight(LightComponent *Light);
	std::size_t GetLightCount() const noexcept;
	/// <summary>
	/// Get all managed lights.
	/// </summary>
	LightList GetLights() const noexcept;
	/// <summary>
	/// Get all managed lights that are inside a bounding volume.
	/// </summary>
	LightList GetLights(const BoundingVolume& Boundary) const noexcept;
};

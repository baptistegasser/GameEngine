#pragma once

#include "math/Octree.h"
#include "math/Vec3f.h"
#include "Light.h"
#include "DirectionalLight.h"

/// <summary>
/// Functor to convert retrieve a position from a ptr to a Light.
/// </summary>
struct LightToPos
{
	Math::Vec3f operator()(LightComponent* Light) const noexcept
	{
		return Light->Position;
	}
};

/// <summary>
/// Manager charged to keep track of lights.
/// </summary>
class LightManager : Octree <LightComponent, LightToPos, false>
{
public:
	using LightList = std::vector<Light>;

	/// <summary>
	/// The base lighting color for the scene, use it
	/// to add a default light level + ambiance style.
	/// </summary>
	Math::Vec3f AmbientColor{ 0.5f };

	/// <param name="Boundary">
	/// The bounding limit defining if an object belong in this manager.
	/// Should idealy match the scene boundary.
	/// </param>
	LightManager(const BoundingBox& Boundary) : Octree{Boundary} {}

	/// <summary>
	/// Register a light component to be tracked by this manager.
	/// </summary>
	void RegisterLight(LightComponent *Light);
	/// <summary>
	/// Get all managed lights.
	/// </summary>
	LightList GetLights() const noexcept;
	/// <summary>
	/// Get all managed lights that are inside a bounding volume.
	/// </summary>
	LightList GetLights(const BoundingVolume& Boundary) const noexcept;
	/// <summary>
	/// Return the pointer to the current directional light.
	/// </summary>
	/// <returns></returns>
	DirectionalLight* GetDirectionalLight() noexcept;

private:
	/// <summary>
	/// Store the directional has there can be only one.
	/// </summary>
	LightComponent* DirectionalLight{ nullptr };

	/// <returns>True if a directional light was registered.</returns>
	bool HasDirectionalLight() const noexcept;
};

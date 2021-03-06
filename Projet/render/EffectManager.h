#pragma once

#include "Effect.h"
#include "unordered_map"
#include <random>

/// <summary>
/// Manager used to store and handle states of post effect inside the engine.
/// </summary>
class EffectManager
{
public:
	EffectManager() = default;
	/// Default destructor as we are not the one truly managing the effects live,
	/// this role is still done by the \ref ResourcesManager
	~EffectManager() noexcept;

	void Cleanup() noexcept;

	/// <summary>
	/// Return true if one of the managed effect is activated.
	/// </summary>
	/// <returns></returns>
	bool HasEffectActivated() const noexcept;
	/// <summary>
	/// Return the currently activated effect.
	/// \warning \ref HasEffectActivated should be called before to assert there is an active effect.
	/// </summary>
	/// <returns></returns>
	Effect* GetActivatedEffect() noexcept;

	/// <summary>
	/// Load an effect by name and add it to the manager.
	/// </summary>
	void AddEffect(const wchar_t* Name);
	/// <summary>
	/// Add a loaded effect to the manager.
	/// </summary>
	void AddEffect(Effect* Effect);

	/// <summary>
	/// Actiavte an effect identified by its name.
	/// </summary>
	void ActivateEffect(const std::string &Name);

	/// <summary>
	/// Activate an effect identified by its name.
	/// </summary>
	void DeactivateEffect();

	/// <summary>
	/// Activate a random effect without the nul effect
	/// </summary>
	void ActivateRandomEffect();

private:
	/// <summary>
	/// Structure to store the effects status
	/// </summary>
	struct EffectNode {
		Effect* Effect;
		bool Activated;
	};
	std::unordered_map<std::string, EffectNode> Effects;

	/// <summary>
	/// Return true if a managed effect match this name.
	/// </summary>
	bool HasEffect(const std::string& Name) const noexcept;


	std::random_device rd;
	std::mt19937 prng{ rd() };
	std::uniform_int_distribution<int> De{ 1, 4 };

};
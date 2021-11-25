#pragma once

/// <summary>
/// Use ForceMode to specify how to apply a force using \ref Rigidbody::AddForce.
/// </summary>
enum class ForceMode
{
	/// <summary>
	/// Add a continuous force taking mass in account.
	/// </summary>
	Force,
	/// <summary>
	/// Add an instant force taking mass in account.
	/// </summary>
	Impulse,
	/// <summary>
	/// Add a continuous acceleration, ignoring mass.
	/// </summary>
	Acceleration,
	/// <summary>
	/// Add an instant velocity change, ignoring mass.
	/// </summary>
	VelocityChange,
};

/// <summary>
/// Convert a force mode to the matching Physx mode.
/// </summary>
inline physx::PxForceMode::Enum PhysxForce(const ForceMode& Mode) noexcept
{
	using namespace physx;

	switch (Mode) {
		case ForceMode::Force: return PxForceMode::eFORCE;
		case ForceMode::Impulse: return PxForceMode::eIMPULSE;
		case ForceMode::Acceleration: return PxForceMode::eACCELERATION;
		case ForceMode::VelocityChange: return PxForceMode::eVELOCITY_CHANGE;
	}
}

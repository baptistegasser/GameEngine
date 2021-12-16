#pragma once

#include "util/Singleton.h"

#include <unordered_map>

class InputManager : public Singleton<InputManager>
{
public:
	using KeyCode = unsigned int;
	using KeyStatus = unsigned int;

	/// <summary>
	/// Initialize the manager.
	/// </summary>
	/// <param name="hInstance">The app instance</param>
	/// <param name="hWnd">The window handle</param>
	void Init(HINSTANCE hInstance, HWND hWnd);

	/// <summary>
	/// Update the devices depending if the windows gain or loss focus.
	/// </summary>
	void HandleFocusChange(bool Focused) noexcept;

	/// <summary>
	/// Update the input devices, should be called once per frame only.
	/// </summary>
	void Tick() noexcept;

	/// <summary>
	/// Use to cleanup ressources.
	/// </summary>
	void Release() noexcept;

	/// <summary>
	/// Retrieve the state of the mouse.
	/// </summary>
	/// <returns></returns>
	const DIMOUSESTATE& GetMouseState() const noexcept;

	/// <summary>
	/// Return true during the first frame where the key is pressed.
	/// </summary>
	bool IsKeyDown(const KeyCode& Key) const noexcept;
	/// <summary>
	/// Return true while the key is pressed.
	/// </summary>
	bool IsKeyPressed(const KeyCode& Key) const noexcept;
	/// <summary>
	/// Return true during the first frame where the key is released.
	/// </summary>
	bool IsKeyUp(const KeyCode& Key) const noexcept;

private:
	// Singleton config
	friend struct Singleton<InputManager>;
	InputManager() = default;
	~InputManager();

	/// <summary>
	/// Check to prevent multiple init.
	/// </summary>
	bool IsInit = false;

	/// <summary>
	/// Direct Input devices manipulator
	/// </summary>
	IDirectInput8* PDirectInput;
	/// <summary>
	/// Input devices supported.
	/// </summary>
	IDirectInputDevice8* PKeyboardDevice, * PMouseDevice;

	DIMOUSESTATE MouseState;
	const static int KeyCount;
	/// <summary>
	/// A buffer to poll the keyboard status.
	/// </summary>
	char* KeyboardBuffer;

	/// <summary>
	/// Flags for a key status.
	/// </summary>
	enum KeyFlag {
		None    = 1 << 0,
		Down    = 1 << 1,
		Pressed = 1 << 2,
		Up		= 1 << 3
	};

	/// <summary>
	/// Store the current status of a keyboard's keys.
	/// </summary>
	std::unordered_map<KeyCode, KeyStatus> KeyboardStatus;

	/// <summary>
	/// Acquire all device for the app to use.
	/// </summary>
	void AcquireDevices() noexcept;
	/// <summary>
	/// Unacquire all device for another app to use.
	/// </summary>
	void UnacquireDevices() noexcept;
};

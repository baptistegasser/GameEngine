#include "stdafx.h"
#include "InputManager.h"

#include "util/Util.h"
#include "resources/resource.h"

#ifdef _DEBUG
#define ASSERT_INIT assert("The InputManager must have been initialized." && IsInit);
#else
#define ASSERT_INIT
#endif

const int InputManager::KeyCount = 256;

void InputManager::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (IsInit) return;
	
	DX_TRY(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&PDirectInput, nullptr), ERREUR_CREATION_DIRECTINPUT);

	KeyboardBuffer = new char[KeyCount];
	DX_TRY(PDirectInput->CreateDevice(GUID_SysKeyboard, &PKeyboardDevice, nullptr), ERREUR_CREATION_CLAVIER);
	DX_TRY(PKeyboardDevice->SetDataFormat(&c_dfDIKeyboard), ERREUR_CREATION_FORMAT_CLAVIER);
	PKeyboardDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DX_TRY(PDirectInput->CreateDevice(GUID_SysMouse, &PMouseDevice, nullptr), ERREUR_CREATION_SOURIS);
	DX_TRY(PMouseDevice->SetDataFormat(&c_dfDIMouse), ERREUR_CREATION_FORMAT_SOURIS);
	PMouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	AcquireDevices();

	// Init keys
	for (KeyCode Key = 0; Key < KeyCount; ++Key) {
		KeyboardStatus[Key] = KeyFlag::None;
	}

	IsInit = true;
}

void InputManager::HandleFocusChange(bool Focused) noexcept
{
	ASSERT_INIT;

	Focused ? AcquireDevices() : UnacquireDevices();
}

void InputManager::Tick() noexcept
{
	ASSERT_INIT;

	HRESULT RK = PKeyboardDevice->GetDeviceState(KeyCount, (void*)KeyboardBuffer);
	HRESULT RM = PMouseDevice->GetDeviceState(sizeof(MouseState), (void*)&MouseState);

	for (KeyCode Key = 0; Key < KeyCount; ++Key) {
		KeyStatus& Status = KeyboardStatus[Key];

		if (KeyboardBuffer[Key] & 0x80) {
			// First frame this key is pressed
			if (Status == KeyFlag::None) {
				Status = KeyFlag::Down | KeyFlag::Pressed;
			}
			// Seconde frame this key is pressed
			else {
				Status = KeyFlag::Pressed;
			}
		}
		else {
			// First frame this key is released
			if (Status != KeyFlag::None) {
				Status = KeyFlag::Up;
			}
			// Seconde frame this key is pressed
			else {
				Status = KeyFlag::None;
			}
		}
	}
}

void InputManager::Release() noexcept
{
	delete[] KeyboardBuffer;
	DX_RELEASE_INPUT(PKeyboardDevice);
	DX_RELEASE_INPUT(PMouseDevice);
	DX_RELEASE(PDirectInput);
}

const DIMOUSESTATE& InputManager::GetMouseState() const noexcept
{
	return MouseState;
}

bool InputManager::IsKeyDown(const KeyCode& Key) const noexcept
{
	return (KeyboardStatus.at(Key) & KeyFlag::Down) == KeyFlag::Down;
}

bool InputManager::IsKeyPressed(const KeyCode& Key) const noexcept
{
	return (KeyboardStatus.at(Key) & KeyFlag::Pressed) == KeyFlag::Pressed;
}

bool InputManager::IsKeyUp(const KeyCode& Key) const noexcept
{
	return (KeyboardStatus.at(Key) & KeyFlag::Up) == KeyFlag::Up;
}

InputManager::~InputManager()
{
	Release();
}

void InputManager::AcquireDevices() noexcept
{
	PKeyboardDevice->Acquire();
	PMouseDevice->Acquire();
}

void InputManager::UnacquireDevices() noexcept
{
	PKeyboardDevice->Unacquire();
	PMouseDevice->Unacquire();
}

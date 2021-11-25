#pragma once

namespace PM3D
{

	class CDIManipulateur
	{
	public:
		CDIManipulateur();
		~CDIManipulateur();

		bool Init(HINSTANCE hInstance, HWND hWnd);
		void StatutClavier();
		bool ToucheAppuyee(UINT touche) const;
		void SaisirEtatSouris();
		void HandleFocusChange(bool Focused, HWND hWnd);

		const DIMOUSESTATE& EtatSouris() const { return mouseState; }

	private:
		IDirectInput8* pDirectInput;
		IDirectInputDevice8* pClavier;
		IDirectInputDevice8* pSouris;
		IDirectInputDevice8* pJoystick;

		static bool bDejaInit;

		char tamponClavier[256];
		DIMOUSESTATE mouseState;

		void AcquireAll(HWND hWnd);
		void UnAcquireAll();
	};

} // namespace PM3D
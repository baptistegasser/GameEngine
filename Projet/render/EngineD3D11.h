#pragma once

#include "Engine.h"
#include "DeviceD3D11.h"
#include "util/Horloge.h"

#define MAX_LOADSTRING 100

class EngineD3D11 final : public Engine<EngineD3D11, DeviceD3D11>
{
public:
	void SetWindowsAppInstance(HINSTANCE hInstance);

private:
	ATOM MyRegisterClass(HINSTANCE hInstance) const;
	bool InitAppInstance();
	int Show() const;

	virtual int InitSpecific() override;
	virtual void TickSpecific() override;
	[[nodiscard]] virtual int64_t GetTimeSpecific() const override;
	[[nodiscard]] virtual double GetTimeIntervalsInSec(int64_t start, int64_t stop) const override;
	virtual std::unique_ptr<DeviceD3D11> CreateDeviceSpecific(const CDS_MODE cdsMode) override;
	virtual void BeginRenderSceneSpecific() override;
	virtual void EndRenderSceneSpecific() override;

	/// <summary>
	/// Treats main windows messages
	/// </summary>
	static LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

	/// <summary>
	/// Treats about dialog messages
	/// </summary>
	static INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

	HACCEL AccelTableHandle;
	static HINSTANCE AppInstanceHandle;
	HWND MainWindowHandle;
	TCHAR WindowClassName[MAX_LOADSTRING];

	PM3D::Horloge Horloge;
	static bool Focused, PreviousFocused;
};

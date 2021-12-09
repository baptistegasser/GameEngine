#pragma once 

/// <summary>
/// DirectX interface to GPU
/// </summary>
struct DeviceInfoD3D11 {
	enum INFO_TYPE {
		CURRENT_DEVICE
	};

	/// <summary>
	/// To obtain information from adapter number
	/// </summary>
	explicit DeviceInfoD3D11(int AdapterN);
	DeviceInfoD3D11(DXGI_MODE_DESC ModeDesc);

	bool Valid = false;
	int ScreenWidth, ScreenHeight, Memory;
	wchar_t GPUName[100]{};
	DXGI_MODE_DESC Mode{};
};

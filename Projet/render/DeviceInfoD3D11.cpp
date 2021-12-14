#include "stdafx.h"
#include "DeviceInfoD3D11.h" 
#include "util/Util.h"
#include <vector>

DeviceInfoD3D11::DeviceInfoD3D11(int AdapterN)
{
	IDXGIFactory* Factory = nullptr;
	IDXGIAdapter* Adapter = nullptr;
	IDXGIOutput* Output = nullptr;

	CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&Factory));

	if (FAILED(Factory->EnumAdapters(AdapterN, &Adapter))) return;
	if (FAILED(Adapter->EnumOutputs(0, &Output))) return;

	DXGI_OUTPUT_DESC OutDesc;
	Output->GetDesc(&OutDesc);
	ScreenWidth = OutDesc.DesktopCoordinates.right - OutDesc.DesktopCoordinates.left;
	ScreenHeight = OutDesc.DesktopCoordinates.bottom - OutDesc.DesktopCoordinates.top;
	Valid = true;

	DXGI_ADAPTER_DESC Desc;  Adapter->GetDesc(&Desc);
	Memory = static_cast<int>(Desc.DedicatedVideoMemory / 1024 / 1024);
	wcscpy_s(GPUName, 100, Desc.Description);

	DX_RELEASE(Output);
	DX_RELEASE(Adapter);
	DX_RELEASE(Factory);
}

DeviceInfoD3D11::DeviceInfoD3D11(DXGI_MODE_DESC ModeDesc) {
	IDXGIFactory* Factory;
	std::vector<IDXGIAdapter*> Adapters;

	CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&Factory));

	{
		IDXGIAdapter* Adapter;
		for (UINT i = 0; Factory->EnumAdapters(i, &Adapter) != DXGI_ERROR_NOT_FOUND; ++i) {
			Adapters.push_back(Adapter);
		}
	}

	// Main GPU index is 0
	*this = DeviceInfoD3D11(0);

	// Main monitor index is 0
	IDXGIOutput* Output = nullptr;
	Adapters[0]->EnumOutputs(0, &Output);

	Output->FindClosestMatchingMode(&ModeDesc, &Mode, nullptr);

	for ( auto Adapter : Adapters )
		DX_RELEASE(Adapter);

	DX_RELEASE(Output);
	DX_RELEASE(Factory);
}

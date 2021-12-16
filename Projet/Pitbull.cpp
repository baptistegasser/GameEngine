#include "stdafx.h"
#include "Pitbull.h"
#include "render/EngineD3D11.h"

using namespace PM3D;

int APIENTRY _tWinMain(
	HINSTANCE HInstance,
	HINSTANCE HPrevInstance,
	LPTSTR    LPCmdLine,
	int       NCMDShow)
{
	// Pour ne pas avoir d'avertissement
	UNREFERENCED_PARAMETER(HPrevInstance);
	UNREFERENCED_PARAMETER(LPCmdLine);
	UNREFERENCED_PARAMETER(NCMDShow);

	srand((unsigned int)time(NULL));

	try
	{
		EngineD3D11& Engine = EngineD3D11::GetInstance();
		Engine.SetWindowsAppInstance(HInstance);
		Engine.Init();
		Engine.Run();

		return (int)1;
	}

	catch (const std::exception& E)
	{
		const int BufferSize = 128;
		wchar_t Message[BufferSize];

		size_t CharacterConvertedCount;
		mbstowcs_s(&CharacterConvertedCount, Message, E.what(), BufferSize - 1);
		::MessageBox(nullptr, Message, L"Error", MB_ICONWARNING);

		return (int)99;
	}

	catch (int ErrorCode)
	{
		wchar_t ErrorMessage[MAX_LOADSTRING];

		::LoadString(HInstance, ErrorCode, ErrorMessage, MAX_LOADSTRING);
		::MessageBox(nullptr, ErrorMessage, L"Error", MB_ICONWARNING);

		return (int)99;
	}

}

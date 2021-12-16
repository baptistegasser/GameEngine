#include "stdafx.h"
#include "resources/resource.h"

#include "EngineD3D11.h"

bool EngineD3D11::Focused = false;
bool EngineD3D11::PreviousFocused = false;
HINSTANCE EngineD3D11::AppInstanceHandle;


void EngineD3D11::SetWindowsAppInstance(HINSTANCE hInstance)
{
	AppInstanceHandle = hInstance;
}

ATOM EngineD3D11::MyRegisterClass(HINSTANCE hInstance) const
{
	WNDCLASSEX WCEX;

	WCEX.cbSize = sizeof(WNDCLASSEX);

	WCEX.style = CS_HREDRAW | CS_VREDRAW;
	WCEX.lpfnWndProc = &WndProc;
	WCEX.cbClsExtra = 0;
	WCEX.cbWndExtra = 0;
	WCEX.hInstance = hInstance;
	WCEX.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PETITMOTEUR3D));
	WCEX.hCursor = LoadCursor(nullptr, IDC_ARROW);
	WCEX.hbrBackground = reinterpret_cast<HBRUSH>((COLOR_WINDOW + 1));
	WCEX.lpszMenuName = MAKEINTRESOURCE(IDC_PETITMOTEUR3D);
	WCEX.lpszClassName = WindowClassName;
	WCEX.hIconSm = LoadIcon(WCEX.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&WCEX);
}

bool EngineD3D11::InitAppInstance()
{
	TCHAR szTitle[MAX_LOADSTRING];
	 
	LoadString(AppInstanceHandle, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(AppInstanceHandle, IDC_PETITMOTEUR3D, WindowClassName, MAX_LOADSTRING);
	MyRegisterClass(AppInstanceHandle);

	MainWindowHandle = CreateWindow(WindowClassName, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, AppInstanceHandle, nullptr);

	if (!MainWindowHandle)
	{
		return false;
	}

	AccelTableHandle = LoadAccelerators(AppInstanceHandle, MAKEINTRESOURCE(IDC_PETITMOTEUR3D));

	return true;
}

int EngineD3D11::Show() const
{
	ShowWindow(MainWindowHandle, SW_SHOWNORMAL);
	UpdateWindow(MainWindowHandle);

	return 0;
}

int EngineD3D11::InitSpecific()
{
	InitAppInstance();
	Show();
	InputManager::GetInstance().Init(AppInstanceHandle, MainWindowHandle);
	return 0;
}

void EngineD3D11::TickSpecific()
{
	MSG msg;

	if (Focused != PreviousFocused) {
		InputManager::GetInstance().HandleFocusChange(Focused);
		PreviousFocused = Focused;
	}

	// Check for windows message
	while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) {
			Stop();
		}

		if (!::TranslateAccelerator(msg.hwnd, AccelTableHandle, &msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}

int64_t EngineD3D11::GetTimeSpecific() const
{
	return Horloge.GetTimeCount();
}

double EngineD3D11::GetTimeIntervalsInSec(int64_t start, int64_t stop) const
{
	return Horloge.GetTimeBetweenCounts(start, stop);
}

std::unique_ptr<DeviceD3D11> EngineD3D11::CreateDeviceSpecific(const CDS_MODE cdsMode)
{
	return std::make_unique<DeviceD3D11>(cdsMode, MainWindowHandle);
}

void EngineD3D11::BeginRenderSceneSpecific()
{
	Device->ClearView();
}

void EngineD3D11::EndRenderSceneSpecific()
{
}

LRESULT CALLBACK EngineD3D11::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// Analyse menu selection
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(AppInstanceHandle, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// Évitez d'ajouter du code ici...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KILLFOCUS:
		Focused = false;
		return 0;
	case WM_SETFOCUS:
		Focused = true;
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK EngineD3D11::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	default: 
		break;
	}
	return (INT_PTR)FALSE;
}

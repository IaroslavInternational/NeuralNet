#include "../../Include/Core/MainWindow.hpp"
#include "../../Include/Core/Resource.hpp"
#include "../../Include/Core/Graphics.hpp"
#include "../../Include/App.hpp"

#include "../../../libs/imgui/imgui.h"
#include "../../../libs/imgui/imgui_impl_win32.h"
#include "../../../libs/imgui/imgui_impl_dx11.h"

#include <assert.h>

MainWindow::MainWindow(HINSTANCE hInst, wchar_t* pArgs, int width, int height)
	:
	args(pArgs),
	hInst(hInst),
	ScreenWidth(width),
	ScreenHeight(height)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX),CS_CLASSDC,_HandleMsgSetup,0,0,
		hInst,nullptr,nullptr,nullptr,nullptr,
		wndClassName,nullptr };
	wc.hIconSm = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, 0);
	wc.hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 32, 32, 0);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	RegisterClassEx(&wc);

	RECT wr;
	wr.left = 350;
	wr.right = ScreenWidth + wr.left;
	wr.top = 100;
	wr.bottom = ScreenHeight + wr.top;
	AdjustWindowRect(&wr, WS_POPUP | WS_OVERLAPPED, FALSE);
	hWnd = CreateWindow(wndClassName, L"Neuro", WS_POPUP | WS_OVERLAPPED,
		wr.left, wr.top, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, hInst, this);

	if (hWnd == nullptr)
	{
		throw (L"Failed to get valid window handle.");
	}

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
}

MainWindow::~MainWindow()
{
	UnregisterClass(wndClassName, hInst);
}

bool MainWindow::IsActive() const
{
	return GetActiveWindow() == hWnd;
}

bool MainWindow::IsMinimized() const
{
	return IsIconic(hWnd) != 0;
}

void MainWindow::ShowMessageBox(const std::wstring& title, const std::wstring& message) const
{
	MessageBox(hWnd, message.c_str(), title.c_str(), MB_OK);
}

bool MainWindow::ProcessMessage()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
		{
			return false;
		}
	}
	return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI MainWindow::_HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		MainWindow* const pWnd = reinterpret_cast<MainWindow*>(pCreate->lpCreateParams);
		
		assert(pWnd != nullptr);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&MainWindow::_HandleMsgThunk));


		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI MainWindow::_HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MainWindow* const pWnd = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT MainWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		ScreenWidth = LOWORD(lParam);
		ScreenHeight = HIWORD(lParam);
		break;

		// ************ KEYBOARD MESSAGES ************ //
	case WM_KEYDOWN:
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) // no thank you on the autorepeat
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
		// ************ END KEYBOARD MESSAGES ************ //

		// ************ MOUSE MESSAGES ************ //
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (x > 0 && x < ScreenWidth && y > 0 && y < ScreenHeight)
		{
			mouse.OnMouseMove(x - int(ScreenWidth * 0.2f), y - 24);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				x = std::max(0, x);
				x = std::min(int(ScreenWidth) - 1, x);
				y = std::max(0, y);
				y = std::min(int(ScreenHeight) - 1, y);
				mouse.OnMouseMove(x - int(ScreenWidth * 0.2f), y - 24);
			}
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
				mouse.OnLeftReleased(x - int(ScreenWidth * 0.2f), y - 24);
				mouse.OnRightReleased(x - int(ScreenWidth * 0.2f), y - 24);
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftPressed(x - int(ScreenWidth * 0.2f), y - 24);

		if (x > 100 && x < ScreenWidth - 24 && y < 23)
		{
			ReleaseCapture();
			SendMessage(hWnd, WM_SYSCOMMAND, 0xF012, 0);
		}

		break;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightPressed(x - int(ScreenWidth * 0.2f), y - 24);
		break;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnLeftReleased(x - int(ScreenWidth * 0.2f), y - 24);
		break;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		mouse.OnRightReleased(x - int(ScreenWidth * 0.2f), y - 24);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			mouse.OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			mouse.OnWheelDown(x, y);
		}
		break;
	}

	// ************ END MOUSE MESSAGES ************ //
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int MainWindow::GetWidth() const
{
	return ScreenWidth;
}

int MainWindow::GetHeight() const
{
	return ScreenHeight;
}

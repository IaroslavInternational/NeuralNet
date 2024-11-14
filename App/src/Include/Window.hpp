#pragma once

#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/imgui_impl_win32.h"
#include "../../libs/imgui/imgui_impl_dx11.h"

#include <d3d11.h>
#include <tchar.h>
#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)

#pragma comment (lib, "d3d11.lib")
class Window
{
public:
	Window(int width, int height, LPCWSTR name);
	~Window();
public:
	void frame_start();
	void render();
	void end_start();
private:
	bool CreateDeviceD3D();
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();
private:
	int width;
	int height;
private:
	WNDCLASSEXW wc;
	HWND hwnd;
	UINT g_ResizeWidth;
	UINT g_ResizeHeight;
	Microsoft::WRL::ComPtr<ID3D11Device>			 g_pd3dDevice = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> g_pd3dDeviceContext = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> g_pSwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  g_mainRenderTargetView = nullptr;
	bool                     g_SwapChainOccluded = false;
};


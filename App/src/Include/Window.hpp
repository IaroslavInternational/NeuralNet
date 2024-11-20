#pragma once

#include <d3d11.h>
#include <tchar.h>

#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)

#pragma comment (lib, "d3d11.lib")

class Window
{
	friend class GUI;
public:
	Window();
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
	float clear_color[4];
public:
	WNDCLASSEXW wc;
	HWND hwnd;
	Microsoft::WRL::ComPtr<ID3D11Device> g_pd3dDevice = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> g_pd3dDeviceContext = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> g_pSwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> g_mainRenderTargetView = nullptr;
	bool g_SwapChainOccluded = false;
};


#pragma once

#include "AppWin.hpp"
#include "Graphics.hpp"
#include "../Device/Keyboard.hpp"
#include "../Device/Mouse.hpp"

#include <string>

class HWNDKey
{
	friend Graphics::Graphics(HWNDKey&, int, int);
	friend class UI;
public:
	HWNDKey(const HWNDKey&) = delete;
	HWNDKey& operator=(HWNDKey&) = delete;
protected:
	HWNDKey() = default;
protected:
	HWND hWnd = nullptr;
};

class MainWindow : public HWNDKey
{
	friend class UI;
public:
	MainWindow(HINSTANCE hInst, wchar_t* pArgs);
	MainWindow(const MainWindow&) = delete;
	MainWindow& operator=(const MainWindow&) = delete;
	~MainWindow();
public:
	bool IsActive() const;
	bool IsMinimized() const;
	void ShowMessageBox(const std::wstring& title, const std::wstring& message) const;
	void Kill()
	{
		PostQuitMessage(0);
	}
	bool ProcessMessage();
	const std::wstring& GetArgs() const
	{
		return args;
	}
	int GetWidth() const;
	int GetHeight() const;
private:
	static LRESULT WINAPI _HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI _HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Keyboard kbd;
	Mouse mouse;
private:
	static constexpr wchar_t* wndClassName = L"Neuro";
	HINSTANCE hInst = nullptr;
	std::wstring args;
	int ScreenWidth;
	int ScreenHeight;
};
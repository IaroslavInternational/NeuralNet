#pragma once

#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Graphics.hpp"
#include "FrameTimer.hpp"

#include <random>

class App
{
public:
	App(class MainWindow& wnd);
	App(const App&) = delete;
	App& operator=(const App&) = delete;
public:
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
private:
	MainWindow& wnd;
	Graphics gfx;
	FrameTimer ft;
};
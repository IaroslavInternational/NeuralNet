#pragma once

#include "Device/Keyboard.hpp"
#include "Device/Mouse.hpp"
#include "Core/Graphics.hpp"
#include "Core/FrameTimer.hpp"
#include "Objects/ResourceManager.hpp"
#include "Objects/Object2D.hpp"

#include "UI.hpp"

class App
{
	friend class UI;
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
	UI ui;
	FrameTimer ft;
	float viewPortX;  // Отсутп от боковой панели
	float viewPortY;  // Высота меню
private:
	std::vector<std::future<void>> workers;
	ResourceManager rManager;
	std::vector<Object2D> objects;
};
/* ============================================================
* Заголовочный файл App.hpp
* Класс приложения App
* Функционал:
*	- Рендер объектов
* Атрибуты:
*	- Адрес окна
*	- Класс Graphics
*	- UI
============================================================ */

#pragma once

#include "Device/Keyboard.hpp"
#include "Device/Mouse.hpp"
#include "Core/Graphics.hpp"
#include "Core/FrameTimer.hpp"
#include "Objects/ResourceManager.hpp"
#include "Objects/Grid.hpp"
#include "Objects/DrawList.hpp"

#include "UI.hpp"

// Класс приложения
class App
{
	friend class UI;
public:
	App(class MainWindow& wnd);
	App(const App&) = delete;
	App& operator=(const App&) = delete;
public:
	void Go();  // Метод запуска приложения (в цикле)
private:
	void UpdateModel();	  // Обновление параметров объектов
	void ComposeFrame();  // Отрисовка объектов 
private:
	MainWindow& wnd;  // Адрес окна отрисовки
	Graphics gfx;	  // Графический API
	UI ui;			  // Интерфейс
	FrameTimer ft;	  // Таймер
private:
	std::vector<std::future<void>> workers;  // Потоки
	ResourceManager rManager;				 // Менеджер ресурсов
	DrawList dList;							 // Список объектов для отрисовки
	Grid grid;								 // Сетка
};
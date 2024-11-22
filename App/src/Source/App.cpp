/* ============================================================
* Файл App.cpp
* Реализация класса приложения App
* Методы:
*	- Go: отрисовка объектов и UI (главная ф-ия)
*	- UpdateModel: обновление параметров объектов
*	- ComposeFrame: Отрисовка объектов
============================================================ */

#include "../Include/Core/MainWindow.hpp"
#include "../Include/App.hpp"

App::App(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd, wnd.GetWidth(), wnd.GetHeight()),
	ui(this),
	dList(),
	grid(50, Colors::MakeRGB(25, 25, 25), wnd.GetWidth(), wnd.GetHeight())
{

}

void App::Go()
{
	gfx.BeginFrame();	
	
	const float dt = ft.Mark();
	ui.Render(dt);

	UpdateModel();
	ComposeFrame();

	gfx.EndFrame();
}

void App::UpdateModel()
{
	//const float dt = ft.Mark();

	if (wnd.kbd.KeyIsPressed('W'))
	{
		dList.Translate({0, 1});
	}

}

void App::ComposeFrame()
{
	grid.Draw(gfx);
	dList.Draw(gfx);
}

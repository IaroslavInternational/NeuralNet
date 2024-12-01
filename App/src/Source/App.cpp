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
	dList("data/Projects/Sample/prj.json", rManager, gfx),
	camera(0, 0, dList)
{

}

void App::Go()
{
	const float dt = ft.Mark();

	gfx.BeginFrame();	
	
	ui.Render();

	UpdateModel(dt);
	ComposeFrame(dt);

	gfx.EndFrame();
}

void App::UpdateModel(float dt)
{
	if (wnd.kbd.KeyIsPressed('W'))
	{
		camera.Translate(0, 1);
	}
	if (wnd.kbd.KeyIsPressed('A'))
	{
		camera.Translate(-1, 0);
	}
	if (wnd.kbd.KeyIsPressed('S'))
	{
		camera.Translate(0, -1);
	}
	if (wnd.kbd.KeyIsPressed('D'))
	{
		camera.Translate(1, 0);
	}

	ui.Update(dt);
}

void App::ComposeFrame(float dt)
{
	dList.Draw(gfx);
}

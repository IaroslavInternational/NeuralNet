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
	dList("data/Projects/Sample/prj.json", rManager),
	camera(0, 0, dList)
{

}

void App::Go()
{
	gfx.BeginFrame();	
	
	ui.Render();

	UpdateModel();
	ComposeFrame();

	gfx.EndFrame();
}

void App::UpdateModel()
{
	const float dt = ft.Mark();

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

	if (wnd.mouse.LeftIsPressed())
	{
		int x = wnd.mouse.GetPosX();
		int y = wnd.mouse.GetPosY();

		dList.CheckHover(x, y);
	}

	ui.Update();
}

void App::ComposeFrame()
{
	dList.Draw(gfx);
}

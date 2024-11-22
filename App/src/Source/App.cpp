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
	dList(wnd.GetWidth())
{
	r = 25;
	g = 25;
	b = 25;
	gridColor = Colors::MakeRGB(r, g, b);
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

	gridColor = Colors::MakeRGB(r, g, b);
}

void App::ComposeFrame()
{
	// Grid
	/*for (size_t i = viewPortX; i < wnd.GetWidth(); i++)
	{
		for (size_t j = viewPortY + gridPadding; j < wnd.GetHeight(); j += gridPadding)
		{
			gfx.PutPixel(i, j, gridColor);
		}
	}

	for (size_t i = viewPortY; i < wnd.GetHeight(); i++)
	{
		for (size_t j = viewPortX + gridPadding; j < wnd.GetWidth(); j += gridPadding)
		{
			gfx.PutPixel(j, i, gridColor);
		}
	}

	for (auto& obj : objects)
	{
		obj.Draw(gfx);
	}*/

	dList.Draw(gfx);
}

#include "../Include/Core/MainWindow.hpp"
#include "../Include/App.hpp"

App::App(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd, wnd.GetWidth(), wnd.GetHeight()),
	rManager(),
	ui(this)
{
	viewPortX = 0.2f * (float)wnd.GetWidth(); 
	viewPortY = 25.0f;  

	for (size_t i = 1; i < 5; i++)
	{
		objects.push_back(Object2D(100, 100*i, rManager["item1.bmp"]));
	}

	for (auto& obj : objects)
	{
		obj.Translate(viewPortX, viewPortY);
	}
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
}

void App::ComposeFrame()
{
	for (auto& obj : objects)
	{
		obj.Draw(gfx);
	}
}

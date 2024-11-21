#include "../Include/MainWindow.hpp"
#include "../Include/App.hpp"

App::App(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd, wnd.GetWidth(), wnd.GetHeight()),
	tex("item.bmp", 50, 50, Color(255, 255, 255, 255))
{
}

void App::Go()
{
	gfx.BeginFrame();	
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
	tex.Draw(gfx);
}

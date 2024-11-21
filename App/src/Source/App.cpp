#include "../Include/Core/MainWindow.hpp"
#include "../Include/App.hpp"

App::App(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd, wnd.GetWidth(), wnd.GetHeight()),
	rManager(),
	ui(),
	obj(rManager["item1.bmp"])
{
}

void App::Go()
{
	gfx.BeginFrame();	
	
	ui.Render(&obj);

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
	obj.Draw(gfx);
}

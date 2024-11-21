#include "../Include/Core/MainWindow.hpp"
#include "../Include/App.hpp"

App::App(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd, wnd.GetWidth(), wnd.GetHeight()),
	rManager(),
	ui()
{
	rManager.Add(Texture("item1.bmp", 50, 50, Color(255, 255, 255, 255)));
	rManager.Add(Texture("item2.bmp", 250, 50, Color(255, 255, 255, 255)));

	auto& tex1 = rManager["item1.bmp"]; // test
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

}

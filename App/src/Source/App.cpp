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
	camera(0, 0, dList),
	ns("")
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
	// Если зажата ЛКМ И
	// Если курсор в окне И
	// Если курсор не покрывает панель И 
	// Если не нажат ctrl
	// Если не нажат shift
	// Если нажат Z
	if (wnd.mouse.LeftIsPressed() &&
		wnd.mouse.IsInWindow()    && 
		wnd.mouse.GetPosX() >= 0  &&
		wnd.mouse.GetPosY() >= 0  &&
		!wnd.kbd.KeyIsPressed(VK_CONTROL) &&
		!wnd.kbd.KeyIsPressed(VK_SHIFT)   &&
		wnd.kbd.KeyIsPressed('Z'))  // Временно
	{
		if (hTime > 0.065f && !onDrag)
		{
			initPos.x = wnd.mouse.GetPosX();
			initPos.y = wnd.mouse.GetPosY();
			
			onDrag = true;
		}
		else
		{
			hTime += dt;
		}
	}
	else
	{
		onDrag = false;
	}

	// Если отпустили ЛКМ
	if (wnd.mouse.LeftIsReleased())
	{
		onDrag = false;
		dt = 0.0f;
	}

	if (onDrag)
	{
		int cur_x = wnd.mouse.GetPosX();
		int cur_y = wnd.mouse.GetPosY();

		camera.Translate(cur_x - initPos.x, cur_y - initPos.y);

		initPos.x = cur_x;
		initPos.y = cur_y;
	}
	
	ui.Update(dt);
}

void App::ComposeFrame(float dt)
{
	dList.Draw(gfx);
}

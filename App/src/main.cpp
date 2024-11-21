#include "Include/Core/MainWindow.hpp"
#include "Include/App.hpp"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	MainWindow wnd(hInst, pArgs, 1300, 800);
	App app(wnd);

	while (wnd.ProcessMessage())
	{
		app.Go();
	}

	return 0;
}
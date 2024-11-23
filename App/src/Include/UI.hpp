#pragma once

class App;

class UI
{
public:
	UI(App* app);
public:
	void Render();
private:
	void SetPanelSizeAndPosition(int corner, float width, float height, float x_offset, float y_offset);
	void ShowMenu();
	void ShowPanel();
	void ShowViewPort();
private:
	App* pApp;
	float appScale = 1.0f;
};


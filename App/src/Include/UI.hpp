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
private:
	App* pApp;
};


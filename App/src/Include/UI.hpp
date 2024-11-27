#pragma once

#include <vector>

#include "Core/var.hpp"

#include <string>

class App;
class Cell;

struct ContextMenu
{
	pos2d pos;
	bool flag_pressed = false;
	bool flag_released = false;
	float counter = 0.0f;
	Cell* first_ptr = nullptr;
	Cell* second_ptr = nullptr;
};

class UI
{
public:
	UI(App* app);
public:
	void Update(float dt);
	void Render();
private:
	void SetPanelSizeAndPosition(int corner, float width, float height, float x_offset, float y_offset);
	void ShowMenu();
	void ShowPanel();
	void ShowViewPort();
	void ShowTopPanel();
	F_DEBUG(void Debug());
private:
	App* pApp;
	float appScale = 1.0f;
	char buffer[6];
	ContextMenu cMenu;
};


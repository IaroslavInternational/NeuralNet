#pragma once

#include <vector>

#include "Core/var.hpp"

class App;

struct ContextMenu
{
	bool show = false;
	pos2d pos;
};

class UI
{
public:
	UI(App* app);
public:
	void Update();
	void Render();
private:
	void SetPanelSizeAndPosition(int corner, float width, float height, float x_offset, float y_offset);
	void ShowMenu();
	void ShowPanel();
	void ShowViewPort();
	F_DEBUG(void Debug());
private:
	App* pApp;
	float appScale = 1.0f;
	ContextMenu cMenu;
	bool Shit = true;
	F_DEBUG(std::vector<pos2d> inputs);
};


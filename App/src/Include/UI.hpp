#pragma once

#include <vector>

#include "Core/var.hpp"

#include <string>
#include <memory>

class App;
class Cell;
class DrawLayer;

// Структура работы с контекстными действиями
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
	App* pApp;							// Указатель на приложение
	float appScale = 1.0f;				// Множитель ViewPort			
	ContextMenu cMenu;					// Структура работы с контекстными действиями
	DrawLayer* pLayer = nullptr;		// Указатель на тек. слой
	std::vector<bool> selected_layers;  // Выбранные слои
	bool isDeleteLayer = false;
};


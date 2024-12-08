#pragma once

#include "Core/var.hpp"

#include <string>
#include <future>
#include <vector>

class App;
class Cell;
class DrawLayer;
enum class LayerType;

// Структура работы с контекстными действиями
struct ContextMenu
{
	pos2d pos;
	bool flag_pressed = false;
	bool flag_released = false;
	bool ctrl_pressed = false;
	bool shift_pressed = false;
	bool a_pressed = false;
	bool d_pressed = false;
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
	void ShowPanel();
	void ShowViewPort();
	void ShowTopPanel();
	F_DEBUG(void Debug());
private:
	bool CheckExLayer(LayerType type);
	void AddNeuron();
	void DeleteNeuron();
	void SpawnThread(void (UI::*ptr)());
	void FindLayer();
private:
	App* pApp;							// Указатель на приложение
	float appScale = 1.0f;				// Множитель ViewPort			
	ContextMenu cMenu;					// Структура работы с контекстными действиями
	DrawLayer* pLayer = nullptr;		// Указатель на тек. слой
	DrawLayer* dragLayer = nullptr;		// Указатель на тек. слой для Drag
	std::vector<bool> selected_layers;  // Выбранные слои
	bool isDeleteLayer = false;
	bool isAddLayer = false;
	bool invalid_cell = false;
	bool isAddNeuron = false;
	bool isDeleteNeuron = false;
private:
	std::future<void> worker;
};


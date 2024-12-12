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
	bool h_pressed = false;
	bool g_pressed = false;

	float counter = 0.0f;
	Cell* first_ptr = nullptr;
	Cell* second_ptr = nullptr;
};

// Класс, определящий признак переименования объектов
enum class RenameState
{
	Down,
	Up
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
	void AddNeuron();									// Добавить нейрон  в слое pLayer
	void DeleteNeuron();								// Удалить нейрон в слое pLayer
	void AddHiddenLayer();								// Добавить скрытый слой
	void SpawnThread(void (UI::*ptr)());				// Выполнить ф-ию в отдельном потоке
	void FindLayer();									// Найти слой по ячейке
	void ShiftLayer(DrawLayer* l, int offset);			// Сдвинуть слой на offset
	void RenameLayer(DrawLayer* l, RenameState state);  // Переименовать слой после добавления/удаления слоёв
	size_t GetHiddenLayersAmount() const;				// Посчитать кол-во скрытых слоёв
private:
	App* pApp;							// Указатель на приложение
	float appScale = 1.0f;				// Множитель ViewPort			
	ContextMenu cMenu;					// Структура работы с контекстными действиями
	DrawLayer* pLayer = nullptr;		// Указатель на тек. слой
	DrawLayer* dragLayer = nullptr;		// Указатель на тек. слой для Drag
	std::vector<bool> selected_layers;  // Выбранные слои

	bool isDeleteLayer = false;	        // Запрос на удаление слоя
	bool isAddLayer = false;            // Запрос на добавления слоя
	bool invalid_cell = false;          // Для проверки валидности перемещения слоёв (перекрытие)
	bool isAddNeuron = false;           // Запрос на добавление нейрона в слой
	bool isAddHiddenLayer = false;      // Запрос на добавление скрытого слоя
	bool isDeleteHiddenLayer = false;   // Запрос на удаление скрытого слоя
	bool isDeleteNeuron = false;        // Запрос на удаление нейрона в слой
	bool ShowLayerInfo = false;	        // Запрос на показ информации о слое

	bool posSet = false;  // Костыль - чтобы установить положение окна об информации о слое 1 раз
private:
	std::future<void> worker;
};


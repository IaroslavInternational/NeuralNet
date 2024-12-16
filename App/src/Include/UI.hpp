#pragma once

#include "Core/var.hpp"

#include <string>
#include <future>
#include <vector>

#include "Core/Graphics.hpp"

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
	bool CheckExLayer(LayerType type);					// Проверить существования слоя в dList

	void AddNeuron();									// Добавить нейрон  в слое pLayer
	void DeleteNeuron();								// Удалить нейрон в слое pLayer
	void AddHiddenLayer();								// Добавить скрытый слой
	void DeleteHiddenLayer();						    // Удалить скрытый слой

	void SpawnThread(void (UI::*ptr)());				// Выполнить ф-ию в отдельном потоке
	void FindLayer();									// Найти слой по ячейке
	void ShiftLayer(DrawLayer* l, int offset);			// Сдвинуть слой на offset
	void RenameLayer(DrawLayer* l, RenameState state);  // Переименовать слой после добавления/удаления слоёв
	size_t GetHiddenLayersAmount() const;				// Посчитать кол-во скрытых слоёв

	void KeyProc(unsigned char key, bool* ctx_state, bool* query);  // Функция обработки нажатия кнопки
#ifndef N_DEBUG
private:
	bool LoadTextureFromMemory(const void* data, size_t data_size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
	bool LoadTextureFromFile(const char* file_name, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
#endif
private:
	App* pApp;							// Указатель на приложение
	float appScale = 1.0f;				// Множитель ViewPort			
	ContextMenu cMenu;					// Структура работы с контекстными действиями
	DrawLayer* pLayer = nullptr;		// Указатель на тек. слой
	DrawLayer* dragLayer = nullptr;		// Указатель на тек. слой для Drag
	std::vector<bool> selected_layers;  // Выбранные слои

	bool isAddLayer = false;            // Запрос на добавления слоя
	bool isDeleteLayer = false;	        // Запрос на удаление слоя

	bool isAddNeuron = false;           // Запрос на добавление нейрона в слой
	bool isDeleteNeuron = false;        // Запрос на удаление нейрона в слой
	
	bool isAddHiddenLayer = false;      // Запрос на добавление скрытого слоя
	bool isDeleteHiddenLayer = false;   // Запрос на удаление скрытого слоя
	
	bool ShowLayerInfo = false;	        // Запрос на показ информации о слое
	bool invalid_cell = false;          // Для проверки валидности перемещения слоёв (перекрытие)

	bool posSet = false;  // Костыль - чтобы установить положение окна об информации о слое 1 раз
private:
	std::future<void> worker;
#ifndef N_DEBUG
	std::vector<ID3D11ShaderResourceView*> pTextures;
#endif
};


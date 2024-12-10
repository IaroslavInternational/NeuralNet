/* ============================================================
* Заголовочный файл DrawList.hpp
* Класс приложения DrawList
* Функционал:
*	- Хранение объектов для отрисовки
* Атрибуты:
*	- Список объектов
*	- Сетка
============================================================ */

#pragma once

#include "DrawLayer.hpp"

#include <unordered_map>

// Список объектов для отрисовки
class DrawList
{
	friend class UI;
public:
	DrawList(const std::string& path, ResourceManager& rManager, Graphics& gfx);
public:
	void Add(DrawLayer& dLayer);
	void Insert(DrawLayer& obj, size_t pos);
	void Delete(DrawLayer* dLayer);
	void Draw(Graphics& gfx);
	void Translate(int dx, int dy);
	void Translate(const pos2d& dpos);
public:
	void DrawSynapses(DrawLayer& l1, DrawLayer& l2, Color c, Graphics& gfx);
	void DrawSelectedLayer(DrawLayer* l, Graphics& gfx);
	void CheckHover(int x, int y);
	size_t GetIdByPtr(DrawLayer* l);
private:
	void DrawLine(pos2d& p1, pos2d& p2, Color c, Graphics& gfx, bool isBlended = false);
	void DrawSynapse(Object2D& obj1, Object2D& obj2, Color c, Graphics& gfx);
private:
	Grid				   grid;				   // Сетка
	std::vector<DrawLayer> dLayers;				   // Слои
	Cell*                  hoveredCell = nullptr;  // Подсвеченная ячейка
	DrawLayer*             selected = nullptr;     // Подсвеченный слой
	std::string            projectName;		       // Имя проекта
};
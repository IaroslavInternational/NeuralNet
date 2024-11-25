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

#include "Object2D.hpp"
#include "Grid.hpp"
#include "ResourceManager.hpp"

#include <unordered_map>

// Список объектов для отрисовки
class DrawList
{
	friend class UI;
public:
	DrawList(const std::string& path, ResourceManager& rManager);
public:
	void Add(Object2D& obj);
	void Draw(Graphics& gfx);
	void Translate(int dx, int dy);
	void Translate(const pos2d& dpos);
public:
	void CheckHover(int x, int y);
public:
	Object2D* operator[](const std::string& key)
	{
		return &dList.at(key);
	};
private:
	ResourceManager& rManager;
	std::unordered_map<std::string, Object2D> dList;  // Список объектов для отрисовки
	Grid grid;										  // Сетка
	Cell* hoveredCell = nullptr;
};
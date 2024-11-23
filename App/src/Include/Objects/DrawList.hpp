#pragma once

#include "Object2D.hpp"
#include "Grid.hpp"

#include <unordered_map>

// Список объектов для отрисовки
class DrawList
{
	friend class UI;
public:
	DrawList();
public:
	void Add(Object2D& obj, const pos2d& dpos);
	void Draw(Graphics& gfx);
	void Translate(int dx, int dy);
	void Translate(const pos2d& dpos);
public:
	Object2D* operator[](const std::string& key)
	{
		return &dList.at(key);
	};
private:
	std::unordered_map<std::string, Object2D> dList;  // Список объектов для отрисовки
	Grid grid;										  // Сетка
};
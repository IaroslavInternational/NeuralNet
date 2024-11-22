#pragma once

#include "Object2D.hpp"

#include <unordered_map>

// Список объектов для отрисовки
class DrawList
{
	friend class UI;
public:
	DrawList(int dx, int dy = 1);
public:
	void Add(Object2D& obj);
	void Draw(Graphics& gfx);
public:
	Object2D* operator[](const std::string& key)
	{
		return &dList.at(key);
	};
private:
	std::unordered_map<std::string, Object2D> dList;  // Список объектов для отрисовки
private:
	float viewPortX;  // Отступ от боковой панели
	float viewPortY;  // Высота меню
};
#pragma once

#include "../Core/Graphics.hpp"
#include "../Core/MainWindow.hpp"

// Сетка поля
class Grid
{
	friend class UI;
public:
	Grid();
public:
	void Draw(Graphics& gfx);
	void Translate(int dx, int dy);
private:
	unsigned padding;
	Color color;
	int wLimit;
	int hLimit;
	int x;
	int y;
};
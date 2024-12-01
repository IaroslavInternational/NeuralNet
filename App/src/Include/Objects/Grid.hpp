#pragma once

#include "../Core/Graphics.hpp"
#include "../Core/MainWindow.hpp"

#include "Cell.hpp"

// Сетка поля
class Grid
{
	friend class UI;
public:
	Grid(Graphics& gfx);
public:
	void Draw(Graphics& gfx);
	void Translate(int dx, int dy);
public:
	Cell* GetCellByPos(int x, int y);
	Cell* GetCellByHover(int x, int y);
	Cell* GetUpperCell(Cell* cell);
	Cell* GetLowerCell(Cell* cell);
private:
	unsigned padding;
	Color color;
	int wLimit;
	int hLimit;
	int x;
	int y;
	std::vector<Cell> cells;
};
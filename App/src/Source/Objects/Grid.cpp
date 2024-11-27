#include "../../Include/Objects/Grid.hpp"

#include "../../Include/Objects/Camera.hpp"
#include "../../../libs/json.hpp"

#include <fstream>

Grid::Grid()
{
	using json = nlohmann::json;
	using namespace std::string_literals;

	std::ifstream dataFile("data/grid.json");
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о сетке");
	}

	json j;
	dataFile >> j;

	int colorRead[4] = {};

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		padding = j.at("padding");
		colorRead[0] = j.at("c-a");
		colorRead[1] = j.at("c-r");
		colorRead[2] = j.at("c-g");
		colorRead[3] = j.at("c-b");
		color = Color(colorRead[0], colorRead[1], colorRead[2], colorRead[3]);
		x = j.at("pos-x");
		y = j.at("pos-y");
		wLimit = int(1300.0f - 1300.0f * 0.2f);
		hLimit = 800 - 24;
	}

	// Тест на заполнение ячейками
	for (size_t i = 0; i < wLimit / padding; i++)
	{
		for (size_t j = 0; j < hLimit / padding; j++)
		{
			cells.emplace_back(i, j);
		}
	}
}

void Grid::Draw(Graphics& gfx)
{
	for (int i = -x; i < wLimit; i++)
	{
		for (int j = -padding; j < hLimit; j += padding)
		{
			if (x + i >= 0 && y + j >= 0 && x + i < gfx.GetWidth() && y + j < gfx.GetHeight())
			{
				gfx.PutPixel(x + i, y + j, color);
			}
		}
	}

	for (int i = -y; i < hLimit; i++)
	{
		for (int j = -padding; j < wLimit; j += padding)
		{
			if (x + j >= 0 && y + i >= 0 && x + j < gfx.GetWidth() && y + i < gfx.GetHeight())
			{
				gfx.PutPixel(x + j, y + i, color);
			}
		}
	}
}

void Grid::Translate(int dx, int dy)
{
	x += dx;
	y += dy;

	for (auto& c : cells)
	{
		c.Translate(dx, dy);
	}
}

Cell* Grid::GetCellByPos(int x, int y)
{
	for (auto& c : cells)
	{
		if (c.GetIdx().x == x && c.GetIdx().y == y)
		{
			return &c;
		}
	}

	return nullptr;
}

Cell* Grid::GetCellByHover(int x, int y)
{
	for (auto& c : cells)
	{
		if (c.IsHovered(x, y))
		{
			return &c;
		}
	}

	return nullptr;
}

Cell* Grid::GetUpperCell(Cell* cell)
{
	return GetCellByPos(cell->GetIdx().x, cell->GetIdx().y - 1);
}

Cell* Grid::GetLowerCell(Cell* cell)
{
	return GetCellByPos(cell->GetIdx().x, cell->GetIdx().y + 1);
}

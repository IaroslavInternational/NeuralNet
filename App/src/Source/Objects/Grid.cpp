#include "../../Include/Objects/Grid.hpp"

#include "../../Include/Objects/Camera.hpp"
#include "../../../libs/json.hpp"

#include <fstream>

Grid::Grid(Graphics& gfx)
{
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

		padding = j.at("padding");
		colorRead[0] = j.at("c-a");
		colorRead[1] = j.at("c-r");
		colorRead[2] = j.at("c-g");
		colorRead[3] = j.at("c-b");
		color = Color(colorRead[0], colorRead[1], colorRead[2], colorRead[3]);
		x = j.at("pos-x");
		y = j.at("pos-y");
		wLimit = R100(gfx.GetWidth() - gfx.GetPanelWidth());
		hLimit = R100(gfx.GetHeight() - gfx.GetMenuHeight());
	}
	
	// Заполнение ячейками
	for (int i = -wLimit / padding; i < 2 * (wLimit / padding); i++)
	{
		for (int j = -hLimit / padding; j < 2 * (hLimit / padding); j++)
		{
			cells.emplace_back(i, j, padding);
		}
	}
}

void Grid::Draw(Graphics& gfx)
{
	for (int i = -wLimit; i < 2 * wLimit; i++)
	{
		for (int j = -hLimit; j < 2 * hLimit; j += padding)
		{
			if (x + i >= 0 && y + j >= 0 && x + i < gfx.GetWidth() && y + j < gfx.GetHeight())
			{
				gfx.PutPixel(x + i, y + j, color);
			}
		}
	}

	for (int i = -hLimit; i < 2 * hLimit; i++)
	{
		for (int j = -wLimit; j < 2 * wLimit; j += padding)
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

int Grid::GetPadding() const
{
	return padding;
}

void Grid::SetPadding(int padding)
{
	this->padding = padding;
	cells.clear();

	// Заполнение ячейками
	for (int i = -wLimit / padding; i < 2 * (wLimit / padding); i++)
	{
		for (int j = -hLimit / padding; j < 2 * (hLimit / padding); j++)
		{
			cells.emplace_back(i, j, padding);
		}
	}
}

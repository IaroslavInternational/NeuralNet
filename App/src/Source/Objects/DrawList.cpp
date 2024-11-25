/* ============================================================
* Файл DrawList.cpp
* Реализация класса DrawList
* Методы:
*	- Add: добавить объект в список отрисовки
*	- Draw: отрисовка всех объектов
*	- Translate: Перемещение объектов на заданный отступ
============================================================ */

#include "../../Include/Objects/DrawList.hpp"

DrawList::DrawList()
	:
	grid(50, Colors::MakeRGB(25, 25, 25), 1300, 800)
{
	// Тест на заполнение ячейками
	for (size_t i = 0; i < 6; i++)
	{
		for (size_t j = 0; j < 6; j++)
		{
			cells.emplace_back(i, j);
		}
	}
}

void DrawList::Add(Object2D& obj, const pos2d& dpos)
{
	obj.SetCell(&cells[dList.size()]);
	obj.Translate(dpos);
	dList.insert({ obj.GetId(), obj });
}

void DrawList::Draw(Graphics& gfx)
{
	grid.Draw(gfx);

	for (auto& obj : dList)
	{
		obj.second.Draw(gfx);
	}
}

void DrawList::Translate(int dx, int dy)
{
	grid.Translate(dx, dy);

	for (auto& obj : dList)
	{
		obj.second.Translate(dx, dy);
	}
}

void DrawList::Translate(const pos2d& dpos)
{
	Translate(dpos.x, dpos.y);
}
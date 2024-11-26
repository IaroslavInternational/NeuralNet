/* ============================================================
* Файл DrawList.cpp
* Реализация класса DrawList
* Методы:
*	- Add: добавить объект в список отрисовки
*	- Draw: отрисовка всех объектов
*	- Translate: Перемещение объектов на заданный отступ
============================================================ */

#include "../../Include/Objects/DrawList.hpp"

#include "../../../libs/json.hpp"

#include <fstream>
#include <sstream>

DrawList::DrawList(const std::string& path, ResourceManager& rManager)
	:
	rManager(rManager)
{
	using json = nlohmann::json;
	using namespace std::string_literals;

	std::ifstream dataFile(path);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о проекте");
	}

	json j;
	dataFile >> j;

	size_t obj_counter = 0;
	std::ostringstream oss;

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto& d = m.key();

		for (const auto& obj : j.at(d))
		{
			auto scene_name = obj.at("name");

			for (const auto& objs : obj.at("objects"))
			{
				for (auto c_obj = objs.begin(); c_obj != objs.end(); c_obj++)
				{
					auto& k = c_obj.key();

					for (const auto& data : objs.at(k))
					{
						Add(Object2D(rManager[data.at("resource")], 
							grid.GetCellByPos(data.at("c-x"), data.at("c-y")),
							data.at("id")));
					}					
				}
			}
		}
	}
}

void DrawList::Add(Object2D& obj)
{
	dList.insert({ obj.GetId(), obj });
}

void DrawList::Draw(Graphics& gfx)
{
	grid.Draw(gfx);
	if (hoveredCell != nullptr)
	{
		hoveredCell->Draw(gfx);
		hoveredCell = nullptr;
	}
	
	for (auto& obj : dList)
	{
		obj.second.Draw(gfx);
	}

	DrawSynapse("N_0", "N_1", Colors::MakeRGB(122, 45, 56), gfx);
	DrawSynapse("N_0", "N_2", Colors::MakeRGB(122, 45, 56), gfx);
}

void DrawList::Translate(int dx, int dy)
{
	grid.Translate(dx, dy);
}

void DrawList::Translate(const pos2d& dpos)
{
	Translate(dpos.x, dpos.y);
}

void DrawList::CheckHover(int x, int y)
{
	hoveredCell = grid.GetCellByHover(x, y);
}

void DrawList::DrawLine(pos2d& p0, pos2d& p1, Color c, Graphics& gfx)
{
	float m = 0.0f;

	if (p1.x != p0.x)
	{
		m = float(p1.y - p0.y) / float(p1.x - p0.x);
	}

	if (p1.x != p0.x && std::fabs(m) <= 1.0f)
	{
		if (p0.x > p1.x)
		{
			std::swap(p0, p1);
		}

		const float b = (float)p0.y - m * (float)p0.x;

		for (int x = p0.x; x < p1.x; x++)
		{
			const float y = m * (float)x + b;

			gfx.PutPixel(x, (int)y, c);
		}
	}
	else
	{
		if (p0.y > p1.y)
		{
			std::swap(p0, p1);
		}

		const float w = float(p1.x - p0.x) / float(p1.y - p0.y);
		const float p = (float)p0.x - w * (float)p0.y;

		for (int y = p0.y; y < p1.y; y++)
		{
			const float x = w * (float)y + p;

			gfx.PutPixel((int)x, y, c);
		}
	}
}

void DrawList::DrawSynapse(Object2D& obj1, Object2D& obj2, Color c, Graphics& gfx)
{
	auto& p1 = obj1.GetPos();
	auto& p2 = obj2.GetPos();

	pos2d _p1;
	_p1.x = p1.x + 50;
	_p1.y = p1.y + 25;

	pos2d _p2;
	_p2.x = p2.x;
	_p2.y = p2.y + 25;

	DrawLine(_p1, _p2, c, gfx);
}

void DrawList::DrawSynapse(const std::string& id1, const std::string& id2, Color c, Graphics& gfx)
{
	DrawSynapse(dList.at(id1), dList.at(id2), c, gfx);
}
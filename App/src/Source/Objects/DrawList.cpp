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
						Add(Object2D(rManager[data.at("resource")], grid.GetCellByPos(data.at("c-x"), data.at("c-y"))));
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
#include "../../Include/Objects/DrawLayer.hpp"

#include "../../../libs/json.hpp"

#include <fstream>
#include <sstream>
#include <winbase.h>

DrawLayer::DrawLayer(ResourceManager& rManager, Grid& grid)
	:
	grid(grid)
{
}

DrawLayer::DrawLayer(const std::string& path, ResourceManager& rManager, Grid& grid)
	:
	grid(grid)
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
			name = obj.at("name");
			type = (LayerType)obj.at("type");

			for (const auto& objs : obj.at("objects"))
			{
				for (auto c_obj = objs.begin(); c_obj != objs.end(); c_obj++)
				{
					auto& k = c_obj.key();

					for (const auto& data : objs.at(k))
					{
						Add(std::move(Object2D(
							rManager[data.at("resource")],
							grid.GetCellByPos(data.at("c-x"), data.at("c-y")),
							data.at("id")
						)));
					}
				}
			}
		}
	}
}

void DrawLayer::Add(Object2D& obj)
{
	dLayer.push_back(obj);
}

void DrawLayer::Insert(Object2D& obj)
{
	for (auto& o : dLayer)
	{
		o.SetCell(grid.GetUpperCell(o.GetCell()));
	}

	auto last_cell = dLayer.back().GetCell();
	pos2d pos = last_cell->GetIdx();
	pos.y += 2;

	obj.SetCell(grid.GetCellByPos(pos.x, pos.y));
	dLayer.push_back(obj);
}

void DrawLayer::Erase()
{
	dLayer.erase(dLayer.begin() + dLayer.size() - 1);

	for (auto& o : dLayer)
	{
		o.SetCell(grid.GetLowerCell(o.GetCell()));
	}
}

void DrawLayer::Draw(Graphics& gfx)
{
	for (auto& obj : dLayer)
	{
		obj.Draw(gfx);
	}
}

void DrawLayer::Translate(int dx, int dy)
{
	for (auto& obj : dLayer)
	{
		obj.Translate(dx, dy);
	}
}

void DrawLayer::Translate(const pos2d& dpos)
{
	Translate(dpos.x, dpos.y);
}

size_t DrawLayer::GetSize() const
{
	return dLayer.size();
}

std::vector<Object2D>& DrawLayer::Get()
{
	return dLayer;
}

LayerType& DrawLayer::GetType()
{
	return type;
}

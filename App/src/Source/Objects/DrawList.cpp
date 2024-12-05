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

DrawList::DrawList(const std::string& path, ResourceManager& rManager, Graphics& gfx)
	:
	grid(gfx)
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

		if (d == "project")
		{
			projectName = j.at(d);
			continue;
		}

		for (const auto& obj : j.at(d))
		{
			Add(DrawLayer(std::string(obj), rManager, grid));
		}
	}
}

void DrawList::Add(DrawLayer& dLayer)
{
	dLayers.push_back(dLayer);
}

void DrawList::Insert(DrawLayer& obj, size_t pos)
{
	dLayers.insert(dLayers.begin() + pos, obj);
}

void DrawList::Delete(DrawLayer* dLayer)
{
	assert(dLayer != nullptr);

	auto iter = std::find(dLayers.begin(), dLayers.end(), *dLayer);
	dLayers.erase(iter);
}

void DrawList::Draw(Graphics& gfx)
{
	grid.Draw(gfx);
	if (hoveredCell != nullptr)
	{
		hoveredCell->Draw(gfx);
	}

	DrawSelectedLayer(selected, gfx);

	for (size_t i = 0; i < dLayers.size(); i++)
	{
		if (i + 1 < dLayers.size())
		{
			// Между входным и выходном слоем не рисуем синапсы
			if (!(dLayers[i].GetType() == LayerType::Input && dLayers[i + 1].GetType() == LayerType::Output))
			{
				DrawSynapses(dLayers[i], dLayers[i + 1], Colors::Syn, gfx);
			}
		}
	}
	
	for (auto& l : dLayers)
	{
		l.Draw(gfx);
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

void DrawList::DrawLine(pos2d& p0, pos2d& p1, Color c, Graphics& gfx, bool isBlended)
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

			if (x >= 0 && y >= 0 && x < gfx.GetWidth() && y < gfx.GetHeight())
			{
				if(isBlended)
				{
					gfx.PutPixelBlended(x, (int)y, c);
				}
				else
				{
					gfx.PutPixel(x, (int)y, c);
				}
			}
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

			if (x >= 0 && y >= 0 && x < gfx.GetWidth() && y < gfx.GetHeight())
			{
				if (isBlended)
				{
					gfx.PutPixelBlended((int)x, y, c);
				}
				else
				{
					gfx.PutPixel((int)x, y, c);
				}
			}
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

void DrawList::DrawSynapses(DrawLayer& l1, DrawLayer& l2, Color c, Graphics& gfx)
{
	for (size_t i = 0; i < l1.GetSize(); i++)
	{
		for (size_t j = 0; j < l2.GetSize(); j++)
		{
			DrawSynapse(l1[i], l2[j], c, gfx);
		}
	}
}

void DrawList::DrawSelectedLayer(DrawLayer* l, Graphics& gfx)
{
	if (l == nullptr)
	{
		return;
	}

	pos2d TopLeft = l->Get()[0].GetPos();
	pos2d BotRight = l->Get().back().GetPos();

	TopLeft.x -= 50;
	TopLeft.y -= 50;

	BotRight.x += 100;
	BotRight.y += 100;

	int w = BotRight.x - TopLeft.x;
	int h = BotRight.y - TopLeft.y;

	for (int i = TopLeft.x; i < TopLeft.x + w; i++)
	{
		for (int j = TopLeft.y; j < TopLeft.y + h; j++)
		{
			if (i >= 0 && j >= 0 && i < gfx.GetWidth() && j < gfx.GetHeight())
			{
				gfx.PutPixelBlended(i, j, Colors::MakeRGB(25, 25, 25));
			}
		}
	}
}
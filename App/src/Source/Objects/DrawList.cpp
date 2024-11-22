#include "../../Include/Objects/DrawList.hpp"

DrawList::DrawList(int dx, int dy)
{
	viewPortX = 0.2f * (float)dx;
	viewPortY = 25.0f * (float)dy;

	for (auto& obj : dList)
	{
		obj.second.Translate(viewPortX, viewPortY);
	}
}

void DrawList::Add(Object2D& obj)
{
	obj.Translate(viewPortX, viewPortY);
	dList.insert({ obj.GetId(), obj });
}

void DrawList::Draw(Graphics& gfx)
{
	for (auto& obj : dList)
	{
		obj.second.Draw(gfx);
	}
}
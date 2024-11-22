#include "../../Include/Objects/DrawList.hpp"

DrawList::DrawList()
{
}

void DrawList::Add(Object2D& obj)
{
	dList.insert({ obj.GetId(), obj });
}

void DrawList::Draw(Graphics& gfx)
{
	for (auto& obj : dList)
	{
		obj.second.Draw(gfx);
	}
}

void DrawList::Translate(const pos2d& dpos)
{
	for (auto& obj : dList)
	{
		obj.second.Translate(dpos);
	}
}
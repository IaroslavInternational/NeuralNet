#include "../../Include/Objects/Cell.hpp"

#include "../../Include/Core/Graphics.hpp"

Cell::Cell(int x, int y)
	:
	x(x),
	y(y),
	size(50),
	pos(x * size, y * size)
{

}

void Cell::Set(int x, int y)
{
	pos.x += (x - this->x) * size;
	pos.y += (y - this->y) * size;

	this->x = x;
	this->y = y;
}

void Cell::SetX(int x)
{
	pos.x += (x - this->x) * size;
	this->x = x;
}

void Cell::SetY(int y)
{
	pos.y += (y - this->y) * size;
	this->y = y;
}

pos2d& Cell::Get()
{
	return pos;
}

pos2d Cell::GetIdx()
{
	return pos2d(x, y);
}

void Cell::Translate(int dx, int dy)
{
	pos.x += dx;
	pos.y += dy;
}

void Cell::Translate(const pos2d& dpos)
{
	Translate(dpos.x, dpos.y);
}

bool Cell::IsHovered(int x, int y)
{
	bool test1 = (pos.x <= x && x <= pos.x + size);
	bool test2 = (pos.y <= y && y <= pos.y + size);
	return (pos.x <= x && x <= pos.x + size) && (pos.y <= y && y <= pos.y + size);
}

void Cell::Draw(Graphics& gfx)
{
	for (int i = pos.x; i < pos.x + size; i++)
	{
		for (int j = pos.y; j < pos.y + size; j++)
		{
			if (i >= 0 && j >= 0 && i < gfx.GetWidth() && j < gfx.GetHeight())
			{
				gfx.PutPixel(i, j, Colors::MakeRGB(25, 25, 25));
			}
		}
	}
}
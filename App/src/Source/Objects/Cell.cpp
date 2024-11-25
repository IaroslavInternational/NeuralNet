#include "../../Include/Objects/Cell.hpp"

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

void Cell::Translate(int dx, int dy)
{
	pos.x += dx;
	pos.y += dy;
}

void Cell::Translate(const pos2d& dpos)
{
	Translate(dpos.x, dpos.y);
}
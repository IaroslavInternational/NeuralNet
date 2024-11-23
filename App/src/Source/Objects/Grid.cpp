#include "../../Include/Objects/Grid.hpp"

#include "../../Include/Objects/Camera.hpp"

Grid::Grid(unsigned padding, const Color& c, int wLimit, int hLimit)
	:
	padding(padding),
	color(c),
	wLimit(wLimit),
	hLimit(hLimit),
	x(0),
	y(0)
{

}

void Grid::Draw(Graphics& gfx)
{
	for (int i = -x; i < wLimit; i++)
	{
		for (int j = -padding; j < hLimit; j += padding)
		{
			if (x + i >= 0 && y + j >= 0 && x + i < gfx.GetWidth() && y + j < gfx.GetHeight())
			{
				gfx.PutPixel(x + i, y + j, color);
			}
		}
	}

	for (int i = -y; i < hLimit; i++)
	{
		for (int j = -padding; j < wLimit; j += padding)
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
}
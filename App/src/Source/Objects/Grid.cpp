#include "../../Include/Objects/Grid.hpp"

Grid::Grid(unsigned padding, const Color& c, int wLimit, int hLimit)
	:
	padding(padding),
	color(c),
	wLimit(wLimit),
	hLimit(hLimit)
{

}

void Grid::Draw(Graphics& gfx)
{
	for (size_t i = 0; i < wLimit; i++)
	{
		for (size_t j = padding; j < hLimit; j += padding)
		{
			gfx.PutPixel(i, j, color);
		}
	}

	for (size_t i = 0; i < hLimit; i++)
	{
		for (size_t j = padding; j < wLimit; j += padding)
		{
			gfx.PutPixel(j, i, color);
		}
	}
}
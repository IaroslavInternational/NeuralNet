#include "../../Include/Objects/Object2D.hpp"

Object2D::Object2D(int x, int y, Texture* tex)
	:
	x(x),
	y(y),
	pTex(tex)
{
}

void Object2D::Draw(Graphics& gfx)
{
	pTex->Draw(x, y, gfx);
}

void Object2D::Translate(int dx, int dy)
{
	x += dx;
	y += dy;
}

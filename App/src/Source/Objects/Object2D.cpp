#include "../../Include/Objects/Object2D.hpp"

Object2D::Object2D(Texture* tex)
	:
	x(tex->GetX()),
	y(tex->GetY()),
	pTex(tex)
{
}

void Object2D::Draw(Graphics& gfx)
{
	pTex->Draw(gfx);
}
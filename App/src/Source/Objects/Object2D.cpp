#include "../../Include/Objects/Object2D.hpp"

#include <random>

Object2D::Object2D(int x, int y, Texture* tex, const std::string& id)
	:
	position(x, y),
	pTex(tex),
	id(id)
{
	if (id == "-1")
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(0, 100);

		auto idx1 = distrib(gen);
		auto idx2 = distrib(gen);

		this->id = std::string("obj#") + std::to_string(idx1) + std::to_string(idx2);
	}
}

Object2D::Object2D(const Object2D& obj)
	:
	position(obj.position),
	pTex(obj.pTex),
	id(obj.id)
{
}

void Object2D::Draw(Graphics& gfx) 
{
	pTex->Draw(position.x, position.y, gfx);
}

std::string& Object2D::GetId()
{
	return id;
}

void Object2D::Translate(int dx, int dy)
{
	position.x += dx;
	position.y += dy;
}

void Object2D::Translate(const pos2d& dpos)
{
	position.x += dpos.x;
	position.y += dpos.y;
}

#include "../../Include/Objects/Object2D.hpp"

#include <random>
#include <cassert>

Object2D::Object2D(Texture* tex, const std::string& id)
	:
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
	cell(obj.cell),
	pTex(obj.pTex),
	id(obj.id)
{
}

void Object2D::Draw(Graphics& gfx) 
{
	assert(cell != nullptr);

	pTex->Draw(cell->Get().x, cell->Get().y, gfx);
}

void Object2D::Translate(int dx, int dy)
{
	//cell->Translate(dx, dy);
}

void Object2D::Translate(const pos2d& dpos)
{
	Translate(dpos.x, dpos.y);
}

std::string& Object2D::GetId()
{
	return id;
}

void Object2D::SetCell(Cell* cell)
{
	this->cell = cell;
}
#include "../../Include/Objects/Object2D.hpp"

#include <random>
#include <cassert>

Object2D::Object2D(Texture* tex, Cell* cell, const std::string& id)
	:
	pTex(tex),
	id(id),
	cell(cell)
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

std::string& Object2D::GetId()
{
	return id;
}

pos2d& Object2D::GetPos()
{
	return cell->Get();
}

void Object2D::SetCell(Cell* cell)
{
	this->cell = cell;
}
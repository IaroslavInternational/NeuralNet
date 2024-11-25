#pragma once

#include "../Core/Graphics.hpp"
#include "../Core/var.hpp"

#include "Cell.hpp"
#include "Texture.hpp"

class Object2D
{
	friend class UI;
public:
	Object2D(Texture* tex, const std::string& id = "-1");
	Object2D(const Object2D& obj);
public:
	void Draw(Graphics& gfx);
	void Translate(int dx, int dy);
	void Translate(const pos2d& dpos);
	std::string& GetId();
	void SetCell(Cell* cell);
private:
	Cell* cell = nullptr; // Ячейка
	Texture* pTex;   // Текстура
	std::string id;  // id
};


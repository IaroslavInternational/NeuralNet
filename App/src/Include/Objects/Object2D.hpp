#pragma once

#include "../Core/Graphics.hpp"
#include "Texture.hpp"

// Позиция в 2-х мерном пространстве
struct pos2d
{
	pos2d(int x, int y)
		:
		x(x),
		y(y)
	{}

	pos2d(const pos2d& pos)
		:
		pos2d(pos.x, pos.y)
	{}

	int x;  // Координата по оX
	int y;  // Координата по оY
};

class Object2D
{
	friend class UI;
public:
	Object2D(int x, int y, Texture* tex, const std::string& id = "-1");
	Object2D(const Object2D& obj);
public:
	void Draw(Graphics& gfx);
	void Translate(int dx, int dy);
	void Translate(const pos2d& dpos);
	std::string& GetId();
private:
	Texture* pTex;   // Текстура
	pos2d position;  // Позиция x, y
	std::string id;  // id
};


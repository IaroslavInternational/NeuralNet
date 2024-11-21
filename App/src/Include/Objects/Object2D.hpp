#pragma once

#include "ResourceManager.hpp"
#include "../Core/Graphics.hpp"

class Object2D
{
	friend class UI;
public:
	Object2D(int x, int y, Texture* tex);
public:
	void Draw(Graphics& gfx);
	void Translate(int x, int y);
private:
	int x;			// Координта по оX
	int y;			// Координта по оY
	Texture* pTex;  // Текстура
};


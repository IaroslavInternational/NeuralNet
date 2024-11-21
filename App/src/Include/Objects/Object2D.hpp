#pragma once

#include "ResourceManager.hpp"
#include "../Core/Graphics.hpp"

class Object2D
{
	friend class UI;
public:
	Object2D(Texture* tex);
public:
	void Draw(Graphics& gfx);
private:
	int x;
	int y;
	Texture* pTex;
};


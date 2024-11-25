#pragma once

#include "../Core/var.hpp"

class Graphics;

class Cell
{
	friend class UI;
public:
	Cell(int x, int y);
public:
	void Set(int x, int y);
	void SetX(int x);
	void SetY(int y);
	pos2d& Get();
	pos2d GetIdx();
	void Translate(int dx, int dy);
	void Translate(const pos2d& dpos);
public:
	bool IsHovered(int x, int y);
	void Draw(Graphics& gfx);
private:
	unsigned size;
	int x;
	int y;
	pos2d pos;
};
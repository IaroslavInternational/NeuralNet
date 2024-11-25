#pragma once

#include "../Core/var.hpp"

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
	void Translate(int dx, int dy);
	void Translate(const pos2d& dpos);
private:
	unsigned size;
	int x;
	int y;
	pos2d pos;
};
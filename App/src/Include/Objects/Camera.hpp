#pragma once

#include "DrawList.hpp"

class Camera
{
	friend class UI;
public:
	Camera(int init_x, int init_y, DrawList& dList);
public:
	void Translate(int dx, int dy);
	pos2d& GetPos();
private:
	pos2d dpos;
	unsigned speed = 2;
	DrawList& dList;
};
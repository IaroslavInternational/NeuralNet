#pragma once

#ifdef NDEBUG
#define F_DEBUG(act);
#else
#define F_DEBUG(act) act;
#endif

// Позиция в 2-х мерном пространстве
struct pos2d
{
	pos2d()
		:
		x(0),
		y(0)
	{}

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

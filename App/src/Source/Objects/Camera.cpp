#include "../../Include/Objects/Camera.hpp"

Camera::Camera(int init_x, int init_y, DrawList& dList)
	:
	dpos(init_x, init_y),
	dList(dList)
{
}

void Camera::Translate(int dx, int dy)
{
	dpos.x += dx * speed;
	dpos.y += dy * speed;

	dList.Translate(dx * speed, dy * speed);
}

pos2d& Camera::GetPos()
{
	return dpos;
}
#pragma once

#include "Window.hpp"

class GUI
{
public:
	GUI(Window* w);
public:
	void render();
private:
	void set_style();
private:
	Window* w;
	int my_image_width = 0;
	int my_image_height = 0;
	ID3D11ShaderResourceView* my_texture = NULL;
};
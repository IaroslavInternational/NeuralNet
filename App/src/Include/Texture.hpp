#pragma once

#include "Graphics.hpp"

#include <string>
#include <memory>
#include <future>

class Texture
{
public:
	Texture(const std::string& path, int x, int y, Color chromakey);
public:
	void Draw(Graphics& gfx);
private:
	void PutPixel(int pos_x, int pos_y, Color c);
	Color& GetPixel(int pos_x, int pos_y);
private:
	std::string path;
	int x;
	int y;
	int width;
	int height;
	Color chromakey;
	std::unique_ptr<Color> pPixels = nullptr;
private:
	std::vector<std::future<void>> workers;
};
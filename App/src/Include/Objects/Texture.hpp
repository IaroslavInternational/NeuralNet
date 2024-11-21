#pragma once

#include "../Core/Graphics.hpp"

#include <string>
#include <memory>
#include <future>

class Texture
{
	friend class UI;
public:
	Texture(const std::string& path, int x, int y, Color chromakey);
	Texture(const Texture& tex)
	{
		path = tex.path;
		x = tex.x;
		y = tex.y;
		width = tex.width;
		height = tex.height;
		chromakey = tex.chromakey;
		pPixels.reset(tex.pPixels.get());
	};
	~Texture()
	{
		pPixels.release();
	}
public:
	void Draw(Graphics& gfx) const;
	std::string GetPath();
	int GetX() const;
	int GetY() const;
private:
	void PutPixel(int pos_x, int pos_y, Color c);
	Color& GetPixel(int pos_x, int pos_y) const;
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

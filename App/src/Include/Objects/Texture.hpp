#pragma once

#include "../Core/Graphics.hpp"

#include <string>
#include <memory>
#include <future>

class Texture
{
	friend class UI;
public:
	Texture(const std::string& path, Color chromakey);
	Texture(const Texture& tex)
	{
		path = tex.path;
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
	void Draw(int x, int y, Graphics& gfx) const;
	std::string GetPath();
private:
	void PutPixel(int pos_x, int pos_y, Color c);
	Color& GetPixel(int pos_x, int pos_y) const;
private:
	std::string path;  // Путь к изображению
	int width;		   // Ширина изображения
	int height;		   // Высота изображения
	Color chromakey;   // Хромокей
	std::unique_ptr<Color> pPixels = nullptr;
private:
	std::vector<std::future<void>> workers;
};

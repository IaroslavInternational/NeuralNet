#include "../../Include/Objects/Texture.hpp"

#include <cassert>
#include <fstream>

Texture::Texture(const std::string& path, Color chromakey)
	:
	path(path),
	chromakey(chromakey)
{
	std::string filepath = "assets/" + path;
	std::ifstream file(filepath, std::ios::binary);
	assert(file);

	BITMAPFILEHEADER bmFileHeader;
	file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

	BITMAPINFOHEADER bmInfoHeader;
	file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

	assert(bmInfoHeader.biBitCount == 24 || bmInfoHeader.biBitCount == 32);
	//assert(bmInfoHeader.biCompression == BI_RGB);

	const bool is32b = bmInfoHeader.biBitCount == 32;

	width = bmInfoHeader.biWidth;

	int yStart;
	int yEnd;
	int dy;
	if (bmInfoHeader.biHeight < 0)
	{
		height = -bmInfoHeader.biHeight;
		yStart = 0;
		yEnd = height;
		dy = 1;
	}
	else
	{
		height = bmInfoHeader.biHeight;
		yStart = height - 1;
		yEnd = -1;
		dy = -1;
	}

	pPixels = std::unique_ptr<Color>(new Color[width * height]);

	file.seekg(bmFileHeader.bfOffBits);

	const int padding = (4 - (width * 3) % 4) % 4;

	for (int i = yStart; i != yEnd; i += dy)
	{
		for (int j = 0; j < width; j++)
		{
			workers.push_back(std::async(std::launch::async, &Texture::PutPixel, this, i, j, Color(255, file.get(), file.get(), file.get())));

			if (is32b)
			{
				file.seekg(1, std::ios::cur);
			}
		}
		if (!is32b)
		{
			file.seekg(padding, std::ios::cur);
		}
	}

	if (workers.size() > 0)
	{
		for (auto& w : workers)
		{
			w.get();
		}

		workers.clear();
	}
}

void Texture::Draw(int x, int y, Graphics& gfx) const
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (chromakey != GetPixel(i, j))
			{
				if (x + j >= 0 && y + i >= 0 && x + j < gfx.GetWidth() && y + i < gfx.GetHeight())
				{
					gfx.PutPixel(x + j, y + i, GetPixel(i, j));
				}
			}
		}
	}
}

std::string Texture::GetPath()
{
	return path;
}

void Texture::PutPixel(int pos_x, int pos_y, Color c)
{
	assert(pos_x >= 0);
	assert(pos_x < width);
	assert(pos_y >= 0);
	assert(pos_y < height);
	pPixels.get()[pos_y * width + pos_x] = c;
}

Color& Texture::GetPixel(int pos_x, int pos_y) const
{
	assert(pos_x >= 0);
	assert(pos_x < width);
	assert(pos_y >= 0);
	assert(pos_y < height);
	return pPixels.get()[pos_y * width + pos_x];
}

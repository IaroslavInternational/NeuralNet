#pragma once

#include "Object2D.hpp"
#include "ResourceManager.hpp"
#include "Grid.hpp"

#include <unordered_map>

// Список объектов для отрисовки
class DrawLayer
{
	friend class UI;
public:
	DrawLayer(const std::string& path, ResourceManager& rManager, Grid& grid);
public:
	void Add(Object2D& obj);
	void Insert(Object2D& obj);
	void Draw(Graphics& gfx);
	void Translate(int dx, int dy);
	void Translate(const pos2d& dpos);
public:
	size_t GetSize() const;
public:
	Object2D& operator[](size_t idx)
	{
		return dLayer[idx];
	}
private:
	std::string name;
	Grid& grid;
	std::vector<Object2D> dLayer;  // Список объектов для отрисовки
};
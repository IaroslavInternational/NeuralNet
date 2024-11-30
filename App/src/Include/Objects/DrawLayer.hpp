#pragma once

#include "Object2D.hpp"
#include "ResourceManager.hpp"
#include "Grid.hpp"

#include <unordered_map>

enum class LayerType
{
	Input,
	Hidden,
	Output
};

// Список объектов для отрисовки
class DrawLayer
{
	friend class UI;
public:
	DrawLayer(const std::string& path, ResourceManager& rManager, Grid& grid);
public:
	void Insert(Object2D& obj);
	void Erase();
	void Draw(Graphics& gfx);
	void Translate(int dx, int dy);
	void Translate(const pos2d& dpos);
public:
	size_t GetSize() const;
private:
	void Add(Object2D& obj);
public:
	Object2D& operator[](size_t idx)
	{
		return dLayer[idx];
	}
	void operator=(DrawLayer& cpyLayer)
	{
		name = cpyLayer.name;
		type = cpyLayer.type;
		grid = cpyLayer.grid;
		dLayer = std::move(cpyLayer.dLayer);
	};
	bool operator==(const DrawLayer& cpyLayer)
	{
		return name == cpyLayer.name;
	}
private:
	std::string name;
	LayerType type;
	Grid& grid;
	std::vector<Object2D> dLayer;  // Список объектов для отрисовки
};
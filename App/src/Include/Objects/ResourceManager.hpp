#pragma once

#include "Texture.hpp"

#include <unordered_map>
#include <string>

class ResourceManager
{
	friend class UI;
public:
	ResourceManager();
public:
	void Add(const std::string& id, Texture& tex);
	const std::string& GetNameBySource(const std::string& id);

	Texture* operator[](const std::string& key)
	{
		return &textures.at(key);
	};
private:
	std::unordered_map<std::string, Texture> textures;
};


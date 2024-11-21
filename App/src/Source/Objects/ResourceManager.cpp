#include "../../Include/Objects/ResourceManager.hpp"

ResourceManager::ResourceManager()
{
	// JSON parse here or smth else
	Add(Texture("item1.bmp", Color(255, 255, 255, 255)));
	Add(Texture("item2.bmp", Color(255, 255, 255, 255)));
}

void ResourceManager::Add(Texture& tex)
{
	textures.insert({ tex.GetPath(), tex });
}
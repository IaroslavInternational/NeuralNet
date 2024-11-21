#include "../../Include/Objects/ResourceManager.hpp"

ResourceManager::ResourceManager()
{
	Add(Texture("item1.bmp", 360, 180, Color(255, 255, 255, 255)));
	Add(Texture("item2.bmp", 250, 50, Color(255, 255, 255, 255)));
}

void ResourceManager::Add(Texture tex)
{
	textures.insert({ tex.GetPath(), tex });
}
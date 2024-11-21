#include "../../Include/Objects/ResourceManager.hpp"

ResourceManager::ResourceManager()
{

}

void ResourceManager::Add(Texture tex)
{
	textures.insert({ tex.GetPath(), tex });
}
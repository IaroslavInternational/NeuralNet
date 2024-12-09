#include "../../Include/Objects/ResourceManager.hpp"

#include "../../../libs/json.hpp"

#include <fstream>

ResourceManager::ResourceManager()
{
	using json = nlohmann::json;
	using namespace std::string_literals;

	std::ifstream dataFile("data/rManager.json");
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о ресурсах");
	}

	json j;
	dataFile >> j;

	std::string currentStr = "";
	int currentColor[4] = {};

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto& resource = m.key();

		for (const auto& obj : j.at(resource))
		{
			currentStr = obj.at("path");
			currentColor[0] = obj.at("ch-key-a");
			currentColor[1] = obj.at("ch-key-r");
			currentColor[2] = obj.at("ch-key-g");
			currentColor[3] = obj.at("ch-key-b");

			Add(resource, std::move(Texture(currentStr, Color(currentColor[0], currentColor[1], currentColor[2], currentColor[3]))));
		}
	}
}

void ResourceManager::Add(const std::string& id, Texture& tex)
{
	textures.insert({ id, tex });
}
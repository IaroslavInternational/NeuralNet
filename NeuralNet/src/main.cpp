/*
* ===== NET SANDBOX FILE ===== 
*/

#include "Include/Library.hpp"
#include "Include/NetSystem.hpp"

#include <iostream>
#include <fstream>

Net::fmatrix weights;

void load()
{
	weights.resize(2);
	weights[0].reserve(49*14);
	weights[1].reserve(14*3);

	std::string current;
	std::ifstream file("weights.txt");

	size_t counter = 0;
	size_t idx = 0;

	if (file.is_open())
	{
		while (std::getline(file, current))
		{
			if (counter < 49 * 14)
			{
				idx = 0;
			}
			else if (counter < 49 * 14 + 14 * 3)
			{
				idx = 1;
			}
			else
			{
				break;
			}

			weights[idx].push_back(std::stof(current));

			counter++;
		}

		file.close();
	}
	else
	{
		throw std::exception("Cannot read a input file!");
	}
}

int main()
{
	load();
	NetSystem nsystem("");

	std::string cmd;						 // Текущая команда
	std::future<Net::train_set> worker;		 // Загрузка датасета в отдельном потоке
	std::vector<std::future<void>> workers;  // Сохранение результатов в отдельном потоке

	// Список соответствий команд
	std::map<std::string, std::function<void()>> cmd_list =
	{
		{"run",		 std::function<void()>(std::bind(&NetSystem::run, &nsystem))},
		{"set w",    std::function<void()>(std::bind(&NetSystem::set_weights, &nsystem, weights))},
		{"train",    std::function<void()>(std::bind(&NetSystem::train, &nsystem, "train"))},
		{"validate", std::function<void()>(std::bind(&NetSystem::validate, &nsystem))},
	};

	// Цикл программы
	while (true)
	{
		std::cout << "cmd>";
		std::getline(std::cin, cmd);
		
		for (auto& c : cmd_list)
		{
			if (c.first == cmd)
			{
				c.second();
			}
		}
	}

	return 0;
}
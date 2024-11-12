/*
* ===== NET SANDBOX FILE ===== 
*/

#include "Include/Library.hpp"
#include "Include/Input.hpp"
#include "Include/NetSystem.hpp"

#include <iostream>

int main()
{
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
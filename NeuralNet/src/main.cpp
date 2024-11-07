#include <iostream>

#include "Include/NeuralNet.hpp"
#include "Include/Monitoring.hpp"

#include "Include/Library.hpp"
#include "Include/Input.hpp"

int main()
{
	// Кол-во нейронов во входном слое, Кол-во промеж. слоёв, 
	// Кол-во нейронов в промеж. слое, Кол-во нейронов в выходном слое
	NeuralNet nn(49, 1, 20, 3);
	Monitoring mon(&nn, "Figures 2.0");  // Объект системы мониторинга

	constexpr size_t max_epochs = 15000;  // Кол-во эпох тренировки
	constexpr float  min_error  = 0.002f; // Цель ошибки

	float out_val = 0.0f;		// Ошибка
	Net::train_set train_sets;  // Датасет для тренировки
	Net::fmatrix res;			// Результаты прогона

	std::string cmd;						 // Текущая команда
	std::future<Net::train_set> worker;		 // Загрузка датасета в отдельном потоке
	std::vector<std::future<void>> workers;  // Сохранение результатов
	bool is_set_loaded = false;				 // Флаг загрузки датасета
	bool is_net_trained = false;			 // Флаг тренировки
	bool is_weights_set = false;			 // Флаг установки весов

	// Цикл программы
	while (true)
	{
		std::cout << "cmd>";
		std::getline(std::cin, cmd);

		if (cmd == "load set")  // Загрузка датасета
		{
			std::cout << "cmd>load set>";
			std::getline(std::cin, cmd);

			worker = std::async(std::launch::async, &Net::get_train_set, cmd + std::string("\\"));

			is_set_loaded = true;
		}
		else if (cmd == "train")  // Тренировка сети
		{
			if (!is_set_loaded)
			{
				std::cout << "Load train set!" << "\n";
				continue;
			}

			train_sets = worker.get();
			res.reserve(train_sets.first.size());

			// Засекаем время тренировки сети
			mon.start();

			size_t epoch;
			for (epoch = 0; epoch < max_epochs; epoch++)
			{
				if (epoch != 0 && out_val < min_error)
				{
					break;
				}

				out_val = 0.0f;
				res.clear();

				for (size_t i = 0; i < train_sets.first.size(); i++)
				{
					nn.train(train_sets.first[i], train_sets.second[i]);
					res.push_back(nn.get_result());
				}

				for (size_t i = 0; i < train_sets.second.size(); i++)
				{
					for (size_t k = 0; k < train_sets.second[0].size(); k++)  // Размер вых. слоя
					{
						out_val += pow(res[i][k] - train_sets.second[i][k], 2);
					}
				}

				out_val = out_val / (train_sets.second.size() * 3);
				mon.get_error(out_val);
			}

			// Получаем для мониторинга кол-во эпох
			mon.get_epoch(epoch);

			// Конец счёта времени тренировки сети
			mon.end();

			is_net_trained = true;
		}
		else if (cmd == "validate")  // Валидация сети
		{
			if (!is_net_trained && !is_weights_set)
			{
				std::cout << "Net is not trained OR Weights were not set!" << "\n";
				continue;
			}
			else if (is_weights_set)
			{
				train_sets = worker.get();
				workers.clear();
			}

			std::cout << "Validation test:" << '\n';

			res.clear();
			for (size_t i = 0; i < train_sets.first.size(); i++)
			{
				nn.run(train_sets.first[i]);
				res.push_back(nn.get_result());
			}

			for (size_t i = 0; i < train_sets.second.size(); i++)
			{
				std::cout << "Train set N_" << i << "\n";
				for (size_t j = 0; j < train_sets.second[0].size(); j++)  // Размер вых. слоя
				{
					std::cout << "Expected: " << train_sets.second[i][j] << " -> Actual: " << res[i][j] << '\n';
				}

				std::cout << "\n";
			}

			workers.push_back(std::async(std::launch::async, &Monitoring::save, &mon));
			workers.push_back(std::async(std::launch::async, &Monitoring::save_weights, &mon));
			workers.push_back(std::async(std::launch::async, &Monitoring::save_report, &mon));
		}
		else if (cmd == "set w")  // Установка весов
		{
			is_weights_set = true;
			nn.set_weights(weights);
		}
	}

	return 0;
}
/*
* ===== NET SANDBOX FILE ===== 
*/

#include "Include/NeuralNet.hpp"
#include "Include/Monitoring.hpp"
#include "Include/Modules/ModuleData.hpp"

#include "Include/Library.hpp"
#include "Include/Input.hpp"

#include <iostream>

// Команды
enum class commands
{
	load_train_set,
	train_net,
	validate_net,
	set_weights,
	run_net
};

// Список соответствий команда
std::map<std::string, commands> cmd_list =
{
	{"load set", commands::load_train_set},
	{"train", commands::train_net},
	{"validate", commands::validate_net},
	{"set w", commands::set_weights},
	{"run", commands::run_net}
};


// Обработка строковой команды
commands proc_cmd(const std::string& cmd)
{
	return cmd_list[cmd];
}

int main()
{
	// Кол-во нейронов во входном слое, Кол-во промеж. слоёв, 
	// Кол-во нейронов в промеж. слое, Кол-во нейронов в выходном слое
	NeuralNet nn(49, 1, 14, 3);
	Monitoring mon(&nn, "Figures 2.0");  // Объект системы мониторинга
	
	ModuleData m_data("scripts/image2bin.py"); // Тест модуля предобработки изображения

	constexpr size_t max_epochs = 5000;   // Кол-во эпох тренировки
	constexpr float  min_error  = 0.002f; // Цель ошибки

	float out_val = 0.0f;		// Ошибка
	Net::train_set train_sets;  // Датасет для тренировки
	Net::fmatrix res;			// Результаты прогона

	std::string cmd;						 // Текущая команда
	commands current_cmd;				     // Текущая команда
	std::future<Net::train_set> worker;		 // Загрузка датасета в отдельном потоке
	std::vector<std::future<void>> workers;  // Сохранение результатов в отдельном потоке
	bool is_set_loaded = false;				 // Флаг загрузки датасета
	bool is_net_trained = false;			 // Флаг тренировки
	bool is_weights_set = false;			 // Флаг установки весов
	
	std::ostringstream oss;
	size_t pass_counter = 0;  // Счётчик правильных результатов
	size_t fail_counter = 0;  // Счётчик неправильных результатов
	size_t acc = 0;			  // Точность прогона
	Net::fvector inputs;	  // Входы для сети

	// Цикл программы
	while (true)
	{
		std::cout << "cmd>";
		std::getline(std::cin, cmd);
		current_cmd = proc_cmd(cmd);

		switch (current_cmd)
		{
			case commands::load_train_set:  // Загрузка датасета
				std::cout << "cmd>load set>";
				std::getline(std::cin, cmd);

				worker = std::async(std::launch::async, &Net::get_train_set, cmd + std::string("\\"));

				is_set_loaded = true;
				break;
			case commands::train_net:  // Тренировка сети
				if (!is_set_loaded)
				{
					std::cout << "Load train set!" << "\n";
					continue;
				}

				train_sets = worker.get();
				Net::shuffle_data(train_sets);
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
							out_val += (float)pow(res[i][k] - train_sets.second[i][k], 2);
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
				break;
			case commands::validate_net:  // Валидация сети
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
					oss << "Train set N_" << i << "\n";
					for (size_t j = 0; j < train_sets.second[0].size(); j++)  // Размер вых. слоя
					{
						oss << "Expected: " << train_sets.second[i][j] << " -> Actual: " << roundf(res[i][j] * 100) / 100 << "\t";

						if (fabs(res[i][j] - train_sets.second[i][j]) <= 0.3f)
						{
							oss << "Pass\n";
							pass_counter++;
						}
						else
						{
							oss << "Failed\n";
							fail_counter++;
						}
					}

					oss << "\n";
				}

				acc = (pass_counter * 100) / (pass_counter + fail_counter);  // Точность
				oss << "Acc: \t" << acc << "%\n";
				oss << "Pass: \t" << pass_counter << "\n";
				oss << "Fail: \t" << fail_counter << "\n";

				if (acc > 85)
				{
					oss << "The Neural Network converges\n";
				}
				else
				{
					oss << "The Neural Network is not converges\n";
				}

				std::cout << oss.str();

				workers.clear();
				workers.push_back(std::async(std::launch::async, &Monitoring::save, &mon));
				workers.push_back(std::async(std::launch::async, &Monitoring::save_weights, &mon));
				workers.push_back(std::async(std::launch::async, &Monitoring::save_report, &mon, oss.str()));
				break;
			case commands::set_weights:
				is_weights_set = true;
				nn.set_weights(weights);
				break;
			case commands::run_net:
				workers.clear();

				std::cout << "cmd>run>";
				std::getline(std::cin, cmd);

				std::remove("nni/proc_data.nni");
				workers.push_back(std::async(std::launch::async, &ModuleData::proc, &m_data, cmd));  // Тест обработки изображения
				
				while (!m_data.is_finished())
				{
					std::cout << "Waiting for data...\n";
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}

				inputs = Net::load_input("nni/proc_data.nni");
				nn.run(inputs);

				res.clear();
				res.push_back(nn.get_result());

				std::cout << "Output Circle" << " = " << res[0][0] << "\n";
				std::cout << "Output Square" << " = " << res[0][1] << "\n";
				std::cout << "Output Triangle" << " = " << res[0][2] << "\n";

				break;
			default:
				break;
		}
	}

	return 0;
}
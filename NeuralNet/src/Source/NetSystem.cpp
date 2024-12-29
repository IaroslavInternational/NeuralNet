#include "../Include/NetSystem.hpp"
#include "../Include/Library.hpp"
#include "../Include/Modules/ModuleData.hpp"

#include <future>

NetSystem::NetSystem(const std::string& config)
	:
	net(2, 1, 4, 1),
	mon(&net, "Test integration")
{
	//add_module(std::move(std::make_unique<ModuleData>("scripts/im2bin.py")));
}

void NetSystem::run()
{
	std::remove("nni/proc_data.nni");
	workers.clear();

	std::vector<std::string> args = 
	{
		"test_data/triangle_noise.png"
	};

	for (size_t i = 0; i < modules.size(); i++)
	{
		if (i > 0)
		{
			while (!modules[i - 1]->is_finished())  // Если предыдущий модуль не закончил работу
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Ожидаем завершения
			}
		}

		workers.push_back(std::async(std::launch::async, &Module::proc, modules[i].get(), args[i]));  // Тест обработки изображения
	}

	while (!modules[modules.size() - 1]->is_finished())  // Если последний модуль не закончил работу
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Ожидаем завершения
	}

	auto inputs = Net::load_input("nni/proc_data.nni");
	net.run(inputs);

	result = net.get_result();
}

void NetSystem::train(const std::string& set_name)
{
	set_loader = std::async(std::launch::async, &Net::get_train_set, set_name + std::string("\\"));

	constexpr size_t max_epochs = 1500;   // Кол-во эпох тренировки
	constexpr float  min_error = 0.002f; // Цель ошибки

	float out_val = 0.0f;		// Ошибка
	Net::fmatrix res;			// Результаты прогона

	train_set = set_loader.get();
	Net::shuffle_data(train_set);
	res.reserve(train_set.first.size());

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

		for (size_t i = 0; i < train_set.first.size(); i++)
		{
			net.train(train_set.first[i], train_set.second[i]);
			res.push_back(net.get_result());
		}

		for (size_t i = 0; i < train_set.second.size(); i++)
		{
			for (size_t k = 0; k < train_set.second[0].size(); k++)  // Размер вых. слоя
			{
				out_val += (float)pow(res[i][k] - train_set.second[i][k], 2);
			}
		}

		out_val = out_val / (train_set.second.size() * train_set.second[0].size());
		mon.get_error(out_val);
	}

	// Получаем для мониторинга кол-во эпох
	mon.get_epoch(epoch);

	// Конец счёта времени тренировки сети
	mon.end();
}

void NetSystem::validate()
{
	Net::fmatrix res;			// Результаты прогона

	std::ostringstream oss;
	size_t pass_counter = 0;  // Счётчик правильных результатов
	size_t fail_counter = 0;  // Счётчик неправильных результатов
	size_t acc = 0;			  // Точность прогона

	res.clear();
	for (size_t i = 0; i < train_set.first.size(); i++)
	{
		net.run(train_set.first[i]);
		res.push_back(net.get_result());
	}

	for (size_t i = 0; i < train_set.second.size(); i++)
	{
		oss << "Train set Num_" << i << "\n";
		for (size_t j = 0; j < train_set.second[0].size(); j++)  // Размер вых. слоя
		{
			oss << "Expected: " << train_set.second[i][j] << " -> Actual: " << roundf(res[i][j] * 100) / 100 << "\t";

			if (fabs(res[i][j] - train_set.second[i][j]) <= 0.3f)
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

	workers.clear();
	workers.push_back(std::async(std::launch::async, &Monitoring::save, &mon));
	workers.push_back(std::async(std::launch::async, &Monitoring::save_weights, &mon));
	workers.push_back(std::async(std::launch::async, &Monitoring::save_report, &mon, oss.str()));
}

void NetSystem::set_weights(const Net::fmatrix& weights)
{
	net.set_weights(weights);
}

Net::fvector NetSystem::get_result()
{
	return net.get_result();
}

void NetSystem::add_module(std::unique_ptr<Module> module)
{
	modules.push_back(std::move(module));
}
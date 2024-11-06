#include <iostream>

#include "Include/NeuralNet.hpp"
#include "Include/Monitoring.hpp"

#include "Include/Library.hpp"
#include "Include/Input.hpp"

int main()
{
	auto train_sets = Net::get_train_set("train\\");

	// Кол-во нейронов во входном слое, Кол-во промеж. слоёв, 
	// Кол-во нейронов в промеж. слое, Кол-во нейронов в выходном слое
	NeuralNet nn(49, 1, 20, 3);
	Monitoring mon(&nn);

#if SET_WEIGHTS == 1
	nn.set_weights(weights);
#endif // SET_WEIGHTS

	float out_val = 1.0f;
	std::vector<std::vector<float>> res;
	res.reserve(train_sets.first.size());

	// Засекаем время тренировки сети
	mon.start();

	for (size_t j = 0; j < 15000; j++)
	{
		if (out_val < 0.002f)
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
			for (size_t k = 0; k < 3; k++)  // Размер вых. слоя
			{
				out_val += pow(res[i][k] - train_sets.second[i][k], 2);
			}
		}
		
		out_val = out_val / (train_sets.second.size() * 3);
		mon.get_error(out_val);
	}

	// Конец счёта времени тренировки сети
	mon.end();

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
		for (size_t j = 0; j < 3; j++)
		{
			std::cout << "Expected: " << train_sets.second[i][j] << " -> Actual: " << res[i][j] << '\n';
		}

		std::cout << "\n";
	}

	mon.save();
	mon.save_weights();
	mon.save_report();

	return 0;
}
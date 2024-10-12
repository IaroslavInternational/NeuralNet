#include <iostream>

#include "Include/NeuralNet.hpp"

/* 
* --- TO DO LIST ---:
* new: Обучение через МОР -> спроектировать
* new: спроектировать класс, собирающий всю информацию о НС
*/


int main()
{
	// Кол-во нейронов во входном слое, Кол-во промеж. слоёв, 
	// Кол-во нейронов в промеж. слое, Кол-во нейронов в выходном слое
	NeuralNet nn(2, 1, 2, 1);

	std::vector<std::vector<float>> train_set =
	{
		{0.0f, 0.0f},
		{0.0f, 1.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f}
	};

	std::vector<std::vector<float>> expected_set =
	{
		{0.0f},
		{1.0f},
		{1.0f},
		{0.0f}
	};

	bool out = false;

	for (size_t j = 0; j < 200; j++)
	{
		if (out)
		{
			break;
		}

		for (size_t i = 0; i < train_set.size(); i++)
		{
			nn.train(train_set[i], expected_set[i]);
		}

		out = nn.get_error().back() < 0.05f;
	}

	std::cout << "TEST:" << std::endl;

	std::vector<std::vector<float>> res;
	for (size_t i = 0; i < train_set.size(); i++)
	{
		res.push_back(nn.run(train_set[i]));
	}

	for (auto& r : res)
	{
		for (auto& r1 : r)
		{
			std::cout << r1 << std::endl;
		}
	}

	return 0;
}
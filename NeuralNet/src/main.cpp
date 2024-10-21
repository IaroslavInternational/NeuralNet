#include <iostream>
#include <chrono>

#include "Include/NeuralNet.hpp"
#include "Include/Monitoring.hpp"

/* 
* --- TO DO LIST ---:
* new: Обучение через МОР -> спроектировать
* new: спроектировать класс, собирающий всю информацию о НС
*/


int main()
{
	// Кол-во нейронов во входном слое, Кол-во промеж. слоёв, 
	// Кол-во нейронов в промеж. слое, Кол-во нейронов в выходном слое
	NeuralNet nn(2, 1, 4, 1);
	Monitoring mon(&nn);

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

	std::vector<float> w =
	{
-8.61197f,
3.5581f,
-6.17751f,
3.49245f,
2.81937f,
-8.6373f,
-6.13125f,
-1.18834f,
5.2806f,
8.1035f,
-16.4926f,
-4.24173f

	};

	//nn.set_weights(&w);

	bool out = false;
	float out_val = 0.0f;
	std::vector<std::vector<float>> res;

	// Get starting timepoint
	auto start = std::chrono::high_resolution_clock::now();

	for (size_t j = 0; j < 100000; j++)
	{
		out_val = 0.0f;
		if (out)
		{
			break;
		}

		for (size_t i = 0; i < train_set.size(); i++)
		{
			nn.train(train_set[i], expected_set[i]);
			//mon.update();
		}

		res.clear();
		for (size_t k = 0; k < 4; k++)
		{
			res.push_back(nn.run(train_set[k]));
		}

		for (size_t s = 0; s < 4; s++)
		{
			out_val += pow(res[s][0] - expected_set[s][0], 2);
		}
		
		out = out_val / 4 < 0.0005f;
	}

	// Get ending timepoint
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	std::cout << "Training time: " << duration.count() << " ms" << std::endl;

	std::cout << "TEST:" << std::endl;

	res.clear();
	for (size_t i = 0; i < train_set.size(); i++)
	{
		res.push_back(nn.run(train_set[i]));
	}

	for (size_t i = 0; i < expected_set.size(); i++)
	{
		std::cout << "Expected: " << expected_set[i][0] << " -> Real: " << res[i][0] << std::endl;
	}

	mon.show_weights();

	std::cout << std::endl;

	//std::cout << std::endl;

	mon.print();

	return 0;
}
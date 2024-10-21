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

	bool out = false;
	float out_val = 0.0f;
	std::vector<std::vector<float>> res;
	res.reserve(4);

	// Get starting timepoint
	auto start = std::chrono::high_resolution_clock::now();

	for (size_t j = 0; j < 100000; j++)
	{
		if (out)
		{
			break;
		}
		
		out_val = 0.0f;
		res.clear();

		for (size_t i = 0; i < train_set.size(); i++)
		{
			nn.train(train_set[i], expected_set[i]);
			res.push_back(nn.get_result());
			//mon.update();
		}

		for (size_t i = 0; i < expected_set.size(); i++)
		{
			for (size_t j = 0; j < 1; j++)  // Размер вых. слоя
			{
				out_val += pow(res[i][j] - expected_set[i][j], 2);
			}
		}
		
		out = out_val / expected_set.size() < 0.0005f;
	}

	// Get ending timepoint
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Training time: " << duration.count() << " ms" << '\n';

	std::cout << "TEST:" << '\n';

	res.clear();
	for (size_t i = 0; i < train_set.size(); i++)
	{
		nn.run(train_set[i]);
		res.push_back(nn.get_result());
	}

	for (size_t i = 0; i < expected_set.size(); i++)
	{
		std::cout << "Expected: " << expected_set[i][0] << " -> Actual: " << res[i][0] << '\n';
	}

	//mon.show_weights();

	//mon.print();

	return 0;
}
#include <iostream>
#include <chrono>

#include "Include/NeuralNet.hpp"
#include "Include/Monitoring.hpp"

#include "Include/Library.hpp"

int main()
{
	auto train_sets = Net::get_train_set("train\\");

	// Кол-во нейронов во входном слое, Кол-во промеж. слоёв, 
	// Кол-во нейронов в промеж. слое, Кол-во нейронов в выходном слое
	NeuralNet nn(49, 1, 20, 3);
	Monitoring mon(&nn);

	float out_val = 1.0f;
	std::vector<std::vector<float>> res;
	res.reserve(4);

	// Get starting timepoint
	auto start = std::chrono::high_resolution_clock::now();

	for (size_t j = 0; j < 10000; j++)
	{
		if (out_val < 0.005f)
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

	// Get ending timepoint
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Training time: " << duration.count() << " ms" << '\n';

	std::cout << "TEST:" << '\n';

	res.clear();
	for (size_t i = 0; i < train_sets.first.size(); i++)
	{
		nn.run(train_sets.first[i]);
		res.push_back(nn.get_result());
	}

	for (size_t i = 0; i < train_sets.second.size(); i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			std::cout << "Expected: " << train_sets.second[i][j] << " -> Actual: " << res[i][j] << '\n';
		}

		std::cout << "\n";
	}

	//mon.show_weights();

	mon.save();

	return 0;
}
#include <iostream>

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

	/*std::vector<float> w =
	{
		0.45f, 
		0.78f,
		-0.12f,
		0.13f,
		1.5f,
		-2.3f
	};*/

	//nn.set_weights(&w);

	bool out = false;
	float out_val = 0.0f;
	std::vector<std::vector<float>> res;
	for (size_t j = 0; j < 1000000; j++)
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


	//std::cout << std::endl;

	mon.print();

	return 0;
}
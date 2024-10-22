#include "../Include/Monitoring.hpp"
#include <fstream>
#include <iostream>

Monitoring::Monitoring(NeuralNet* nn)
	:
	nn(nn)
{
}

void Monitoring::update()
{
	//data.push_back(nn->hidden_layers[0].synapses[0].get_weight());
	//data.push_back(nn->error[0]);
}



void Monitoring::save()
{
	std::ofstream ofile{};
	ofile.open("data.txt", std::ios::out);
	for (auto& d : data)
	{
		ofile << d << std::endl;
	}
	ofile.close();

	system("python main.py");
}

void Monitoring::get_error(float error)
{
	data.push_back(error);
}

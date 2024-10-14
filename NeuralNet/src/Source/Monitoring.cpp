#include "../Include/Monitoring.hpp"
#include <fstream>

Monitoring::Monitoring(NeuralNet* nn)
	:
	nn(nn)
{
}

void Monitoring::update()
{
	//data.push_back(nn->hidden_layers[0].synapses[0].get_weight());
	data.push_back(nn->error[0]);
}

void Monitoring::print()
{
	std::ofstream ofile{};
	ofile.open("data.txt", std::ios::out); //app is append which means it will put the text at the end
	for (auto& d : data)
	{
		ofile << d << std::endl;
	}
	ofile.close();
}

#include "../Include/Monitoring.hpp"

Monitoring::Monitoring(NeuralNet* nn)
	:
	nn(nn)
{
}

void Monitoring::update()
{
	//weights.push_back(nn->hidden_layers[0].synapses[0].get_weight());
	data.push_back(nn->error[0]);
}

void Monitoring::print()
{
	for (auto& d : data)
	{
		std::cout << d << std::endl;
	}
}

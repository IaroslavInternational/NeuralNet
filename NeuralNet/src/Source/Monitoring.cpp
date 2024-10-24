#include "../Include/Monitoring.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

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
	std::ofstream ofile("data.txt", std::ios::out);
	for (auto& d : data)
	{
		ofile << d << '\n';
	}
	ofile.close();

	system("python main.py");
}

void Monitoring::save_weights()
{
	std::ostringstream out;
	out << "Net::fmatrix weights =\n{\n\t{";

	for (size_t i = 0; i < nn->input_layer.synapses.size(); i++)
	{
		out << nn->input_layer.synapses[i].weight;

		if (i != nn->input_layer.synapses.size() - 1)
		{
			out << ", ";
		}
		else
		{
			out << "}";
		}
	}

	out << ",\n{";

	for (size_t i = 0; i < nn->hidden_layers.size(); i++)
	{
		for (size_t j = 0; j < nn->hidden_layers[i].synapses.size(); j++)
		{
			out << nn->hidden_layers[i].synapses[j].weight;

			if (j != nn->hidden_layers[i].synapses.size() - 1)
			{
				out << ", ";
			}
			else
			{
				out << "}";
			}
		}

		if (i != nn->hidden_layers.size() - 1)
		{
			out << ",\n\t{";
		}
	}

	out << "\n};";

	std::ofstream ofile("weights.txt", std::ios::out);
	ofile << out.str();
	ofile.close();
}

void Monitoring::get_error(float error)
{
	data.push_back(error);
}

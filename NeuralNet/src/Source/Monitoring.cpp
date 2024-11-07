#include "../Include/Monitoring.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Monitoring::Monitoring(NeuralNet* nn, const std::string& model_name)
	:
	nn(nn),
	model_name(model_name)
{
	data.reserve(10000);
}

void Monitoring::save()
{	
	data.shrink_to_fit();

	if (data.size() == 0)
	{
		return;
	}

	std::ofstream ofile("data.txt", std::ios::out);
	for (auto& d : data)
	{
		ofile << d << '\n';
	}
	ofile.close();

	try
	{
		system("python main.py");
	}
	catch (const std::exception& ex)
	{
		std::ostringstream out;
		out << "Python problem appears! " << ex.what();
		throw std::exception(out.str().c_str());
	}
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

void Monitoring::save_report()
{
	std::ostringstream out;
	out << "Report file for Neural Net\n";
	out << "Date:\t" << std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() } << "\n\n";

	out << "Model " << model_name << ": \n";
	out << "Input layer size:\t" << nn->input_layer.neurons.size() << "\n";
	out << "Hidden layers amount:\t" << nn->hidden_layers.size() << "\n";
	out << "Output layer size:\t" << nn->output_layer.neurons.size() << "\n";
	out << "Training time:\t" << get_duration() << "ms" << "\n";
	out << "Number of epochs:\t" << epoch << "\n";

	std::ofstream ofile("report.txt", std::ios::out);
	ofile << out.str();
	ofile.close();
}

void Monitoring::get_error(float error)
{
	data.push_back(error);
}

void Monitoring::get_epoch(size_t epoch)
{
	this->epoch = epoch;
}

void Monitoring::start()
{
	start_time = std::chrono::high_resolution_clock::now();
}

void Monitoring::end()
{
	end_time = std::chrono::high_resolution_clock::now();
}

long long Monitoring::get_duration()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
}

#include "../../Include/Neuron/OutputNeuron.hpp"

OutputNeuron::OutputNeuron(const std::string& name)
	:
	Neuron(name)
{}


void OutputNeuron::get_input(float value)
{
	input += value;
}

void OutputNeuron::activate()
{
	input = 1 / (1 + exp(-input));
	output = input;
}
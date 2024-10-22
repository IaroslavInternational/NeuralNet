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
	output = 1 / (1 + exp(-input));
	input = 0.0f;
}
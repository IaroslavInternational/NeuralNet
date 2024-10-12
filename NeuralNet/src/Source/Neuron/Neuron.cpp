#include "../../Include/Neuron/Neuron.hpp"

Neuron::Neuron(const std::string& name)
	:
	id(name)
{
}

void Neuron::get_input(float value)
{
	input = value;
	output = input;
}

float Neuron::get_output() const
{
	return output;
}
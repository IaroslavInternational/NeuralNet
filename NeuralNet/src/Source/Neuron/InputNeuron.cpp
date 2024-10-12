#include "../../Include/Neuron/InputNeuron.hpp"

InputNeuron::InputNeuron(const std::string& name)
	:
	Neuron(name)
{}

void InputNeuron::get_input(float value)
{
	input = value;
	output = input;
}

float InputNeuron::get_output() const
{
	return output;
}

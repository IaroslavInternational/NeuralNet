#include "../../Include/Neuron/HiddenNeuron.hpp"

#include <math.h>

HiddenNeuron::HiddenNeuron(const std::string& name)
	:
	Neuron(name)
{}

void HiddenNeuron::get_input(float value)
{
	input += value;
}

float HiddenNeuron::get_output() const
{
	return output;
}

void HiddenNeuron::activate()
{
	output = 1 / (1 + exp(-input));
	input = 0.0f;
}

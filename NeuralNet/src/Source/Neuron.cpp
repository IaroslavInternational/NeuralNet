#include "../Include/Neuron.hpp"
#include <math.h>

void Neuron::get_input(float value)
{
	input += value;
}

float Neuron::get_output() const
{
	return output;
}

void Neuron::activate()
{
	input = 1 / (1 + exp(-input));
	output = input;
}

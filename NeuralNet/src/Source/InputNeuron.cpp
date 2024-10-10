#include "../Include/InputNeuron.hpp"

void InputNeuron::get_input(float value)
{
	input = value;
	output = input;
}

float InputNeuron::get_output() const
{
	return output;
}

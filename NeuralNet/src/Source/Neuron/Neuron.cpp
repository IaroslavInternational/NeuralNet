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

void Neuron::set_delta(float delta)
{
	this->delta = delta;
}

float Neuron::get_delta() const
{
	return delta;
}

std::string Neuron::get_name() const
{
	return id;
}

void Neuron::set_bias(float b)
{
	offset = b;
}

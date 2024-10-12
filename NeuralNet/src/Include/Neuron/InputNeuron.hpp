#pragma once

#include "Neuron.hpp"

class InputNeuron : public Neuron
{
public:
	InputNeuron(const std::string& name);
public:
	void get_input(float value) override;
	float get_output() const override;
};


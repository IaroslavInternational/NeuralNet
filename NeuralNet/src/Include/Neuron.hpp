#pragma once

#include "InputNeuron.hpp"

class Neuron : public InputNeuron
{
public:
	void get_input(float value) override;
	float get_output() const override;
public:
	void activate();
};


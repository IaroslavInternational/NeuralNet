#pragma once

#include "Neuron.hpp"

class OutputNeuron : public Neuron
{
public:
	OutputNeuron(const std::string& name);
public:
	void get_input(float value) override;
public:
	void activate();
};


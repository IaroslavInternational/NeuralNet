#pragma once

#include "Neuron.hpp"

class HiddenNeuron : public Neuron
{
public:
	HiddenNeuron(const std::string& name);
public:
	void get_input(float value) override;
	float get_output() const override;
public:
	void activate();
};


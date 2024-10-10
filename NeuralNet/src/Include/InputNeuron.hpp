#pragma once

#include "Synapse.hpp"

class InputNeuron
{
public:
	virtual void get_input(float value);
	virtual float get_output() const;
protected:
	float input = 0;
	float output = 0;
};


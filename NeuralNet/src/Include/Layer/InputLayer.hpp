#pragma once

#include "Layer.hpp"

class InputLayer : public Layer<InputNeuron>
{
public:
	InputLayer(size_t n, const std::string& name);
public:
	void set_input(const std::vector<float>& values);
	void forward();
};
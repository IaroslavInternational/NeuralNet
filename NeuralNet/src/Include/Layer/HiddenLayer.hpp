#pragma once

#include "Layer.hpp"

class HiddenLayer : public Layer<HiddenNeuron>
{
public:
	HiddenLayer(size_t n, const std::string& name);
public:
	void forward();
	void activate();
};


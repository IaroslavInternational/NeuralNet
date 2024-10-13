#pragma once

#include "Layer.hpp"

class HiddenLayer : public Layer<HiddenNeuron>
{
	friend class Monitoring;
public:
	HiddenLayer(size_t n, const std::string& name);
public:
	void forward();
	void activate();
};


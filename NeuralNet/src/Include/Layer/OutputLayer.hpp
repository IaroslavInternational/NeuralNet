#pragma once

#include "Layer.hpp"

class OutputLayer : public Layer<OutputNeuron>
{
public:
	OutputLayer(size_t n, const std::string& name);
public:
	void activate(); 
	void get_result();
};


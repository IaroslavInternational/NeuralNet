#pragma once

#include "Layer.hpp"

class OutputLayer : public Layer<OutputNeuron>
{
public:
	OutputLayer(size_t n, const std::string& name);
public:
	void activate(); 
	std::vector<float>& get_result();
};


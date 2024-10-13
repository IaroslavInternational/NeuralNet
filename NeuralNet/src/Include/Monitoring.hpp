#pragma once

#include "NeuralNet.hpp"

class Monitoring
{
public:
	Monitoring(NeuralNet* nn);
public:
	void update();
	void print();
private:
	std::vector<float> data;
	NeuralNet* nn;
};


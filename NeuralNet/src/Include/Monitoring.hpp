#pragma once

#include "NeuralNet.hpp"

class Monitoring
{
public:
	Monitoring(NeuralNet* nn);
public:
	void update();
	void save();
	void get_error(float error);
private:
	std::vector<float> data;
	NeuralNet* nn;
};


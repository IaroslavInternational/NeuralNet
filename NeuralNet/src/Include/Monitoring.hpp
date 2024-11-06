#pragma once

#include "NeuralNet.hpp"

#include <chrono>

class Monitoring
{
public:
	Monitoring(NeuralNet* nn);
public:
	void save();
	void save_weights();
	void save_report();
	void get_error(float error);
public:
	void start();
	void end();
	long long get_duration();
private:
	NeuralNet* nn;
	std::vector<float> data;
	std::chrono::steady_clock::time_point start_time;
	std::chrono::steady_clock::time_point end_time;
};


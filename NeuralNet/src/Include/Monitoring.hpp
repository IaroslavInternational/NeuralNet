#pragma once

#include "NeuralNet.hpp"

#include <chrono>

class Monitoring
{
	friend class UI;
public:
	Monitoring(NeuralNet* nn, const std::string& model_name);
public:
	void save();
	void save_weights();
	void save_report(const std::string& report);
	void get_error(float error);
	void get_epoch(size_t epoch);
public:
	void start();
	void end();
	long long get_duration();
private:
	NeuralNet* nn;
	std::vector<float> data;
	size_t epoch = 0;
	std::string model_name;
	std::chrono::steady_clock::time_point start_time;
	std::chrono::steady_clock::time_point end_time;
};


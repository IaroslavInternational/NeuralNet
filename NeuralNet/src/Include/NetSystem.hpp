#pragma once

#include "Modules/Module.hpp"
#include "NeuralNet.hpp"
#include "Monitoring.hpp"

class NetSystem
{
public:
	NetSystem(const std::string& config);
public:
	void run();
	void train(const std::string& set_name);
	void validate();
	void set_weights(const Net::fmatrix& weights);
	Net::fvector get_result();
public:
	void add_module(std::unique_ptr<Module> module);
private:
	NeuralNet net;
	Monitoring mon;
	std::vector<std::unique_ptr<Module>> modules;
private:
	std::future<Net::train_set> set_loader;
	std::vector<std::future<void>> workers;
	Net::train_set train_set;   // Датасет для тренировки
	Net::fvector result;
};
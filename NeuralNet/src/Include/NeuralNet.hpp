#pragma once

#include "Layer/InputLayer.hpp"
#include "Layer/HiddenLayer.hpp"
#include "Layer/OutputLayer.hpp"

#include "Library.hpp"

#include <future>

class NeuralNet
{
	friend class UI;
	friend class Monitoring;
public:
	NeuralNet(size_t I_Layer_size, size_t H_Layer_num, size_t H_Layer_size, size_t O_Layer_size);
public:
	void run(const Net::fvector& inputs);
	void train(const Net::fvector& inputs, const Net::fvector& expected);
	void set_weights(const Net::fmatrix& weights);
	void set_params(float E = 0.7f, float a = 0.3f);
	std::vector<float>& get_result();
private:
	template<class T1, class T2> void create_synapses(Layer<T1>* l1, Layer<T2>* l2);
private:
	InputLayer input_layer;
	std::vector<HiddenLayer> hidden_layers;
	OutputLayer output_layer;
private:
	std::vector<float> result;
	float error;
	float E;
	float a;
private:
	std::vector<Synapse>* train_synapses = nullptr;
	std::vector<std::future<void>> workers;
};

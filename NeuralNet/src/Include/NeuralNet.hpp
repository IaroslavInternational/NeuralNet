#pragma once

#include "Layer/InputLayer.hpp"
#include "Layer/HiddenLayer.hpp"
#include "Layer/OutputLayer.hpp"

class NeuralNet
{
	friend class Monitoring;
public:
	NeuralNet(size_t I_Layer_size, size_t H_Layer_num, size_t H_Layer_size, size_t O_Layer_size);
public:
	void train(const std::vector<float>& inputs, const std::vector<float>& expected);
	std::vector<float>& run(const std::vector<float>& inputs);
	std::vector<float>& get_result();
	std::vector<float>& get_error();
	void set_weights(std::vector<float>* weights);
private:
	template<class T1, class T2> void create_synapses(Layer<T1>* l1, Layer<T2>* l2);
private:
	InputLayer input_layer;
	std::vector<HiddenLayer> hidden_layers;
	OutputLayer output_layer;
private:
	size_t iterations = 0;
	std::vector<float> result;
	std::vector<float> error;
	float E = 0.7f;
	float a = 0.3f;
	std::vector<float> delta_output;
};

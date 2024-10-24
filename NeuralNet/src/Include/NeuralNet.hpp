#pragma once

#include "Layer/InputLayer.hpp"
#include "Layer/HiddenLayer.hpp"
#include "Layer/OutputLayer.hpp"

#include "Library.hpp"

class NeuralNet
{
	friend class Monitoring;
public:
	NeuralNet(size_t I_Layer_size, size_t H_Layer_num, size_t H_Layer_size, size_t O_Layer_size);
public:
	void train(const std::vector<float>& inputs, const std::vector<float>& expected);
	void run(const std::vector<float>& inputs);
	std::vector<float>& get_result();
	void set_weights(const Net::fmatrix& weights);
private:
	template<class T1, class T2> void create_synapses(Layer<T1>* l1, Layer<T2>* l2);
	void update_synapse(Synapse* synapse);
private:
	InputLayer input_layer;
	std::vector<HiddenLayer> hidden_layers;
	OutputLayer output_layer;
private:
	std::vector<float> result;
	float error;
	float E = 0.7f;
	float a = 0.5f;
private:
	std::vector<Synapse>* train_synapses = nullptr;
};

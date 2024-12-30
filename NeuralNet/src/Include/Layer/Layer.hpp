#pragma once

#include "../Synapse.hpp"
#include "../Neuron/InputNeuron.hpp"
#include "../Neuron/HiddenNeuron.hpp"
#include "../Neuron/OutputNeuron.hpp"

#include <vector>
#include <string>
#include <cassert>

template<class T>
class Layer
{
	friend class Monitoring;
public:
	Layer(size_t n, const std::string& name);
public:
	size_t length() const;
	template<class T1, class T2> void add_synapse(std::shared_ptr<T1> from, std::shared_ptr<T2> to);
	std::shared_ptr<T> get_neuron(size_t index);
	void set_weights(const std::vector<float>& weights);
public:
	std::vector<Synapse>* get_synapses();
	void clear();
protected:
	std::vector<std::shared_ptr<T>> neurons;
	std::vector<Synapse> synapses;
	std::string name;
};

template<class T>
Layer<T>::Layer(size_t n, const std::string& name)
	:
	name(name)
{
	assert(n != 0);
	neurons.reserve(n);
	for (size_t i = 0; i < n; i++)
	{
		neurons.push_back(std::make_shared<T>(name + ": N_" + std::to_string(i)));
	}
}

template<class T>
size_t Layer<T>::length() const
{
	return neurons.size();
}

template<class T>
template<class T1, class T2>
void Layer<T>::add_synapse(std::shared_ptr<T1> from, std::shared_ptr<T2> to)
{
	synapses.push_back(Synapse(from, to));
}

template<class T>
std::shared_ptr<T> Layer<T>::get_neuron(size_t index)
{
	return neurons[index];
}

template<class T>
void Layer<T>::set_weights(const std::vector<float>& weights)
{
	assert(weights.size() == synapses.size());

	for (size_t i = 0; i < synapses.size(); i++)
	{
		synapses[i].set_weight(weights[i]);
	}
}

template<class T>
std::vector<Synapse>* Layer<T>::get_synapses()
{
	return &synapses;
}

template<class T>
void Layer<T>::clear()
{
	neurons.clear();
	synapses.clear();
}
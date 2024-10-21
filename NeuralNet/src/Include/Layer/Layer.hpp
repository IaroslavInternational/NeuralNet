#pragma once

#include "../Synapse.hpp"
#include "../Neuron/InputNeuron.hpp"
#include "../Neuron/HiddenNeuron.hpp"
#include "../Neuron/OutputNeuron.hpp"

#include <vector>
#include <string>
#include <cassert>
#include <iostream>

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
	Synapse* get_next_synapse();
	void set_weights(std::vector<float>* weights);
public:
	std::vector<Synapse>* get_synapses();
protected:
	std::vector<std::shared_ptr<T>> neurons;
	std::vector<Synapse> synapses;
	std::string name;
	size_t next_index = 0;
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
Synapse* Layer<T>::get_next_synapse()
{
	if (next_index > synapses.size() - 1)
	{
		next_index = 0;
		return nullptr;
	}

	return &synapses[next_index++];
}

template<class T>
void Layer<T>::set_weights(std::vector<float>* weights)
{
	size_t i = 0;
	for (auto it = weights->begin(); it != weights->end(); ++it)
	{
		if (i < synapses.size())
		{
			synapses[i].set_weight(*it);
			i++;
		}
		else
		{
			break;
		}
	}
	
	weights->erase(weights->begin(), weights->begin() + i);
}

template<class T>
std::vector<Synapse>* Layer<T>::get_synapses()
{
	return &synapses;
}

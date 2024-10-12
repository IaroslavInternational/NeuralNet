#pragma once

#include "../Synapse.hpp"
#include "../Neuron/InputNeuron.hpp"
#include "../Neuron/HiddenNeuron.hpp"
#include "../Neuron/OutputNeuron.hpp"

#include <vector>
#include <string>
#include <cassert>
#include <iostream>

/*
* LAYER:
* -тип нейронов
* -массив нейронов
* -массив исходящих синапсов из этих нейронов
* Парметры:
* -кол-во нейронов
* 
*/

template<class T>
class Layer
{
public:
	Layer(size_t n, const std::string& name);
public:
	size_t length() const;
	template<class T1, class T2> void add_synapse(std::shared_ptr<T1> from, std::shared_ptr<T2> to);
	std::shared_ptr<T> get_neuron(size_t index);
protected:
	std::vector<std::shared_ptr<T>> neurons;
	std::vector<Synapse> synapses;
	std::string name;
	size_t pos = 0;
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
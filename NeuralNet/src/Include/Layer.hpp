#pragma once

#include "Synapse.hpp"
#include "InputNeuron.hpp"

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

class Synapse;

template <class T>
class Layer
{
public:
	Layer(size_t n, const std::string& name);
public:
	size_t length() const;
	void add_synapse(InputNeuron* from, InputNeuron* to);
	T* get_neuron(size_t index);
public:
	void set_input(const std::vector<int>& values); // Only INPUT LAYER
	void forward();	// FOR ALL EXCEPT OUTPUT LAYER
	void activate(); // FOR ALL EXCEPT INPUT LAYER 
	void get_result(); // Only OUTPUT LAYER
private:
	std::vector<T> neurons;
	std::vector<Synapse> synapses;
	std::string name;
};

template<class T>
inline Layer<T>::Layer(size_t n, const std::string& name)
{
	assert(n != 0);
	neurons.reserve(n);
	for (size_t i = 0; i < n; i++)
	{
		neurons.push_back(T());
	}
}

template<class T>
inline size_t Layer<T>::length() const
{
	return neurons.size();
}

template<class T>
inline T* Layer<T>::get_neuron(size_t index)
{
	return &neurons[index];
}

template<class T>
inline void Layer<T>::set_input(const std::vector<int>& values)
{
	assert(neurons.size() == values.size());

	for (size_t i = 0; i < values.size(); i++)
	{
		neurons[i].get_input(values[i]);
	}
}

template<class T>
inline void Layer<T>::forward()
{
	for (auto& s : synapses)
	{
		s.send();
	}
}

template<class T>
inline void Layer<T>::activate()
{
	//assert(T == InputNeuron);
	for (auto& n : neurons)
	{
		n.activate();
	}
}

template<class T>
inline void Layer<T>::get_result()
{
	for (auto& n : neurons)
	{
		std::cout << n.get_output() << std::endl;
	}
}

template <class T>
inline void Layer<T>::add_synapse(InputNeuron* from, InputNeuron* to)
{
	synapses.push_back(Synapse(from, to));
}

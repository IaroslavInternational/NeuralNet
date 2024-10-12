#pragma once

#include <memory>

class Neuron;

class Synapse
{
public:
	Synapse(std::shared_ptr<Neuron> from, std::shared_ptr<Neuron> to);
	Synapse(float w, std::shared_ptr<Neuron> from, std::shared_ptr<Neuron> to);
public:
	void send() const;
private:
	float weight;
	std::shared_ptr<Neuron> from;
	std::shared_ptr<Neuron> to;
};

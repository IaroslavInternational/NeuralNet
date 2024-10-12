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
	bool valid() const;
	std::shared_ptr<Neuron> get_from();
	std::shared_ptr<Neuron> get_to();
	float get_weight() const;
	void update_weight(float delta, float alfa);
private: 
	float weight;
	float prev_weight_delta = 0.0f;
	std::shared_ptr<Neuron> from;
	std::shared_ptr<Neuron> to;
};

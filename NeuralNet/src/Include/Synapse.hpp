#pragma once

#include <memory>

class Neuron;

class Synapse
{
	friend class Monitoring;
public:
	Synapse(std::shared_ptr<Neuron> from, std::shared_ptr<Neuron> to);
	Synapse(float w, std::shared_ptr<Neuron> from, std::shared_ptr<Neuron> to);
public:
	void send() const;
	std::shared_ptr<Neuron>& get_from();
	std::shared_ptr<Neuron>& get_to();
	void set_weight(float weight);
	float get_weight() const;
	void update_weight(float E, float alfa);
private: 
	float weight;
	float prev_weight_delta = 0.0f;
	float delta_w = 0.0f;
	std::shared_ptr<Neuron> from;
	std::shared_ptr<Neuron> to;
};

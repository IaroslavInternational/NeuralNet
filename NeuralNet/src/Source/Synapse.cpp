#include "../Include/Synapse.hpp"
#include "../Include/Neuron/Neuron.hpp"

#include <random>
#include <cassert>

#include <iostream> // !

Synapse::Synapse(std::shared_ptr<Neuron> from, std::shared_ptr<Neuron> to)
	:
	from(from),
	to(to)
{
	std::random_device rd;  
	std::mt19937 gen(rd()); 
	std::uniform_real_distribution<> distrib(-1.0, 1.0);

	weight = distrib(gen);
}

Synapse::Synapse(float w, std::shared_ptr<Neuron> from, std::shared_ptr<Neuron> to)
	:
	weight(w),
	from(from),
	to(to)
{
}

void Synapse::send() const
{
	assert(from.get() != nullptr);
	assert(to.get() != nullptr);

	to->get_input(from->get_output()*weight);
}

bool Synapse::valid() const
{
	return !(from == nullptr || to == nullptr);
}

std::shared_ptr<Neuron> Synapse::get_from()
{
	return from;
}

std::shared_ptr<Neuron> Synapse::get_to()
{
	return to;
}

void Synapse::set_weight(float weight)
{
	std::cout << "n w = " << weight << std::endl;
	this->weight = weight;
}

float Synapse::get_weight() const
{
	return weight;
}

void Synapse::update_weight(float delta, float alfa)
{
	float delta_w = delta + alfa * prev_weight_delta;
	weight += delta + alfa * prev_weight_delta;
	prev_weight_delta = delta_w;
}

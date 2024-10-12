#include "../Include/Synapse.hpp"
#include "../Include/Neuron/Neuron.hpp"

#include <random>
#include <cassert>

Synapse::Synapse(std::shared_ptr<Neuron> from, std::shared_ptr<Neuron> to)
	:
	from(from),
	to(to)
{
	std::random_device rd;  
	std::mt19937 gen(rd()); 
	std::uniform_real_distribution<> distrib(0.0, 1);

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

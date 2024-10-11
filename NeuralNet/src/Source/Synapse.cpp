#include "../Include/Synapse.hpp"
#include "../Include/Neuron.hpp"

#include <random>
#include <cassert>

Synapse::Synapse(InputNeuron* from, InputNeuron* to)
	:
	from(from),
	to(to)
{
	std::random_device rd;  
	std::mt19937 gen(rd()); 
	std::uniform_real_distribution<> distrib(0.0, 0.1);

	weight = distrib(gen);
}

Synapse::Synapse(float w, InputNeuron* from, InputNeuron* to)
	:
	weight(w),
	from(from),
	to(to)
{
}

void Synapse::send() const
{
	assert(from != nullptr);
	assert(to != nullptr);

	to->get_input(from->get_output()*weight);
}

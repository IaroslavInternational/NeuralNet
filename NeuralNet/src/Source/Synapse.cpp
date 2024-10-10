#include "../Include/Synapse.hpp"
#include "../Include/Neuron.hpp"

Synapse::Synapse(float w, InputNeuron* from, InputNeuron* to)
	:
	weight(w),
	from(from),
	to(to)
{
}

void Synapse::send() const
{
	to->get_input(from->get_output()*weight);
}

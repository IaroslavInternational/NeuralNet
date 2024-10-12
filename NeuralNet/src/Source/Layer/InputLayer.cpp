#include "../../Include/Layer/InputLayer.hpp"

InputLayer::InputLayer(size_t n, const std::string& name)
	:
	Layer(n, name)
{}

void InputLayer::set_input(const std::vector<int>& values)
{
	assert(neurons.size() == values.size());

	for (size_t i = 0; i < values.size(); i++)
	{
		neurons[i]->get_input(values[i]);
	}
}

void InputLayer::forward()
{
	for (auto& s : synapses)
	{
		s.send();
	}
}
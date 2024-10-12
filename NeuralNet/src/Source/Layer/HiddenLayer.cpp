#include "../../Include/Layer/HiddenLayer.hpp"

HiddenLayer::HiddenLayer(size_t n, const std::string& name)
	:
	Layer(n, name)
{}

void HiddenLayer::forward()
{
	for (auto& s : synapses)
	{
		s.send();
	}
}

void HiddenLayer::activate()
{
	for (auto& n : neurons)
	{
		n->activate();
	}
}
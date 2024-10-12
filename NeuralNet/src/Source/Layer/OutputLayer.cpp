#include "../../Include/Layer/OutputLayer.hpp"

OutputLayer::OutputLayer(size_t n, const std::string& name)
	:
	Layer(n, name)
{}

void OutputLayer::activate()
{
	for (auto& n : neurons)
	{
		n->activate();
	}
}

void OutputLayer::get_result()
{
	for (auto& n : neurons)
	{
		std::cout << n->get_output() << std::endl;
	}
}
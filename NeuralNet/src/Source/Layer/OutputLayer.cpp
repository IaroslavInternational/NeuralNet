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

std::vector<float>& OutputLayer::get_result()
{
	static std::vector<float> res;
	res.clear();

	for (auto& n : neurons)
	{
		res.push_back(n->get_output());
	}

	return res;
}
#include "../Include/NeuralNet.hpp"

NeuralNet::NeuralNet(size_t I_Layer_size, size_t H_Layer_num, size_t H_Layer_size, size_t O_Layer_size)
	:
	input_layer(I_Layer_size, "Input Layer"),
	output_layer(O_Layer_size, "Output Layer")
{
	for (size_t i = 0; i < H_Layer_num; i++)
	{
		hidden_layers.push_back(HiddenLayer(H_Layer_size, "Hidden Layer " + std::to_string(i)));
	}

	create_synapses(&input_layer, &hidden_layers[0]);

	for (size_t i = 0; i < hidden_layers.size() - 1; i++)
	{
		create_synapses(&hidden_layers[i], &hidden_layers[i + 1]);
	}

	create_synapses(&hidden_layers.back(), &output_layer);
}

void NeuralNet::train(const std::vector<float>& inputs, const std::vector<float>& expected)
{
	assert(inputs.size() == input_layer.length() && expected.size() == output_layer.length());

	// Прогоняем входы через нейросеть
	run(inputs);
	
	// Устанавливаем дельту для выходного слоя синапсов
	train_synapses = hidden_layers.back().get_synapses();
	for (size_t i = 0; i < result.size(); i++)
	{
		train_synapses->at(i).get_to()->set_delta(((expected[i] - result[i]) * ((1.0f - result[i]) * result[i])));
	}

	for (int i = hidden_layers.size() - 1; i >= 0; i--)
	{
		train_synapses = hidden_layers[i].get_synapses();
		for (size_t j = 0; j < train_synapses->size(); j++)
		{	
			update_synapse(&train_synapses->at(j));
		}
	}

	train_synapses = input_layer.get_synapses();
	for (size_t j = 0; j < train_synapses->size(); j++)
	{
		update_synapse(&train_synapses->at(j));
	}
}

void NeuralNet::run(const std::vector<float>& inputs)
{
	input_layer.set_input(inputs);
	input_layer.forward();

	for (auto& hl : hidden_layers)
	{
		hl.activate();
		hl.forward();
	}

	output_layer.activate();
	result = output_layer.get_result();
}

std::vector<float>& NeuralNet::get_result()
{
	return result;
}

// Refactor
void NeuralNet::set_weights(std::vector<float>* weights)
{
	input_layer.set_weights(weights);
	hidden_layers[0].set_weights(weights);
}

template<class T1, class T2>
void NeuralNet::create_synapses(Layer<T1>* l1, Layer<T2>* l2)
{
	size_t c = 0;

	for (size_t i = 0; i < l1->length(); i++)
	{
		for (size_t j = 0; j < l2->length(); j++)
		{
			l1->add_synapse(l1->get_neuron(i), l2->get_neuron(j));
		}
	}
}

void NeuralNet::update_synapse(Synapse* synapse)
{
	synapse->get_from()->set_delta(((1.0f - synapse->get_from()->get_output()) * synapse->get_from()->get_output()) *
		(synapse->get_weight() * synapse->get_to()->get_delta()));
	synapse->update_weight(E * synapse->get_from()->get_output() * synapse->get_to()->get_delta(), a);
}
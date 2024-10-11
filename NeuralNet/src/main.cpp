#include <iostream>
#include "Include/Layer.hpp"
#include "Include/OutputNeuron.hpp"

/* TO DO:
* Наследование class Layer
* class NeuralNet
* Функционал NeuralNet и Layer
*/

/*
* добавить assert`ы
* изменить указатели на shared_ptr
*/

template<class T1, class T2>
void create_synapses(Layer<T1>* l1, Layer<T2>* l2)
{
	size_t c = 0;

	for (size_t i = 0; i < l1->length(); i++)
	{
		for (size_t j = 0; j < l2->length(); j++)
		{
			l1->add_synapse(l1->get_neuron(i), l2->get_neuron(j));

			std::cout << "Synapse " << c << " between " << "N_out " << i << " and N_in " << j << std::endl;
			c++;
		}
	}
}

int main()
{
	Layer<InputNeuron> input_layer(0, "Input Layer");
	Layer<Neuron> hidden_layer(50, "Hidden Layer");
	Layer<OutputNeuron> output_layer(4, "Output Layer");

	create_synapses(&input_layer, &hidden_layer);
	std::cout << std::endl;
	create_synapses(&hidden_layer, &output_layer);

	input_layer.set_input({ 1, 0, 1, 0, 0, 0, 0, 0, 0, 1});
	input_layer.forward();

	hidden_layer.activate();
	hidden_layer.forward();

	output_layer.activate();
	output_layer.get_result();

	return 0;
}
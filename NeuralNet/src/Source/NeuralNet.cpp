#include "../Include/NeuralNet.hpp"

NeuralNet::NeuralNet(size_t I_Layer_size, size_t H_Layer_num, size_t H_Layer_size, size_t O_Layer_size)
	:
	input_layer(I_Layer_size, "Input Layer"),
	output_layer(O_Layer_size, "Output Layer")
{
	set_params();

	// Резервируем память для синапсов
	input_layer.get_synapses()->reserve(I_Layer_size * H_Layer_size);

	// Создаём скрытые слои
	hidden_layers.reserve(H_Layer_num);
	for (size_t i = 0; i < H_Layer_num; i++)
	{
		hidden_layers.push_back(HiddenLayer(H_Layer_size, "Hidden Layer " + std::to_string(i)));
	}

	// ====== Создаём синапсы ====== 

	// Синпасы между входным слоем и первым скрытым слоем
	create_synapses(&input_layer, &hidden_layers[0]);

	// Синпасы между скрытыми слоями
	for (size_t i = 0; i < hidden_layers.size() - 1; i++)
	{
		create_synapses(&hidden_layers[i], &hidden_layers[i + 1]);
	}

	// Синпасы между последним скрытым слоем и выходным слоем
	create_synapses(&hidden_layers.back(), &output_layer);
	
	// =============================
}

/* Public */


void NeuralNet::run(const Net::fvector& inputs)
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

void NeuralNet::train(const Net::fvector& inputs, const Net::fvector& expected)
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
	
	// Защита для обновления синапсов
	if (workers.size() > 0)
	{
		for (auto& w : workers)
		{
			w.get();
		}

		workers.clear();
	}

	// Обновляем веса синапсов для скрытых слоёв
	for (int i = (int)hidden_layers.size() - 1; i >= 0; i--)
	{
		train_synapses = hidden_layers[i].get_synapses();
		for (size_t j = 0; j < train_synapses->size(); j++)
		{	
			workers.push_back(std::async(std::launch::async, &NeuralNet::update_synapse, this, &train_synapses->at(j)));
		}
	}

	// Обновляем веса синапсов для первого слоя
	train_synapses = input_layer.get_synapses();
	for (size_t j = 0; j < train_synapses->size(); j++)
	{
		update_synapse(&train_synapses->at(j));
	}
}

void NeuralNet::set_weights(const Net::fmatrix& weights)
{
	for (size_t i = 0; i < weights.size(); i++)
	{
		if (i == 0)
		{
			input_layer.set_weights(weights[i]);
		}
		else
		{
			hidden_layers[i - 1].set_weights(weights[i]);
		}
	}
}

void NeuralNet::set_params(float E, float a)
{
	this->E = E;
	this->a = a;
}

std::vector<float>& NeuralNet::get_result()
{
	return result;
}

/* Private */


template<class T1, class T2>
void NeuralNet::create_synapses(Layer<T1>* l1, Layer<T2>* l2)
{
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
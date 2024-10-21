#include "../Include/NeuralNet.hpp"

//#define LOG(str) std::cout << str << std::endl;
#define LOG(str) //td::cout << str << std::endl;

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
	// Прогоняем входы через нейросеть
	run(inputs);
	error.clear();

	// Получаем ошибку
	for (size_t i = 0; i < result.size(); i++)
	{
		error.push_back(pow((expected[i] - result[i]), 2) / 1);
	}

	LOG("Res: ")
	for (auto& r : result)
	{
		LOG(r)
	}

	LOG("Err: ")
	for (auto& e : error)
	{
		LOG(e)
	}

	LOG("Iterations " + std::to_string(iterations))

	LOG("")

	delta_output.clear();

	LOG("Delta OUT:")
	// Считаем дельту для выходного слоя
	for (size_t i = 0; i < result.size(); i++)
	{
		delta_output.push_back(((expected[i] - result[i]) * ((1.0f - result[i]) * result[i])));
		LOG(delta_output[i])
	}

	// Устанавливаем дельту для выходного слоя синапсов
	train_synapses = hidden_layers.back().get_synapses();
	for (size_t i = 0; i < result.size(); i++)
	{
		for (size_t j = 0; j < train_synapses->size(); j++)
		{
			for (auto& s : *train_synapses)
			{
				s.get_to()->set_delta(((expected[i] - result[i]) * ((1.0f - result[i]) * result[i])));
			}
		}
	}
	
	LOG("")

	for (int i = hidden_layers.size() - 1; i >= 0; i--)
	{
		train_synapses = hidden_layers[i].get_synapses();
		for (size_t j = 0; j < train_synapses->size() - 1; j++)
		{	
			update_synapse(&train_synapses->at(j));
		}
	}

	LOG("")

	LOG("ADJ W from I to H:")
	while (!false)
	{
		auto syn = input_layer.get_next_synapse();

		if (syn == nullptr)
		{
			break;
		}

		LOG("Syn " + syn->get_from()->get_name())
		LOG("Old w: " + std::to_string(syn->get_weight()))

		auto n_out = syn->get_from()->get_output();
		auto w = syn->get_weight();
		auto d_out = syn->get_to()->get_delta();
		//auto n_delta = ((expected[0] - n_out) * n_out) * (w * d_out);
		//syn->get_from()->set_delta(n_delta);

		auto grad = n_out * d_out;

		syn->update_weight(E * grad, a);

		LOG("New w: " + std::to_string(syn->get_weight()))

	}

	iterations++;
}

std::vector<float>& NeuralNet::run(const std::vector<float>& inputs)
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

	return result;
}

std::vector<float>& NeuralNet::get_result()
{
	return result;
}

std::vector<float>& NeuralNet::get_error()
{
	return error;
}

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
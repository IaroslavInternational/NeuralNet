#include <iostream>
#include "Include/Neuron.hpp"

/*
* class Layer
* class NeuralNet
* Функционал NeuralNet и Layer
*/


int main()
{
    InputNeuron input;
    input.get_input(1);

    Neuron n1;

    Synapse s(0.5f, &input, &n1);
    
    s.send();

    n1.activate();
    auto res = n1.get_output();

    std::cout << "Hello World!\n";
}
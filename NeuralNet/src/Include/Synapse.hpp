#pragma once

class InputNeuron;

class Synapse
{
public:
	Synapse(InputNeuron* from, InputNeuron* to);
	Synapse(float w, InputNeuron* from, InputNeuron* to);
public:
	void send() const;
private:
	float weight;
	InputNeuron* from;
	InputNeuron* to;
};

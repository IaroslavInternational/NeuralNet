#pragma once

#include "../Synapse.hpp"

#include <string>

class Neuron
{
public:
	Neuron(const std::string& name);
public:
	virtual void get_input(float value);
	virtual float get_output() const;
public:
	void set_delta(float delta);
	float get_delta() const;
	const std::string& get_name() const;
protected:
	float input = 0.0f;
	float output = 0.0f;
	float delta = 0.0f;
	std::string id;
};


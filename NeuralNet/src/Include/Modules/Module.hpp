#pragma once

#include <string>

class Module
{
public:
	Module() {};
	virtual ~Module() {};
public:
	virtual void proc(const std::string& args) = 0;
	virtual bool is_finished() = 0;
};
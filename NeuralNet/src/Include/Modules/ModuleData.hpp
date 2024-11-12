#pragma once

#include "Module.hpp"

class ModuleData : public Module
{
public:
	ModuleData(const std::string& script_name);
public:
	void proc(const std::string& args) override;
	bool is_finished() override;
private:
	std::string script_name;
};
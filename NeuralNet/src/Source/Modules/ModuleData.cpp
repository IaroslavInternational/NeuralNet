#include "../../Include/Modules/ModuleData.hpp"

#include <sstream>
#include <cassert>
#include <fstream>

ModuleData::ModuleData(const std::string& script_name)
	:
	Module(),
	script_name(script_name)
{
}

void ModuleData::proc(const std::string& args)
{
	assert(args.size() != 0);

	std::ostringstream out;
	try
	{
		out << "python " << script_name << " " << args;
		system(out.str().c_str());
	}
	catch (const std::exception& ex)
	{	
		out.clear();
		out << "Python problem appears! " << ex.what();
		throw std::exception(out.str().c_str());
	}
}

bool ModuleData::is_finished()
{
	std::ifstream file("nni/proc_data.nni");
	return file.is_open();
}
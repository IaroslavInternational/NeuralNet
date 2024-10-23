#pragma once

#include <vector>
#include <map>
#include <string>

namespace Net
{
	using fmatrix = std::vector<std::vector<float>>;
	using train_set = std::pair<fmatrix, fmatrix>;

	std::vector<std::string> split(const std::string& str, char delimiter);
	train_set get_train_set(const std::string& path);
};

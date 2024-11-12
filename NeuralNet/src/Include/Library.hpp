#pragma once

#include <vector>
#include <map>
#include <string>

namespace Net
{
	using fmatrix = std::vector<std::vector<float>>;
	using fvector = std::vector<float>;
	using train_set = std::pair<fmatrix, fmatrix>;

	std::vector<std::string> split(const std::string& str, char delimiter);
	train_set				 get_train_set(const std::string& path);
	void				     shuffle_data(train_set& data);
	fvector				     load_input(const std::string& filename = "nni/input.nni");
};

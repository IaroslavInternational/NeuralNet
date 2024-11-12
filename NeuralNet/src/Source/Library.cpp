#include "../Include/Library.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <random>

namespace Net
{
	std::vector<std::string> split(const std::string& str, char delimiter) // refactor delimiter
	{
		std::string temp;
		std::vector<std::string> strings;
		std::stringstream ss(str);

		while (std::getline(ss, temp, ' '))
		{
			strings.push_back(temp);
		}
		
		return strings;
	}

	train_set get_train_set(const std::string& path)
	{
		train_set set;					  // first - входные данные, second - выходные
		char delimiter = ' ';			  // Разделитель данных
		std::vector<float> values;		  // Массив значений с текущей строки файла
		std::vector<std::string> strings; // Разделенная строка
		std::string current;
		std::ifstream file;

		std::vector<std::string> lines;
		lines.reserve(500);
		
		// DANGER ERROR PATH
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			file.open(entry.path().string());

			if (file.is_open())
			{
				lines.clear();

				while (std::getline(file, current))
				{
					lines.push_back(current);
				}

				file.close();
			}
			else
			{
				throw std::exception("Cannot read a file!");
			}

			lines.shrink_to_fit();

			// Заполнение сета
			if (lines.size() > 0)
			{
				// Цикл по всем строкам в файле
				for (size_t i = 0; i < lines.size(); i++)
				{
					// Первая строка - выходный массив
					if (i == 0)
					{
						strings = split(lines[i], delimiter);

						for (auto& s : strings)
						{
							values.push_back(std::stof(s));
						}

						set.second.push_back(values);
						values.clear();
					}
					else // Все остальные строки - входный массив
					{
						strings = split(lines[i], delimiter);

						for (auto& s : strings)
						{
							values.push_back(std::stof(s));
						}
					}
				}

				set.first.push_back(values);
				values.clear();
			}
			else
			{
				throw std::exception("Empty file!");
			}
		}

		return set;
	}

	void shuffle_data(train_set& data)
	{
		std::random_device rd;
		std::mt19937 g(rd());

		std::vector<int> idx;

		for (int i = 0; i < (int)data.first.size(); i++)
		{
			idx.push_back(i);
		}

		std::shuffle(idx.begin(), idx.end(), g);

		train_set shuffled_set;
		shuffled_set.first.resize(data.first.size());
		shuffled_set.second.resize(data.second.size());

		for (size_t i = 0; i < idx.size(); i++)
		{
			shuffled_set.first[i] = data.first[idx[i]];
			shuffled_set.second[i] = data.second[idx[i]];
		}

		data = std::move(shuffled_set);
	}

	fvector load_input(const std::string& filename)
	{
		fvector inputs;
		std::string current;
		std::vector<std::string> lines;
		std::vector<std::string> strings;
		lines.reserve(500);

		std::ifstream file(filename);

		if (file.is_open())
		{
			while (std::getline(file, current))
			{
				lines.push_back(current);
			}

			file.close();
			lines.shrink_to_fit();

			if (lines.size() > 0)
			{
				for (size_t i = 0; i < lines.size(); i++)
				{
					strings = split(lines[i], ' ');

					for (auto& s : strings)
					{
						inputs.push_back(std::stof(s));
					}
				}
			}
			else
			{
				throw std::exception("Empty file!");
			}
		}
		else
		{
			throw std::exception("Cannot read a input file!");
		}

		return inputs;
	}
}

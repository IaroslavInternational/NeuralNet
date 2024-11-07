#include "../Include/Library.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

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
}

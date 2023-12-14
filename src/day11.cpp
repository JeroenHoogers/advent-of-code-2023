#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace fs = std::filesystem;

struct Pos {
	int x;
	int y;
};

void print_map(const std::vector<std::string>& map) {

	for (const auto& row : map) {
		std::cout << row << std::endl;
	}

	std::cout << std::endl;
}

void expand_map(std::vector<std::string>& map) 
{
	int height = map.size();
	int width = map[0].size();

	for (int x = width - 1; x >= 0; x--)
	{
		bool is_empty = true;
		for (int y = height - 1; y >= 0; y--)
		{
			is_empty &= (map[y][x] == '.');
		}

		if (is_empty) 
		{
			for (int y = height - 1; y >= 0; y--)
			{
				map[y][x] = '%';
				//map[y].insert(map[y].begin() + x, '%');
			}

			//width++;
		}
	}

	for (int y = height - 1; y >= 0; y--)
	{
		if (map[y].find('#') == std::string::npos)
		{
			map[y] = std::string(width, '%');
			//map.insert(map.begin() + y, std::string(width,'%'));
			//height++;
		}
	}
}

void compute_path(std::vector<std::string>& map, std::vector<Pos> galaxies)
{
	int height = map.size();
	int width = map[0].size();
}

int main() {
	fs::path input_path("../data/day11_input.txt");
	//fs::path input_path("../data/day11_input_test.txt");

	std::ifstream file(input_path);
	
	std::vector<std::string> map;

	const int EXPANSION_FACTOR = 1000000;

	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line)) {
			map.push_back(line);
		}
	}
	file.close();

	std::cout << "Input:" << std::endl;
	print_map(map);

	expand_map(map);

	std::cout << "Expanded:" << std::endl;
	print_map(map);

	int height = map.size();
	int width = map[0].size();

	// find galaxies
	std::vector<Pos> galaxies;

	int boundaries_y = 0;
	int boundaries_x = 0;

	for (int y = 0; y < height; y++)
	{
		if (map[y][0] == '%') {
			boundaries_y++;
		}

		boundaries_x = 0;

		for (int x = 0; x < width; x++)
		{
			if (map[y][x] == '#') 
			{
				galaxies.push_back(
					Pos { 
						x + ((EXPANSION_FACTOR - 1) * boundaries_x), 
						y + ((EXPANSION_FACTOR - 1) * boundaries_y)
					}
				);
			} else if (map[y][x] == '%') {
				boundaries_x++;
			}
		}
	}

	uint64_t count = 0;
	for (size_t i = 0; i < galaxies.size(); i++)
	{
		for (size_t j = i + 1; j < galaxies.size(); j++)
		{
			uint64_t distance = std::abs(galaxies[i].x - galaxies[j].x) + std::abs(galaxies[i].y - galaxies[j].y);

			//std::cout << i << " (" << galaxies[i].x << ", " << galaxies[i].y << ") -- "
			//	<< j << " (" << galaxies[j].x << ", " << galaxies[j].y << ") \t = " << distance
			//	<< std::endl;

			count += distance;
		}
	}

	int result = 0;
	std::cout << "The answer is: " << count << std::endl;
	//std::cout << "The p2 answer is: " << inside_tiles << std::endl;

	return 0;
}
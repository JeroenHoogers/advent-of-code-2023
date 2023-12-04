#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>

namespace fs = std::filesystem;

static std::unordered_map<int, std::vector<int>> gears;

bool checkSymbolAdjacency(const std::vector<std::string>& schematic, int x, int y, const std::string& part_str) {
	int width = schematic[0].size();
	int height = schematic.size();

	for (int i = std::max(y - 1, 0); i < std::min(y + 2, height); i++)
	{
		for (int j = std::max(x - (int)part_str.size() - 1, 0); j < std::min(x + 1, width); j++)
		{
			char c2 = schematic[i][j];
			if (!isdigit(c2) && c2 != '.') // check if part is adjacent to symbol
			{
				if (c2 == '*') {

					int hash = i * height + j;

					if (gears.find(hash) == gears.end()) {
						gears[hash] = std::vector<int>{ stoi(part_str) };
					}
					else {
						gears[hash].push_back(stoi(part_str));
					}
				}

				return true;
			}
		}
	}

	return false;
}


int main() {
	fs::path input_path("../data/day3_input.txt");
	//fs::path input_path("../data/day3_input_test.txt");

	int part_id_sum = 0;
	int gear_ratio_sum = 0;

	std::ifstream file(input_path);

	if (file.is_open()) 
	{
		std::vector<std::string> schematic;

		std::string line;
		while (std::getline(file, line)) {
			schematic.push_back(line);
		}

		int width = schematic[0].size();
		int height = schematic.size();

		std::cout << "Checking schematic (w: " << width << ", h: " << height << ")" << std::endl;

		for (int y = 0; y < height; y++)
		{
			bool reading_partnum = false;
			std::string part_str = "";

			for (int x = 0; x < width; x++)
			{
				char c = schematic[y][x];

				if (isdigit(c)) {
					reading_partnum = true;
					part_str += c;
				}
				else if (reading_partnum)
				{
					int part = stoi(part_str);
					if (checkSymbolAdjacency(schematic, x, y, part_str)) 
					{	
						part_id_sum += part;
					}

					reading_partnum = false;
					part_str = "";
				}

				if (c == '.')
					continue;

			}

			// handle partId at end of line edge case
			if (reading_partnum) 
			{
				int part = stoi(part_str);
				if (checkSymbolAdjacency(schematic, width, y, part_str)) {
					part_id_sum += part;
				}
			}
		}

		for (auto& gear : gears) {
			if (gear.second.size() == 2) {
				gear_ratio_sum += gear.second[0] * gear.second[1];
			}
		}
	}
	
	std::cout << "The answer is: " << part_id_sum << std::endl;
	std::cout << "The gear ratio sum is: " << gear_ratio_sum << std::endl;

	return 0;
}
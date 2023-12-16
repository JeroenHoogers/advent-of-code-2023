#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <unordered_map>
#include <tuple>

namespace fs = std::filesystem;

struct Key {
	int load;
	int last;

	bool operator==(const Key& other) const {
		return load == other.load && last == other.last;
	}
};

namespace std {
	template<> struct hash<Key> {
		size_t operator()(Key const& key) const {
			return
				((hash<int>()(key.last) ^
				(hash<int>()(key.load) << 1)) >> 1);
		}
	};
}

void print_panel(const std::vector<std::string>& panel) {
	for (auto& row : panel) {
		std::cout << row << std::endl;
	}

	std::cout << std::endl;
}

void cycle(std::vector<std::string>& panel) 
{
	int w = panel[0].size();
	int h = panel.size();

	std::vector<int> ground(w);

	// Tilt North
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			if (panel[y][x] == 'O')
			{
				// move rock to nearest ground
				panel[y][x] = '.';
				panel[ground[x]][x] = 'O';
				ground[x]++;
			}
			else if (panel[y][x] == '#') {
				ground[x] = y + 1;
			}
		}
	}

	//std::cout << "tilted north: " << std::endl;
	//print_panel(panel);

	// Tilt West
	for (int y = 0; y < h; y++)
	{
		int height = 0;
		for (int x = 0; x < w; x++)
		{
			if (panel[y][x] == 'O')
			{
				// move rock to nearest ground
				panel[y][x] = '.';
				panel[y][height] = 'O';
				height++;
			}
			else if (panel[y][x] == '#') {
				height = x + 1;
			}
		}
	}

	//std::cout << "tilted west: " << std::endl;
	//print_panel(panel);

	// Tilt South
	std::fill(ground.begin(), ground.end(), panel.size() - 1);

	for (int y = h - 1; y >= 0; y--)
	{
		for (int x = 0; x < w; x++)
		{
			if (panel[y][x] == 'O')
			{
				// move rock to nearest ground
				panel[y][x] = '.';
				panel[ground[x]][x] = 'O';
				ground[x]--;
			}
			else if (panel[y][x] == '#') {
				ground[x] = y - 1;
			}
		}
	}

	//std::cout << "tilted south: " << std::endl;
	//print_panel(panel);

	// Tilt East
	for (int y = 0; y < h; y++)
	{
		int height = w - 1;
		for (int x = w - 1; x >= 0; x--)
		{
			if (panel[y][x] == 'O')
			{
				// move rock to nearest ground
				panel[y][x] = '.';
				panel[y][height] = 'O';
				height--;
			}
			else if (panel[y][x] == '#') {
				height = x - 1;
			}
		}
	}

	//std::cout << "tilted east: " << std::endl;
	//print_panel(panel);
}

void tilt_north(std::vector<std::string>& panel) {
	std::vector<int> ground(panel[0].size());

	for (size_t y = 0; y < panel.size(); y++)
	{
		for (size_t x = 0; x < panel[0].size(); x++)
		{
			if (panel[y][x] == 'O')
			{
				// move rock to nearest ground
				panel[y][x] = '.';
				panel[ground[x]][x] = 'O';
				ground[x]++;
			}
			else if (panel[y][x] == '#') {
				ground[x] = y + 1;
			}
		}
	}
}

int calculate_load(const std::vector<std::string>& panel) 
{
	int sum = 0;
	for (size_t y = 0; y < panel.size(); y++)
	{
		for (size_t x = 0; x < panel[0].size(); x++)
		{
			if (panel[y][x] == 'O')
			{
				sum += panel.size() - y;
			}
		}
	}

	return sum;
}

int main() {
	fs::path input_path("../data/day14_input.txt");
	//fs::path input_path("../data/day14_input_test.txt");

	std::ifstream file(input_path);

	int sum_p1 = 0;
	int sum_p2 = 0;

	if (file.is_open())
	{
		std::string line;

		std::vector<std::string> panel;

		while (std::getline(file, line)) 
		{
			panel.push_back(line);
		}

		std::cout << "Input: " << std::endl;
		print_panel(panel);

		// pt1
		// tilt_north(panel);
		// print_panel(panel);
		// sum_p1 = calculate_load(panel);

		// pt2
		std::unordered_map<Key, int> cycle_nums;

		const int CYCLES = 1000000000;

		int last = 0;
		int cycle_offset = 0;
		int cycle_size = 0;

		for (size_t i = 1; i <= CYCLES; i++)
		{
			cycle(panel);
			int load = calculate_load(panel);

			Key key = { load, last };
			auto match = cycle_nums.find(key);

			if (match != cycle_nums.end()) 
			{
				if (i - match->second == cycle_size) {
					cycle_offset = i;
					break;
				}

				cycle_size = i - match->second;
			}

			cycle_nums[key] = i;

			last = load;
		}

		std::unordered_map<int, int> loads;

		for (size_t i = 0; i < cycle_size; i++)
		{
			cycle(panel);
			loads[i] = calculate_load(panel);
		}

		int idx = (1000000000 - cycle_offset - 1) % cycle_size;
		sum_p2 = loads[idx];

	}
	file.close();

	//std::cout << "The p1 answer is: " << sum_p1 << std::endl;
	std::cout << "The p2 answer is: " << sum_p2 << std::endl;

	return 0;
}
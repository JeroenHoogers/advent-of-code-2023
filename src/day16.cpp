#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>

namespace fs = std::filesystem;

void print_map(const std::vector<std::string>& map) {
	for (const auto& row : map)
	{
		std::cout << row << std::endl;
	}
}

struct Coord {
	int x;
	int y;

	bool operator==(Coord other) {
		return x == other.x && y == other.y;
	}

	Coord operator+(Coord other) {
		return { x + other.x, y + other.y };
	}

	Coord operator+=(Coord other) {
		x += other.x;
		y += other.y;
		return *this;
	}
};

constexpr Coord UP = { 0, -1 };
constexpr Coord DOWN = { 0, 1 };
constexpr Coord RIGHT = { 1, 0 };
constexpr Coord LEFT = { -1, 0 };

struct Beam {
	Coord pos;
	Coord dir;
};

void reset_map(std::vector<std::string>& energized_map) {
	for (size_t i = 0; i < energized_map.size(); i++)
	{
		std::fill(energized_map[i].begin(), energized_map[i].end(), '.');
	}
}

int compute_energy(std::vector<std::string>& energized_map) {
	int sum = 0;
	for (size_t y = 0; y < energized_map.size(); y++)
	{
		for (size_t x = 0; x < energized_map[0].size(); x++)
		{
			if (energized_map[y][x] == '#') {
				sum++;
			}
		}
	}
	return sum;
}

void simulate_light(const std::vector<std::string>& map, std::vector<std::string>& energized_map, Coord origin, Coord direction) 
{
	const int w = map[0].size();
	const int h = map.size();

	// start in top left
	Coord pos = origin;
	Coord dir = direction;

	while(true) 
	{
		// terminate when beam is out of bounds
		if (pos.x < 0 || pos.x >= w ||
			pos.y < 0 || pos.y >= h) {
			return;
		}

		char c = map[pos.y][pos.x];
		bool is_energized = energized_map[pos.y][pos.x] == '#';

		if (c == '/') {
			if (dir == UP) {
				dir = RIGHT;
			}
			else if (dir == DOWN) {
				dir = LEFT;
			}
			else if (dir == LEFT) {
				dir = DOWN;
			}
			else if (dir == RIGHT) {
				dir = UP;
			}
		}
		if (c == '\\') {
			if (dir == UP) {
				dir = LEFT;
			}
			else if (dir == DOWN) {
				dir = RIGHT;
			}
			else if (dir == LEFT) {
				dir = UP;
			}
			else if (dir == RIGHT) {
				dir = DOWN;
			}
		}
		if (c == '|' && dir.x != 0) {
			if (is_energized)
				return;

			// split beam vertically
			energized_map[pos.y][pos.x] = '#';
			simulate_light(map, energized_map, pos + UP, UP);
			simulate_light(map, energized_map, pos + DOWN, DOWN);
			return;
		}
		if (c == '-' && dir.y != 0) {
			if (is_energized)
				return;

			// split beam horizontally
			energized_map[pos.y][pos.x] = '#';
			simulate_light(map, energized_map, pos + LEFT, LEFT);
			simulate_light(map, energized_map, pos + RIGHT, RIGHT);
			return;
		}

		energized_map[pos.y][pos.x] = '#';
		
		pos += dir;
	}
}


int find_maximum(const std::vector<std::string>& map, std::vector<std::string>& energized_map) 
{
	int max = 0;

	const int w = map[0].size();
	const int h = map.size();

	for (int y = 0; y < h; y++)
	{
		reset_map(energized_map);
		simulate_light(map, energized_map, { 0, y }, RIGHT);
		int result = compute_energy(energized_map);
		max = std::max(max, result);
	}

	for (int y = 0; y < h; y++)
	{
		reset_map(energized_map);
		simulate_light(map, energized_map, { w - 1, y }, LEFT);
		int result = compute_energy(energized_map);
		max = std::max(max, result);
	}

	for (int x = 0; x < w; x++)
	{
		reset_map(energized_map);
		simulate_light(map, energized_map, { x, 0 }, DOWN);
		int result = compute_energy(energized_map);
		max = std::max(max, result);
	}

	for (int x = 0; x < w; x++)
	{
		reset_map(energized_map);
		simulate_light(map, energized_map, { x, h - 1 }, UP);
		int result = compute_energy(energized_map);
		max = std::max(max, result);
	}

	return max;
}

int main() {
	//fs::path input_path("../data/day16_input.txt");
	fs::path input_path("../data/day16_input_test.txt");

	std::ifstream file(input_path);

	int sum_p1 = 0;
	int sum_p2 = 0;

	if (file.is_open())
	{
		std::vector<std::string> map;
		std::vector<std::string> energized_map;

		std::string line;
		while (std::getline(file, line))
		{
			map.push_back(line);
			//energized_map.push_back(line);
			energized_map.push_back(std::string(line.size(), '.'));
		}

		simulate_light(map, energized_map, {0, 0}, RIGHT);

		sum_p1 = compute_energy(energized_map);
		sum_p2 = find_maximum(map, energized_map);

		print_map(map);
		std::cout << std::endl;
		print_map(energized_map);
	}
	file.close();

	std::cout << "The p1 answer is: " << sum_p1 << std::endl;
	std::cout << "The p2 answer is: " << sum_p2 << std::endl;

	return 0;
}
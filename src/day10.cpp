#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <array>
#include <cassert>

namespace fs = std::filesystem;

void print_map(const std::vector<std::string>& map) {

	for (const auto& row : map) {
		std::cout << row << std::endl;
	}

	std::cout << std::endl;
}

struct Position {
	int x;
	int y;
};

Position FindNextStep(const std::vector<std::string>& map, const Position& c, const Position& p) 
{
	int width = map[0].size();
	int height = map.size();

	bool top = c.y > 0				&& c.y - 1 != p.y; 
	bool bot = c.y < height - 1		&& c.y + 1 != p.y;
	bool left = c.x > 0				&& c.x - 1 != p.x;
	bool right = c.x < width - 1	&& c.x + 1 != p.x;

	char curr = map[c.y][c.x];

	top		&= (curr == '|') || (curr == 'J') || (curr == 'L') || (curr == 'S');
	bot		&= (curr == '|') || (curr == 'F') || (curr == '7') || (curr == 'S');
	left	&= (curr == '-') || (curr == 'J') || (curr == '7') || (curr == 'S');
	right	&= (curr == '-') || (curr == 'F') || (curr == 'L') || (curr == 'S');

	// DEBUG
	//print_map(map);

	if (top && (map[c.y - 1][c.x] == 'S' || map[c.y - 1][c.x] == '|' || map[c.y - 1][c.x] == '7' || map[c.y - 1][c.x] == 'F')) {
		return Position{ c.x, c.y - 1 };
	}

	if (left && (map[c.y][c.x - 1] == 'S' || map[c.y][c.x - 1] == '-' || map[c.y][c.x - 1] == 'L' || map[c.y][c.x - 1] == 'F')) {
		return Position{ c.x - 1, c.y };
	}

	if (bot && (map[c.y + 1][c.x] == 'S' || map[c.y + 1][c.x] == '|' || map[c.y + 1][c.x] == 'J' || map[c.y + 1][c.x] == 'L')) {
		return Position{ c.x, c.y + 1 };
	}

	if (right && (map[c.y][c.x + 1] == 'S' || map[c.y][c.x + 1] == '-' || map[c.y][c.x + 1] == '7' || map[c.y][c.x + 1] == 'J')) {
		return Position{ c.x + 1, c.y };
	}

	assert(false && "Path interrupted!");

	// check neighbours
	//char top = map[c.y - 1][c.x];
	//char bottom = map[c.y + 1][c.x];
	//char left = map[c.y][c.x - 1];
	//char right = map[c.y][c.x + 1];
}


void FloodFill(std::vector<std::string>& map, const Position& c) {
	int width = map[0].size();
	int height = map.size();

	if (c.y < 0 || c.y >= height || c.x < 0 || c.x >= width)
		return;

	if (map[c.y][c.x] == 'O' || map[c.y][c.x] == '*')
		return;

	bool top = c.y > 0;
	bool bot = c.y < height - 1;
	bool left = c.x > 0;
	bool right = c.x < width - 1;

	map[c.y][c.x] = 'O';

	if (top) {
		FloodFill(map, Position{ c.x, c.y - 1 });
	}

	if (bot) {
		FloodFill(map, Position{ c.x, c.y + 1 });
	}

	if (left) {
		FloodFill(map, Position{ c.x - 1, c.y });
	}

	if (right) {
		FloodFill(map, Position{ c.x + 1, c.y });
	}
}

int main() {
	fs::path input_path("../data/day10_input.txt");
	//fs::path input_path("../data/day10_input_test.txt");
	//fs::path input_path("../data/day10_input_test2.txt");
	//fs::path input_path("../data/day10_input_test3.txt");
	//fs::path input_path("../data/day10_input_test4.txt");
	//fs::path input_path("../data/day10_input_test5.txt");

	std::ifstream file(input_path);

	//std::vector<std::string> orignal;
	std::vector<std::string> map;
	
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line)) {
			map.push_back(line);
		}
	}
	file.close();
	
	std::cout << "Input: " << std::endl;
	print_map(map);

	int width = map[0].size();
	int height = map.size();

	Position start{};
	
	// find start
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (map[y][x] == 'S') {
				start = Position{ x, y };
			}
		}
	}

	Position prev = start;
	Position p = FindNextStep(map, start, start);

	int steps = 1;
	
	std::vector<Position> path;
	path.push_back(start);
	while (map[p.y][p.x] != 'S')
	{
		path.push_back(p);
		Position next = FindNextStep(map, p, prev);
		prev = p;
		p = next;

		map[prev.y][prev.x] = '*';

		steps++;
	}

	map[start.y][start.x] = '*';

	// DEBUG
	print_map(map);

	size_t result = steps / 2;

	// Part 2
	for (size_t i = 1; i < path.size(); i++)
	{
		const auto& prev = path[i - 1];
		const auto& curr = path[i];

		// up
		if (prev.y > curr.y) {
			FloodFill(map, Position{ prev.x - 1, prev.y });
			FloodFill(map, Position{ curr.x - 1, curr.y });
		}
		// down
		else if (prev.y < curr.y) {
			FloodFill(map, Position{ prev.x + 1, prev.y });
			FloodFill(map, Position{ curr.x + 1, curr.y });
		}
		// left
		else if (prev.x > curr.x) {
			FloodFill(map, Position{ prev.x, prev.y + 1 });
			FloodFill(map, Position{ curr.x, curr.y + 1 });
		}
		// right
		else if (prev.x < curr.x) {
			FloodFill(map, Position{ prev.x, prev.y - 1 });
			FloodFill(map, Position{ curr.x, curr.y - 1 });
		}
	}

	// DEBUG
	print_map(map);

	int inside_tiles = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (map[y][x] != '*' && map[y][x] != 'O') {

				inside_tiles++;
			}
		}
	}

	std::cout << "The answer is: " << result << std::endl;
	std::cout << "The p2 answer is: " << inside_tiles << std::endl;

	return 0;
}
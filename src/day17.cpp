#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_set>

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

	bool operator==(const Coord other) const {
		return x == other.x && y == other.y;
	}

	bool operator!=(const Coord other) const {
		return x != other.x || y != other.y;
	}

	Coord operator+(const Coord other) const {
		return { x + other.x, y + other.y };
	}

	Coord operator-(const Coord other) const {
		return { x - other.x, y - other.y };
	}

	Coord operator*(int scalar) const {
		return { x * scalar, y * scalar };
	}

	void normalize() {
		if (x != 0) {
			x /= abs(x);
		}
		if (y != 0) {
			y /= abs(y);
		}
	}

	Coord operator+=(const Coord other) {
		x += other.x;
		y += other.y;
		return *this;
	}
};

struct State
{
	Coord coord;
	int dist;
	bool vertical;

	bool operator==(const State other) const {
		return coord == other.coord && vertical == other.vertical;
	}
};

namespace std {
	template<> struct hash<Coord> {
		size_t operator()(Coord const& coord) const {
			return
				((hash<int>()(coord.x) ^
					(hash<int>()(coord.y) << 1)) >> 1);
		}
	};

	template<> struct hash<State> {
		size_t operator()(State const& state) const {
			return
				((hash<Coord>()(state.coord) ^
					(hash<bool>()(state.vertical) << 1)) >> 1);
		}
	};
}

constexpr Coord UNDEFINED = { -1, -1 };
constexpr Coord UP = { 0, -1 };
constexpr Coord DOWN = { 0, 1 };
constexpr Coord RIGHT = { 1, 0 };
constexpr Coord LEFT = { -1, 0 };

constexpr Coord DIRS[4] {
	UP,
	DOWN,
	RIGHT,
	LEFT
};

int calculate_min_path(std::vector<std::string>& map, int min_steps, int max_steps)
{
	int h = map.size();
	int w = map[0].size();

	std::vector<std::vector<Coord>> prev_nodes;

	//for (int i = 0; i < map.size(); i++)
	//{
	//	prev_nodes.push_back(std::vector<Coord>(map[i].size()));
	//}

	auto comp = [](const State& s1, const State& s2) -> bool
	{
		return s1.dist > s2.dist;
	};

	std::priority_queue<State, std::vector<State>, decltype(comp)> q(comp);
	std::unordered_set<State> visited;

	prev_nodes[0][0] = UNDEFINED;

	q.push({ 0, 0, 0, true });
	q.push({ 0, 0, 0, false });

	int min_distance = 0;

	Coord end = { w - 1, h - 1 };

	while (!q.empty())
	{
		State u = q.top();
		q.pop();

		if (u.coord == end) {
			return  u.dist;
		}

		if (visited.find(u) != visited.end()) {
			continue;
		}

		visited.insert(u);

		for (const Coord& dir : DIRS) 
		{
			// allow only 90 deg turns
			bool vert = dir == UP || dir == DOWN;
			if (u.vertical == vert) {
				continue;
			}

			int dist = u.dist; // min_distances[u.y][u.x];

			for (size_t i = 1; i <= max_steps; i++)
			{
				Coord v = u.coord + dir * i;

				if (v.y < 0 || v.y >= h || v.x < 0 || v.x >= w) {
					break;
				}

				dist += (map[v.y][v.x] - '0');
				if (i >= min_steps)
				{
					q.push({ v, dist, vert });
				}
			}
		}
	}
	return 0;
}

int main() {
	fs::path input_path("../data/day17_input.txt");
	//fs::path input_path("../data/day17_input_test.txt");

	std::ifstream file(input_path);

	int sum_p1 = 0;
	int sum_p2 = 0;

	if (file.is_open())
	{
		std::vector<std::string> map;

		std::string line;
		while (std::getline(file, line))
		{
			map.push_back(line);
		}
		
		print_map(map);

		sum_p1 = calculate_min_path(map, 1, 3);
		sum_p2 = calculate_min_path(map, 4, 10);

		std::cout << std::endl;
		print_map(map);
	}
	file.close();

	std::cout << "The p1 answer is: " << sum_p1 << std::endl;
	std::cout << "The p2 answer is: " << sum_p2 << std::endl;

	return 0;
}
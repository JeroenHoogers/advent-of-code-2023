#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>
#include <stack>
#include <algorithm>

namespace fs = std::filesystem;

int find_reflection(const std::vector<std::string>& pattern, int smudges = 0) 
{
	if (smudges == 0) {
		for (auto row : pattern) {
			std::cout << row << std::endl;
		}
		std::cout << std::endl;
	}

	// scan for columns
	auto row_distance = [&pattern](int r1, int r2)
	{
		int d = 0;
		for (size_t i = 0; i < pattern[0].size(); i++)
		{
			if (pattern[r1][i] != pattern[r2][i]) {
				d++;
			}
		}

		return d;
	};

	// scan for 2 the same rows 
	for (int i = 1; i < pattern.size(); i++)
	{
		int d = row_distance(i, i - 1);
		if (d <= smudges)
		{
			bool is_symmetric = true;
			for (size_t j = 1; j < i && j < pattern.size() - i; j++)
			{
				d += row_distance(i + j, i - j - 1);
				if (d > smudges) {
					is_symmetric = false;
					break;
				}
			}

			// check if this is symmetric, except for exactly 1 smudge
			if (is_symmetric && d == smudges) {
				return i * 100;
			}
		}
	}

	// scan for columns
	auto column_distance = [&pattern](int c1, int c2)
	{
		int d = 0;
		for (size_t i = 0; i < pattern.size(); i++)
		{
			if (pattern[i][c1] != pattern[i][c2]) {
				d++;
			}
		}

		return d;
	};

	for (int i = 1; i < pattern[0].size(); i++)
	{
		int d = column_distance(i, i - 1);
		if (d <= smudges)
		{
			bool is_symmetric = true;
			for (size_t j = 1; j < i && j < pattern[0].size() - i; j++)
			{
				d += column_distance(i + j, i - j - 1);
				if (d > smudges) {
					is_symmetric = false;
					break;
				}
			}

			// check if this is symmetric, except for exactly 1 smudge
			if (is_symmetric && d == smudges) {
				return i;
			}
		}
	}

	return 0;
}

int main() {
	fs::path input_path("../data/day13_input.txt");
	//fs::path input_path("../data/day13_input_test.txt");

	std::ifstream file(input_path);

	int sum_p1 = 0;
	int sum_p2 = 0;

	if (file.is_open())
	{
		std::string line;

		std::vector<std::string> pattern;

		while (std::getline(file, line)) 
		{
			if (line != "") {
				pattern.push_back(line);
			}
			else 
			{
				sum_p1 += find_reflection(pattern);
				sum_p2 += find_reflection(pattern, 1);
				pattern.clear();
			}
		}

		sum_p1 += find_reflection(pattern);
		sum_p2 += find_reflection(pattern, 1);

		std::vector<int> sequence;
	}
	file.close();

	std::cout << "The p1 answer is: " << sum_p1 << std::endl;
	std::cout << "The p2 answer is: " << sum_p2 << std::endl;

	return 0;
}
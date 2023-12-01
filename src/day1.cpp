#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>

namespace fs = std::filesystem;

int parse_digit(const std::string& s, int i) 
{
	static std::unordered_map<std::string, int> values {
		{"one",		1},
		{"two",		2},
		{"three",	3},
		{"four",	4},
		{"five",	5},
		{"six",		6},
		{"seven",	7},
		{"eight",	8},
		{"nine",	9},
	};

	for (auto number : values)
	{
		if (s.substr(i, number.first.size()) == number.first) {
			return number.second;
		}
	}
	
	return -1;
}

int main() {
	fs::path input_path("../data/day1_input.txt");
	//fs::path input_path("../data/day1_p2_input_test.txt");

	std::string line;
	std::ifstream file(input_path);

	int sum = 0;
	if (file.is_open()) {
		while (std::getline(file, line)) {
			int first = -1;
			int second = -1;
			for (int i = 0; i < line.size(); i++) {
				int digit = -1;

				if (isdigit(line[i])) {
					digit = line[i] - '0';
				} else {
					digit = parse_digit(line, i);
				}

				if (digit >= 0) {
					second = digit;
					if (first < 0) {
						first = digit;
					}
				}
			}

			int num = first * 10 + second;
			sum += num;
		}
		file.close();
	}

	std::cout << "The answer is: " << sum << std::endl;

	return 0;
}
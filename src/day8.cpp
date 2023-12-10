#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <array>

namespace fs = std::filesystem;

struct Node
{
	std::string label;
	std::string left;
	std::string right;
};

int main() {
	fs::path input_path("../data/day8_input.txt");
	//fs::path input_path("../data/day8_input_test.txt");
	//fs::path input_path("../data/day8_input_test2.txt");
	//fs::path input_path("../data/day8_input_test_pt2.txt");

	std::unordered_map<std::string, Node> nodes;
	std::vector<std::string> paths;

	std::ifstream file(input_path);

	std::string instructions;

	if (file.is_open())
	{
		std::string line;
		std::getline(file, line);
		instructions = line;

		std::cout << instructions << std::endl;

		std::getline(file, line);

		// parse times
		while (std::getline(file, line))
		{
			Node node{};
			node.label = line.substr(0, 3);
			node.left = line.substr(7, 3);
			node.right = line.substr(12, 3);

			if (node.label[2] == 'A') {
				std::cout << node.label << std::endl;
				paths.push_back(node.label);
			}

			nodes[node.label] = node;
			//std::cout << node.label << " --> (" << node.left << ", " << node.right << ")" << std::endl;

		}
	}
	file.close();
	
	//std::cout << std::endl;

	// part 1
	size_t steps = 0;

	std::string current = "AAA";
	while (current != "ZZZ")
	{
		char instruction = instructions[steps % instructions.size()];

		std::string next = (instruction == 'L') ? nodes[current].left : nodes[current].right;

		//std::cout << current << " --" << instruction << "--> " << next << std::endl;

		current = next;

		steps++;
	}

	size_t p1_result = steps;

	std::cout << "instructions: " << instructions.size() << std::endl;

	// part 2
	std::vector<size_t> path_cycles;
	path_cycles.resize(paths.size());

	for (size_t i = 0; i < paths.size(); i++)
	{
		std::string& current = paths[i];

		for (size_t s = 0; ; s++)
		{
			char instruction = instructions[s % instructions.size()];
			current = (instruction == 'L') ? nodes[current].left : nodes[current].right;

			if (current[2] == 'Z') {
				path_cycles[i] = s + 1;
				break;
			}
		}
	}

	steps = 0;
	bool done = 0;
	for (size_t i = path_cycles[0]; ; i += path_cycles[0])
	{
		int completed_paths = 0;
		for (const auto& cycle : path_cycles)
		{
			if (i % cycle == 0)
			{
				completed_paths++;
			}
		}

		if (completed_paths == paths.size()) {
			steps = i;
			break;
		}
	}

	size_t p2_result = steps;

	std::cout << "The p1 answer is: " << p1_result << std::endl;
	std::cout << "The p2 answer is: " << p2_result << std::endl;

	return 0;
}
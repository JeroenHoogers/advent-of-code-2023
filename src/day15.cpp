#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

uint8_t hash(const std::string& instruction) {
	uint8_t current = 0;
	for (auto& c : instruction) {
		current += c;
		current *= 17;
		//current %= 256;
	}

	return current;
}

struct Lens {
	std::string label;
	int focal_length;

	bool operator==(const Lens& r) const
	{
		return label == r.label;
	}
};

int main() {
	fs::path input_path("../data/day15_input.txt");
	//fs::path input_path("../data/day15_input_test.txt");

	std::ifstream file(input_path);

	int sum_p1 = 0;
	int sum_p2 = 0;

	if (file.is_open())
	{
		std::vector<std::vector<Lens>> boxes(256);

		std::string instruction;
		while (std::getline(file, instruction, ','))
		{
			//std::cout << instruction;
			int p1_val = hash(instruction);
			//std::cout << " = " << p1_val << std::endl;
			sum_p1 += p1_val;

			std::string label;

			// parse instruction
			for(int i = 0; i < instruction.size(); i++)
			{
				char c = instruction[i];
				if (c == '=' || c == '-') {
					int idx = hash(label);
					
					auto match = std::find_if(boxes[idx].begin(), boxes[idx].end(), 
						[&label](const Lens& l) -> bool { 
							return label == l.label; 
						}
					);

					if (match != boxes[idx].end()) {
						if (c == '=') {
							int focal_length = instruction[i + 1] - '0';
							(*match).focal_length = focal_length;
						}
						else if (c == '-') {
							boxes[idx].erase(match);
						}
					}
					else if (c == '=') {
						int focal_length = instruction[i + 1] - '0';
						boxes[idx].push_back({ label, focal_length });
					}	
					break;
				}

				label += c;
			}
		}

		// Calculate p2 sum
		for (size_t i = 0; i < boxes.size(); i++)
		{
			if (boxes[i].size() == 0) {
				continue;
			}

			std::cout << " Box " << i << ": ";
			for (size_t j = 0; j < boxes[i].size(); j++) {
				int focus_power = (i + 1) * (j + 1) * boxes[i][j].focal_length;
				std::cout << "[" << boxes[i][j].label << " " << boxes[i][j].focal_length << "] " ;
				sum_p2 += focus_power;
			}
			std::cout << std::endl;
		}
	}
	file.close();

	std::cout << "The p1 answer is: " << sum_p1 << std::endl;
	std::cout << "The p2 answer is: " << sum_p2 << std::endl;

	return 0;
}
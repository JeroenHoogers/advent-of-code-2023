#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <array>

namespace fs = std::filesystem;
int main() {
	fs::path input_path("../data/day9_input.txt");
	//fs::path input_path("../data/day9_input_test.txt");

	std::vector<std::string> paths;

	std::ifstream file(input_path);

	int sum = 0;

	if (file.is_open())
	{
		std::string line;

		// parse times
		while (std::getline(file, line))
		{
			std::vector<std::vector<int>> sequences;
			sequences.push_back({});

			std::stringstream line_ss(line);
			std::string number;

			while (std::getline(line_ss, number, ' '))
			{
				sequences[0].push_back(stoi(number));
			}

			// enable for part 2
			std::reverse(sequences[0].begin(), sequences[0].end());
			
			bool all_zeroes = false;
			int depth = 1;

			while (!all_zeroes)
			{
				sequences.push_back({});

				bool only_zeroes = true;
				for (size_t i = 1; i < sequences[depth-1].size(); i++)
				{
					int delta = sequences[depth-1][i] - sequences[depth-1][i-1];

					sequences[depth].push_back(delta);

					only_zeroes &= (delta == 0);
				}

				all_zeroes = only_zeroes;

				depth++;
			}

			// DEBUG
			for (const auto& sequence : sequences)
			{
				for (const auto& num : sequence)
				{
					std::cout << num << " ";
				}

				std::cout << std::endl;
			}


			// compute sum of last numbers
			int last_sum = 0;
			for (const auto& sequence : sequences)
			{
				last_sum += *(sequence.end()-1);
			}

			sum += last_sum;
			std::cout << "= " << last_sum << std::endl << std::endl;
		}
	}
	file.close();
	
	//std::cout << std::endl;

	// part 1
	size_t result = sum;

	std::cout << "The answer is: " << sum << std::endl;
	//std::cout << "The p2 answer is: " << p2_result << std::endl;

	return 0;
}
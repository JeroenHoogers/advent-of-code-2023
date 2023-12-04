#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <unordered_map>

namespace fs = std::filesystem;


int count_number_cards(const std::vector<int>& counts, int idx) {
	int count = 1;

	for (size_t i = idx + 1; i < std::min(idx + counts[idx] + 1, static_cast<int>(counts.size())); i++)
	{
		count += count_number_cards(counts, i);
	}
	return count;
}

int main() {
	fs::path input_path("../data/day4_input.txt");
	//fs::path input_path("../data/day4_input_test.txt");

	std::ifstream file(input_path);

	std::vector<int> winning_number_counts;

	int winning_sum = 0;

	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			std::set<int> winning_numbers;
			int winning_number_count = 0;

			int colon_idx = std::find(line.begin(), line.end(), ':') - line.begin();
			int bar_idx = std::find(line.begin(), line.end(), '|') - line.begin();

			std::stringstream winning_nums_ss(line.substr(colon_idx + 1, bar_idx - colon_idx - 1));
			std::string num;
			while (std::getline(winning_nums_ss, num, ' ')) {
				if (num.size() == 0)
					continue;

				std::cout << num << " ";
				winning_numbers.insert(stoi(num));
			}

			std::cout << "| ";
			std::stringstream card_nums_ss(line.substr(bar_idx + 1));

			while (std::getline(card_nums_ss, num, ' ')) {
				if (num.size() == 0)
					continue;

				if (winning_numbers.find(stoi(num)) != winning_numbers.end())
				{
					std::cout << "*";
					winning_number_count++;
				}

				std::cout << num << " ";
			}

			int score = pow(2, winning_number_count - 1);

			std::cout << " winning numbers: " << winning_number_count << ", score: " << score << std::endl;

			winning_number_counts.push_back(winning_number_count);

			winning_sum += score;
		}
	}

	// calculate total number of cards
	int total_cards = 0;
	for (int i = 0; i < winning_number_counts.size(); i++) {
		total_cards += count_number_cards(winning_number_counts, i);
	}

	std::cout << "The answer is: " << winning_sum << std::endl;
	std::cout << "The total number of cards is: " << total_cards << std::endl;

	return 0;
}
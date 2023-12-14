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

bool check_validity(const std::string& pattern, const std::vector<int>& sequence) 
{
	int j = 0;
	int broken = 0;
	for (auto& c : pattern)
	{
		if (c == '#')
		{
			broken++;
		}
		else if (broken > 0) {
			if (broken != sequence[j]) {
				return false;
			}
			broken = 0;
			j++;
		}
	}

	if (j < sequence.size() - 1 && sequence[j] != broken) {
		return false;
	}

	return true;
}

void expand(std::string& pattern, std::vector<int>& sequence) {
	const std::string original = pattern;

	pattern = original + '?' + original + '?' + original + '?' + original + '?' + original;
	int size = sequence.size();

	for (size_t i = 0; i < 4; i++)
	{
		sequence.insert(sequence.end(), sequence.begin(), sequence.begin() + size);
	}
}

inline int calculate_options(const std::string& pattern, const std::vector<int>& sequence) {
	std::vector<int> broken;
	std::vector<int> wildcards;

	for (size_t i = 0; i < pattern.size(); i++)
	{
		if (pattern[i] == '?') {
			wildcards.push_back(i);
		}
		else if (pattern[i] == '#') {
			broken.push_back(i);
		}
	}

	int broken_sum = std::accumulate(sequence.begin(), sequence.end(), 0);

	int valid_substitutions = 0;
	
	int last_pos = 0;

	// iterate over all possible combinations to distribute n broken hotsprings over N wildcards
	int n = broken_sum - broken.size();
	int m = wildcards.size();
	
	std::cout << pattern << " wildcards: " << m << " broken hotsprings to place: " << n << " sequence: ";
	for (auto x : sequence)
		std::cout << x << ", ";

	std::cout << std::endl;

	std::vector<int> selected_wildcards(n);
	for (size_t i = 0; i < n; i++) {
		selected_wildcards[i] = i;
	}

	while (true)
	{
		std::string substituted = pattern;
		for (int idx : selected_wildcards)
		{
			substituted[wildcards[idx]] = '#';
		}

		if (check_validity(substituted, sequence)) {
			valid_substitutions++;
		}

		// std::cout << substituted << " valid subs: " << valid_substitutions << std::endl;

		bool combinations_exausted = true;

		// get next combination
		for (int i = n - 1; i >= 0; i--)
		{
			if (selected_wildcards[i] < m - (n - i)) {
				selected_wildcards[i]++;
				for (size_t j = i+1; j < n; j++)
				{
					selected_wildcards[j] = selected_wildcards[j - 1] + 1;
				}
				combinations_exausted = false;
				break;
			}
		}

		if (combinations_exausted) {
			break;
		}
	}

	std::cout << "valid arrangements: " << valid_substitutions << std::endl << std::endl;

	return valid_substitutions;
}

int main() {
	//fs::path input_path("../data/day12_input.txt");
	fs::path input_path("../data/day12_input_test.txt");

	std::ifstream file(input_path);

	int sum = 0;

	if (file.is_open())
	{
		std::string line;

		while (std::getline(file, line)) 
		{
			std::vector<int> sequence;
			int separator = line.find(' ');

			std::string pattern = line.substr(0, separator);

			std::stringstream sequence_ss(line.substr(separator+1));
			std::string num;
			while (std::getline(sequence_ss, num, ',')) {
				sequence.push_back(stoi(num));
			}

			std::cout << pattern << " -> ";
			for (auto x : sequence)
				std::cout << x << ", ";
			std::cout << std::endl;

			//expand(pattern, sequence);

			//std::cout << pattern << " -> ";
			//for (auto x : sequence)
			//	std::cout << x << ", ";
			//std::cout << std::endl;

			sum += calculate_options(pattern, sequence);
		}

		std::vector<int> sequence;
	}
	file.close();

	std::cout << "The answer is: " << sum << std::endl;
	//std::cout << "The p2 answer is: " << inside_tiles << std::endl;

	return 0;
}
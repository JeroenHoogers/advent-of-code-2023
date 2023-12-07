#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

namespace fs = std::filesystem;

inline int determine_rank(const std::string& hand)
{
	std::unordered_map<char, int> card_counts;
	card_counts['J'] = 0;

	for (const auto& character : hand) {
		if (card_counts.find(character) != card_counts.end()) {
			card_counts[character]++;
		}
		else {
			card_counts[character] = 1;
		}
	}

	int jokers = card_counts['J'];

	int highest = 0;
	int second_highest = 0;

	for (const auto& counts : card_counts)
	{
		if (counts.first == 'J')
			continue;

		if (counts.second >= highest)
		{
			second_highest = highest;
			highest = counts.second;
		} 
		else if(counts.second >= second_highest)
		{
			second_highest = counts.second;
		}
	}

	// 4-5 of a kind
	if (highest + jokers >= 4) {
		return highest + jokers + 2;
	}

	// full house
	if (highest + second_highest + jokers == 5) {
		return 5;
	}

	// three of a kind
	if (highest + jokers == 3) {
		return 4;
	}

	// two pair
	if (highest + second_highest + jokers == 4)
		return 3;

	// pair & high card
	return highest + jokers;
}

std::unordered_map<char, int> card_values {
	{ 'A', 14 },
	{ 'K', 13 },
	{ 'Q', 12 },
	{ 'J', 1 },
	{ 'T', 10 },
	{ '9', 9 },
	{ '8', 8 },
	{ '7', 7 },
	{ '6', 6 },
	{ '5', 5 },
	{ '4', 4 },
	{ '3', 3 },
	{ '2', 2 }
};

struct Bid {
	std::string hand;
	int bid;

	bool operator < (const Bid& other) const
	{
		int rank_this = determine_rank(hand);
		int rank_other = determine_rank(other.hand);

		if (rank_this != rank_other) {
			return rank_this < rank_other;
		}

		// check highest digit
		for (size_t i = 0; i < 5; i++)
		{
			if (card_values[hand[i]] != card_values[other.hand[i]]) {
				return card_values[hand[i]] < card_values[other.hand[i]];
			}
		}
	}
};


int main() {
	fs::path input_path("../data/day7_input.txt");
	//fs::path input_path("../data/day7_input_test.txt");

	std::ifstream file(input_path);

	std::vector<Bid> bids;

	if (file.is_open())
	{
		std::string line;

		// parse times
		while (std::getline(file, line))
		{				
			Bid bid = {
				line.substr(0, 5),
				stoi(line.substr(6))
			};

			bids.push_back(bid);

			//std::cout << bid.hand << ", " << bid.bid << std::endl;
		}

		std::sort(bids.begin(), bids.end());

		// DEBUG
		for (const auto& bid : bids) {

			std::cout << bid.hand << ", " << bid.bid << "\t rank: " << determine_rank(bid.hand) << std::endl;
		}
	}
	file.close();

	uint64_t p1_result = 0;

	for (int i = 0; i < bids.size(); i++)
	{
		p1_result += bids[i].bid * (i + 1);
	}


	// 250714296 <
	// 250202718

	// 250606082
	std::cout << "The p1 answer is: " << p1_result << std::endl;
	//std::cout << "The p2 answer is: " << p2_result << std::endl;

	return 0;
}
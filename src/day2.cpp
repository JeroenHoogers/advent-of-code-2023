#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>

namespace fs = std::filesystem;


int main() {
	fs::path input_path("../data/day2_input.txt");
	//fs::path input_path("../data/day2_input_test.txt");

	std::string line;
	std::ifstream file(input_path);

	std::unordered_map<std::string, int> input_cubes {
		{"red", 12},
		{"green", 13},
		{"blue", 14}
	};

	std::unordered_map<std::string, int> max_counts {
		{"red", 0},
		{"green", 0},
		{"blue", 0}
	};

	int possible_game_id_sum = 0;
	int cube_power_sum = 0;

	if (file.is_open()) {
		int game_id = 0;
		while (std::getline(file, line)) {

			max_counts["red"] = 0;
			max_counts["green"] = 0;
			max_counts["blue"] = 0;
			game_id++;

			// skip to game content
			int i = 0;
			while (line[i] != ':')
				i++;
			
			i++;

			// parse game
			std::stringstream games_ss(line.substr(i));

			std::string game;
			while (std::getline(games_ss, game, ';')) {
				//std::cout << "game: " << game << std::endl;
				std::stringstream game_ss(game);
				std::string pick;

				while (std::getline(game_ss, pick, ',')) {
					//std::cout << "pick: " << pick << std::endl;

					int j;

					for (j = 0; j < pick.size(); j++)
					{
						if (pick[j] != ' ' && !isdigit(pick[j])) {
							break;
						}
					}
					
					int count = stoi(pick.substr(0, j));
					std::string color = pick.substr(j);
					max_counts[color] = std::max(count, max_counts[color]);

					//std::cout << "color: " << color << " count: " << count << " max: " << max_counts[color] << std::endl;
				}
			}

			// check if this game is possible
			bool possible = true;
			int power = 1;
			for (auto input : input_cubes)
			{
				power *= max_counts[input.first];

				if (max_counts[input.first] > input.second) {
					possible = false;
				}
			}

			cube_power_sum += power;

			if (possible) {
				possible_game_id_sum += game_id;
			}

			std::cout << "Game " << game_id << " possible: " << possible << std::endl;
		}
		file.close();
	}

	std::cout << "The answer is: " << possible_game_id_sum << std::endl;
	std::cout << "The sum of minimum set powers is: " << cube_power_sum << std::endl;

	return 0;
}
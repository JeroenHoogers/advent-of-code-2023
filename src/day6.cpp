#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

uint64_t CalculateWaysToBeat(uint64_t time, uint64_t distance)
{
	uint64_t ways_to_beat = 0;
	for (uint64_t t = 0; t < time; t++) {
		uint64_t dist = (time - t) * t;
		if (dist > distance)
			ways_to_beat++;
	}

	std::cout << "time: " << time << ", distance: " << distance << " ways to beat: " << ways_to_beat << std::endl;

	return ways_to_beat;
}

int main() {
	//fs::path input_path("../data/day6_input.txt");
	fs::path input_path("../data/day6_input_test.txt");

	std::ifstream file(input_path);

	std::vector<int> times;
	std::vector<int> distances;

	uint64_t total_time = 0;
	uint64_t total_distance = 0;

	if (file.is_open())
	{
		std::string line;

		const int DATA_OFFSET = 10;

		// parse times
		std::getline(file, line);

		std::stringstream ss(line.substr(DATA_OFFSET));
		std::string full_num = "";
		std::string num;
		while (std::getline(ss, num, ' '))
		{
			if (num == "") {
				continue;
			}

			times.push_back(stoull(num));
			full_num += num;
		}

		total_time = stoi(full_num);
		full_num = "";

		// parse distances
		std::getline(file, line);

		ss = std::stringstream(line.substr(DATA_OFFSET));
		while (std::getline(ss, num, ' '))
		{
			if (num == "") {
				continue;
			}

			distances.push_back(stoi(num));
			full_num += num;
		}

		total_distance = stoull(full_num);
	}
	file.close();

	uint64_t p1_result = 1;

	for (size_t i = 0; i < times.size(); i++)
	{
		p1_result *= CalculateWaysToBeat(times[i], distances[i]);
	}

	std::cout << "total time: " << total_time << " total distance: " << total_distance << std::endl;

	uint64_t p2_result = CalculateWaysToBeat(total_time, total_distance);
	
	std::cout << "The p1 answer is: " << p1_result << std::endl;
	std::cout << "The p2 answer is: " << p2_result << std::endl;

	return 0;
}
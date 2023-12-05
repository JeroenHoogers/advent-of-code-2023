#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <unordered_map>

namespace fs = std::filesystem;


enum class MapType : int
{
	None					= 0,
	SeedToSoil				= 1,
	SoilToFertilizer		= 2,
	FertilizerToWater		= 3,
	WaterToLight			= 4,
	LightToTemperature		= 5,
	TemperatureToHumidity	= 6,
	HumidityToLocation		= 7
};

struct Range {
	uint64_t src_start;
	uint64_t dst_start;
	uint64_t len;
};

inline uint64_t SeedToLocation(std::unordered_map<MapType, std::vector<Range>>& range_maps, uint64_t seed)
{
	MapType maptype = MapType::SeedToSoil;

	uint64_t input = seed;

	//std::cout << input;

	// apply all mappings
	for (int i = 1; i < 8; i++)
	{
		maptype = (MapType)i;
		auto ranges = range_maps[maptype];

		for (const auto& range : ranges) {
			if (range.src_start <= input && input < range.src_start + range.len) {
				input = range.dst_start + (input - range.src_start);
				break;
			}
		}

	//	std::cout << " --> " << input;
	}

	//std::cout << std::endl;

	return input;
}

int main() {
	//fs::path input_path("../data/day5_input.txt");
	fs::path input_path("../data/day5_input_test.txt");

	std::ifstream file(input_path);

	std::vector<uint64_t> seeds;
	std::unordered_map<MapType, std::vector<Range>> mappings;

	if (file.is_open())
	{
		MapType current_map = MapType::None;

		std::string line;
		while (std::getline(file, line))
		{
			if (line == "") {
				continue;
			}

			if (line.substr(0, 7) == "seeds: ") {
				std::stringstream seeds_ss(line.substr(7));
				std::string seed_str;
				while (std::getline(seeds_ss, seed_str, ' '))
				{
					seeds.push_back(stoul(seed_str));
				}
				continue;
			}

			if (line == "seed-to-soil map:")					{ current_map = MapType::SeedToSoil;			continue; }
			else if (line == "soil-to-fertilizer map:")			{ current_map = MapType::SoilToFertilizer;		continue; }
			else if (line == "fertilizer-to-water map:")		{ current_map = MapType::FertilizerToWater;		continue; }
			else if (line == "water-to-light map:")				{ current_map = MapType::WaterToLight;			continue; }
			else if (line == "light-to-temperature map:")		{ current_map = MapType::LightToTemperature;	continue; }
			else if (line == "temperature-to-humidity map:")	{ current_map = MapType::TemperatureToHumidity; continue; }
			else if (line == "humidity-to-location map:")		{ current_map = MapType::HumidityToLocation;	continue; }

			std::stringstream range_ss(line);
			std::string dst_range, src_range, len;

			std::getline(range_ss, dst_range, ' ');
			std::getline(range_ss, src_range, ' ');
			std::getline(range_ss, len, ' ');
			Range range { stoul(src_range), stoul(dst_range), stoul(len) };

			std::cout << line << " [" << (int)current_map << "] (" << range.dst_start << ", " << range.src_start << ", " << range.len << ")" << std::endl;

			mappings[current_map].push_back(range);
		}
	}

	uint64_t lowest_location_p1 = ULONG_MAX;

	for (auto seed : seeds)
	{
		uint64_t location = SeedToLocation(mappings, seed);

		lowest_location_p1 = std::min(lowest_location_p1, location);
	}

	uint64_t lowest_location_p2 = ULONG_MAX;

	for (size_t i = 0; i < seeds.size(); i += 2)
	{
		for (uint64_t j = seeds[i]; j < seeds[i] + seeds[i+1]; j++)
		{
			uint64_t location = SeedToLocation(mappings, j);//seed_to_location(j);

			lowest_location_p2 = std::min(lowest_location_p2, location);
		}

		std::cout << (i + 2) / 2 << "/" << seeds.size() / 2 << std::endl;
	}

	std::cout << "The p1 answer is: " << lowest_location_p1 << std::endl;
	std::cout << "The p2 answer is: " << lowest_location_p2 << std::endl;

	return 0;
}
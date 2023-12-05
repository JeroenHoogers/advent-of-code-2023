#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <stack>

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

struct Mapping {
	uint64_t src;
	uint64_t dst;
	uint64_t len;
};

typedef std::tuple<uint64_t, uint64_t> Range;

inline uint64_t SeedToLocation(std::unordered_map<MapType, std::vector<Mapping>>& range_maps, uint64_t seed)
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
			if (range.src <= input && input < range.src + range.len) {
				input = range.dst + (input - range.src);
				break;
			}
		}

	//	std::cout << " --> " << input;
	}

	//std::cout << std::endl;

	return input;
}

inline uint64_t SeedRangeToLocation(std::unordered_map<MapType, std::vector<Mapping>>& range_maps, Range seed_range)
{
	MapType maptype = MapType::SeedToSoil;

	std::vector<Range> ranges;
	ranges.push_back(seed_range);

	//// DEBUG ONLY
	//const auto& [s, e] = seed_range;
	//std::cout << "(" << s << ", " << e << ")";

	// apply all mappings
	for (int i = 1; i < 8; i++)
	{
		maptype = (MapType)i;
		auto mappings = range_maps[maptype];

		std::vector<Range> remapped;

		for (const auto& mapping : mappings) 
		{
			uint64_t src_end = mapping.src + mapping.len;

			std::vector<Range> new_ranges;

			while (!ranges.empty())
			{
				const auto [start, end] = ranges.back();
				ranges.pop_back();

				Range before(start, std::min(end, mapping.src));
				Range intersection(std::max(mapping.src, start), std::min(src_end, end));
				Range after(std::max(src_end, start), end);

				if (std::get<1>(before) > std::get<0>(before)) {
					new_ranges.push_back(before);
				}
				if (std::get<1>(intersection) > std::get<0>(intersection)) {
					remapped.push_back(Range(mapping.dst + (std::get<0>(intersection) - mapping.src), mapping.dst + (std::get<1>(intersection) - mapping.src)));
				}
				if (std::get<1>(after) > std::get<0>(after)) {
					new_ranges.push_back(after);
				}
			}

			ranges = new_ranges;
		}

		ranges.insert(ranges.end(), remapped.begin(), remapped.end());

		//// DEBUG ONLY
		//std::cout << " --> ";
		//for (const auto& location_ranges : ranges)
		//{
		//	const auto [start, end] = location_ranges;
		//	std::cout << "(" << start << ", " << end << ")";
		//}
		//std::cout << std::endl;
	}

	//std::cout << "results for (" << std::get<0>(seed_range) << ", " << std::get<1>(seed_range) << "): " << std::endl;

	uint64_t min_location = ULONG_MAX;
	for (const auto& location_ranges : ranges)
	{
		const auto [start, end] = location_ranges;
		//std::cout << "(" << start << ", " << end << ")" << std::endl;
		min_location = std::min(start, min_location);
	}
	return min_location;
}

int main() {
	fs::path input_path("../data/day5_input.txt");
	//fs::path input_path("../data/day5_input_test.txt");

	std::ifstream file(input_path);

	std::vector<uint64_t> seeds;
	std::unordered_map<MapType, std::vector<Mapping>> mappings;

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
			Mapping mapping { stoul(src_range), stoul(dst_range), stoul(len) };

			//std::cout << line << " [" << (int)current_map << "] (" << mapping.dst << ", " << mapping.src << ", " << mapping.len << ")" << std::endl;

			mappings[current_map].push_back(mapping);
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
		auto range = std::tuple<uint64_t, uint64_t>(seeds[i], seeds[i] + seeds[i+1]);

		uint64_t location = SeedRangeToLocation(mappings, range);
		lowest_location_p2 = std::min(lowest_location_p2, location);

		//for (uint64_t j = seeds[i]; j < seeds[i] + seeds[i+1]; j++)
		//{
		//	uint64_t location = SeedToLocation(mappings, j);

		//	lowest_location_p2 = std::min(lowest_location_p2, location);
		//}

		//std::cout << (i + 2) / 2 << "/" << seeds.size() / 2 << std::endl;
	}

	std::cout << "The p1 answer is: " << lowest_location_p1 << std::endl;
	std::cout << "The p2 answer is: " << lowest_location_p2 << std::endl;

	return 0;
}
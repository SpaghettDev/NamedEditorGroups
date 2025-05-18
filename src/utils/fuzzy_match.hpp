#pragma once

#include <string>
#include <array>
#include <utility>

#include <Geode/external/fts/fts_fuzzy_match.h>

namespace ng::utils::fuzzy_match
{
	// taken from geode search mods feature
	bool weightedFuzzyMatch(const std::string&, const std::string&, double, double&);
	bool weightedFuzzyMatch(const std::string&, const std::string&, double, double&, std::array<std::uint8_t, 256>&);

	bool matchesQuery(const std::string&, std::pair<std::string, short>);
	bool matchesQuery(const std::string&, std::pair<std::string, short>, std::array<std::uint8_t, 256>&);
}

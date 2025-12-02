#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include <Geode/external/fts/fts_fuzzy_match.h>

#include "fuzzy_match.hpp"

bool ng::utils::fuzzy_match::weightedFuzzyMatch(const std::string& str, const std::string& kw, double weight, double& out)
{
	int score;

	if (fts::fuzzy_match(kw.c_str(), str.c_str(), score))
	{
		out = std::max(out, score * weight);
		return true;
	}

	return false;
}

bool ng::utils::fuzzy_match::weightedFuzzyMatch(
	const std::string& str, const std::string& kw,
	double weight, double& out,
	std::array<std::uint8_t, 256>& matches
) {
	int score;

	if (fts::fuzzy_match(kw.c_str(), str.c_str(), score, matches.data(), matches.size()))
	{
		out = std::max(out, score * weight);
		return true;
	}

	return false;
}

bool ng::utils::fuzzy_match::matchesQuery(const std::string& query, std::pair<std::string, short> itemData)
{
	bool doesMatch = false;
	double weighted = 0;

	doesMatch |= weightedFuzzyMatch(itemData.first, query, .5, weighted);
	doesMatch |= weightedFuzzyMatch(fmt::format("{}", itemData.second), query, 1.0, weighted);

	if (weighted < 50 + 5 * query.size())
		doesMatch = false;

	return doesMatch;
}

bool ng::utils::fuzzy_match::matchesQuery(
	const std::string& query, std::pair<std::string, short> itemData,
	std::array<std::uint8_t, 256>& indices
) {
	bool doesMatch = false;
	double weighted = 0;

	doesMatch |= weightedFuzzyMatch(itemData.first, query, .5, weighted, indices);
	doesMatch |= weightedFuzzyMatch(fmt::format("{}", itemData.second), query, 1.0, weighted);

	if (weighted < 50 + 5 * query.size())
		doesMatch = false;

	return doesMatch;
}

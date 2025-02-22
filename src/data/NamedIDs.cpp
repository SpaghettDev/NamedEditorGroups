#include "NamedIDs.hpp"

#include <string_view>
#include <string>

#include <ranges>

std::string NamedID::dump() const
{
	return fmt::format("{}:{}", name, id);
}

NamedID NamedID::from(std::string_view str)
{
	if (auto separatorIdx = str.find(':'); separatorIdx != std::string_view::npos)
	{
		return {
			str.substr(0, separatorIdx),
			geode::utils::numFromString<short>(str.substr(separatorIdx + 1)).unwrapOr(-1)
		};
	}

	return {};
}


std::string NamedIDs::dump() const
{
	auto v = std::views::transform(namedIDs, [](const auto& ng) {
		return fmt::format("{}:{}", ng.first, ng.second);
	});
	return fmt::format("{}", fmt::join(v, "#"));
}

NamedIDs NamedIDs::from(std::string_view str)
{
	NamedIDs res;

	std::size_t posStart = 0, posEnd;

	while ((posEnd = str.find('#', posStart)) != std::string_view::npos)
	{
		auto ngSeparatorIdx = str.find(':', posStart);

		res.namedIDs[
			std::string{ str.substr(posStart, ngSeparatorIdx - posStart) }
		] = geode::utils::numFromString<short>(
			str.substr(ngSeparatorIdx + 1, posEnd - ngSeparatorIdx - 1)
		).unwrapOr(-1);

		posStart = posEnd + 1;
	}

	if (posStart < str.size())
	{
		auto ngSeparatorIdx = str.find(':', posStart);

		res.namedIDs[
			std::string{ str.substr(posStart, ngSeparatorIdx - posStart) }
		] = geode::utils::numFromString<short>(
			str.substr(ngSeparatorIdx + 1)
		).unwrapOr(-1);
	}

	return std::move(res);
}

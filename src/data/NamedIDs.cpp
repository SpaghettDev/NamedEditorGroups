#include "NamedIDs.hpp"

#include <string_view>
#include <string>
#include <ranges>
#include <utility>

#include "utils.hpp"

std::string NamedID::dump() const
{
	return fmt::format("{}:{}", name, id);
}

geode::Result<NamedID> NamedID::from(std::string_view str)
{
	if (auto separatorIdx = str.find(':'); separatorIdx != std::string_view::npos)
	{
		auto id = geode::utils::numFromString<short>(str.substr(separatorIdx + 1));

		if (id.isErr())
			return geode::Err("Malformed ID");

		return geode::Ok(NamedID{ str.substr(0, separatorIdx), id.unwrap() });
	}

	return geode::Err("Malformed Named ID");
}


std::string NamedIDs::dump() const
{
	auto v = std::views::transform(namedIDs, [](const auto& ng) {
		return fmt::format("{}:{}", ng.first, ng.second);
	});
	return fmt::format("{}", fmt::join(v, "#"));
}

geode::Result<NamedIDs> NamedIDs::from(std::string_view str)
{
	NamedIDs res{};
	auto parseStr = [](std::string_view str, std::size_t posStart, std::size_t posEnd, bool isEnd = false) -> geode::Result<std::pair<std::string, short>> {
		auto ngSeparatorIdx = str.find(':', posStart);

		if (ngSeparatorIdx == std::string_view::npos || ngSeparatorIdx > posEnd)
			return geode::Err("Invalid NamedIDs: Missing or misplaced ':' separator near position {}", posStart);

		auto name = std::string{ str.substr(posStart, ngSeparatorIdx - posStart) };
		auto id = str.substr(ngSeparatorIdx + 1, isEnd ? std::string_view::npos : posEnd - ngSeparatorIdx - 1);

		if (auto sanitizeRes = ng::utils::sanitizeName(name); sanitizeRes.isErr())
			return geode::Err("Failed to parse name '{}': {}", name, sanitizeRes.unwrapErr());

		auto idNumRes = geode::utils::numFromString<short>(id);

		if (idNumRes.isErr())
			return geode::Err("Failed to parse number '{}': {}", id, idNumRes.unwrapErr());

		auto idNum = idNumRes.unwrap();
#ifndef NID_DEBUG_BUILD
		if (idNum < 1)
			return geode::Err("Invalid ID: '{}' out of range", idNum);
#endif

		return geode::Ok(std::pair<std::string, short>{ std::move(name), idNum });
	};

	std::size_t posStart = 0, posEnd;

	while ((posEnd = str.find('#', posStart)) != std::string_view::npos)
	{
		if (auto parseRes = parseStr(str, posStart, posEnd); parseRes.isErr())
			return geode::Err(parseRes.unwrapErr());
		else
		{
			auto&& parseResUnwrapped = parseRes.unwrap();

			res.namedIDs[std::move(parseResUnwrapped.first)] = parseResUnwrapped.second;
		}

		posStart = posEnd + 1;
	}

	if (posStart < str.size())
	{
		if (auto parseRes = parseStr(str, posStart, posEnd, true); parseRes.isErr())
			return geode::Err(parseRes.unwrapErr());
		else
		{
			auto&& parseResUnwrapped = parseRes.unwrap();

			res.namedIDs[std::move(parseResUnwrapped.first)] = parseResUnwrapped.second;
		}

		auto ngSeparatorIdx = str.find(':', posStart);
	}

	return geode::Ok(std::move(res));
}

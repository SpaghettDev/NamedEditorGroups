#pragma once

#include <string_view>
#include <unordered_map>

#include <Geode/utils/StringMap.hpp>
#include <Geode/Result.hpp>

struct NamedID
{
	std::string_view name;
	short id;

	std::string dump() const;
	static geode::Result<NamedID> from(std::string_view);
};

struct NamedIDs
{
	std::unordered_map<std::string, short, geode::utils::StringHash, std::equal_to<>> namedIDs;

	// unsafe
	short& operator[](const std::string_view name) { return namedIDs.find(name)->second; }
	// unsafe
	const short& operator[](const std::string_view name) const { return namedIDs.find(name)->second; }

	std::string dump() const;
	static geode::Result<NamedIDs> from(std::string_view);
};

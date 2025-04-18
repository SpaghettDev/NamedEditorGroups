#pragma once

#include <string_view>
#include <unordered_map>

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
	std::unordered_map<std::string, short> namedIDs;

	short& operator[](const std::string& name) { return namedIDs.at(name); }
	const short& operator[](const std::string& name) const { return namedIDs.at(name); }

	std::string dump() const;
	static geode::Result<NamedIDs> from(std::string_view);
};

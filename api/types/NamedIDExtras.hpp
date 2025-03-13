#pragma once

#include <string>

#pragma pack(push, 1)
struct NamedIDExtra
{
	// increments by 1 for regular changes
	// increments by 10 when the reserved space is not enough to hold new data
	static constexpr std::uint16_t VERSION = 1;

	std::string description;
	bool isPreviewed = true;

	[[deprecated("This field is reserved for future use")]] std::uint64_t _reserved[5]{};
};
#pragma pack(pop)

struct NamedIDsExtras
{
	std::unordered_map<short, NamedIDExtra> extras;

	NamedIDExtra& operator[](short id) { return extras.at(id); }
	const NamedIDExtra& operator[](short id) const { return extras.at(id); }
};

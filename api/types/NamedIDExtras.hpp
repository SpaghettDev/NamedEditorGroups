#pragma once

#include <string>
#include <unordered_map>

static_assert(true); // clangd bug fix

#pragma pack(push, 1)
struct NamedIDExtra
{
	// increments by 1 for regular changes
	// increments by 10 when the reserved space is not enough to hold new data
	static constexpr std::uint16_t VERSION = 1;

	NamedIDExtra(std::string&& description, bool isPreviewed)
		: description(static_cast<std::string&&>(description)), isPreviewed(isPreviewed)
	{};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
	NamedIDExtra() = default;
	NamedIDExtra(const NamedIDExtra&) = default;
	NamedIDExtra(NamedIDExtra&&) = default;
	NamedIDExtra& operator=(const NamedIDExtra&) = default;
	NamedIDExtra& operator=(NamedIDExtra&&) = default;
#pragma clang diagnostic pop

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

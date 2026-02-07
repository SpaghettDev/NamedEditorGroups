#define GEODE_DEFINE_EVENT_EXPORTS
#include <NIDManager.hpp>

#include "NamedIDs.hpp"

#include "events/NewNamedIDEvent.hpp"
#include "events/RemovedNamedIDEvent.hpp"

#include "utils.hpp"

static bool g_isDirty;
static NamedIDs g_namedGroups;
static NamedIDs g_namedCollisions;
static NamedIDs g_namedCounters;
static NamedIDs g_namedTimers;
static NamedIDs g_namedEffects;
static NamedIDs g_namedColors;

geode::Result<NamedIDs&> containerForNID(NID id)
{
	switch (id)
	{
		case NID::GROUP:
			return geode::Ok(g_namedGroups);

		case NID::COLLISION:
			return geode::Ok(g_namedCollisions);

		case NID::DYNAMIC_COUNTER_TIMER: [[fallthrough]];
		case NID::COUNTER:
			return geode::Ok(g_namedCounters);

		case NID::TIMER:
			return geode::Ok(g_namedTimers);

		case NID::EFFECT:
			return geode::Ok(g_namedEffects);

		case NID::COLOR:
			return geode::Ok(g_namedColors);

		default:
			return geode::Err("Invalid NID enum value");
	}
}


geode::Result<std::string> NIDManager::getNameForID(NID nid, short id)
{
	const auto& ids = GEODE_UNWRAP(containerForNID(nid));

	auto it = std::ranges::find_if(
		ids.namedIDs.begin(),
		ids.namedIDs.end(),
		[id](const auto& p) { return p.second == id; }
	);

	if (it == ids.namedIDs.end())
		return geode::Err("ID {} has no name associated to it", id);

	return geode::Ok(it->first);
}

geode::Result<short> NIDManager::getIDForName(NID nid, std::string_view name)
{
	const auto& ids = GEODE_UNWRAP(containerForNID(nid));

	if (!ids.namedIDs.contains(name))
		return geode::Err("Name {} has no ID associated to it", name);

	return geode::Ok(ids[name]);
}

geode::Result<> NIDManager::saveNamedID(NID nid, std::string_view name, short id)
{
#ifndef NID_DEBUG_BUILD
	if (id <= 0)
		return geode::Err("Invalid ID!");
#endif // !NID_DEBUG_BUILD

	if (auto sanitizeRes = ng::utils::sanitizeName(name); sanitizeRes.isErr())
		return sanitizeRes;

	auto idsRes = containerForNID(nid);
	if (idsRes.isErr())
		return geode::Err(idsRes.unwrapErr());
	auto& ids = idsRes.unwrap();

	if (auto idName = getNameForID(nid, id); idName.isOk())
		ids.namedIDs.erase(idName.unwrap());

	ids.namedIDs[std::string{ name }] = id;

	g_isDirty = true;
	NewNamedIDEvent().send(nid, name, id);

	return geode::Ok();
}

geode::Result<> NIDManager::removeNamedID(NID nid, std::string_view name)
{
	auto idsRes = containerForNID(nid);
	if (idsRes.isErr())
		return geode::Err(idsRes.unwrapErr());
	auto& ids = idsRes.unwrap();

	if (!ids.namedIDs.contains(name))
		return geode::Err("No saved Named ID {}", name);

	// what an outdated ass ndk does to a mf
#ifdef GEODE_WINDOWS
	ids.namedIDs.erase(name);
#else
	ids.namedIDs.erase(ids.namedIDs.find(name));
#endif

	g_isDirty = true;
	RemovedNamedIDEvent().send(nid, name, ids[name]);

	return geode::Ok();
}

geode::Result<> NIDManager::removeNamedID(NID nid, short id)
{
	auto idsRes = containerForNID(nid);
	if (idsRes.isErr())
		return geode::Err(idsRes.unwrapErr());
	auto& ids = idsRes.unwrap();
	auto&& name = getNameForID(nid, id);

	if (name.isErr())
		return geode::Err(name.unwrapErr());

	ids.namedIDs.erase(name.unwrap());

	g_isDirty = true;
	RemovedNamedIDEvent().send(nid, name.unwrap(), id);

	return geode::Ok();
}

geode::Result<const std::unordered_map<std::string, short, geode::utils::StringHash, std::equal_to<>>&> NIDManager::getNamedIDs(NID nid)
{
	auto idsRes = containerForNID(nid);

	if (idsRes.isErr())
		return geode::Err(idsRes.unwrapErr());

	return geode::Ok(idsRes.unwrap().namedIDs);
}

// internal


bool NIDManager::isDirty() { return g_isDirty; }

bool NIDManager::isEmpty()
{
	return (
		g_namedGroups.namedIDs.empty() &&
		g_namedCollisions.namedIDs.empty() &&
		g_namedCounters.namedIDs.empty() &&
		g_namedTimers.namedIDs.empty() &&
		g_namedEffects.namedIDs.empty() &&
		g_namedColors.namedIDs.empty()
	);
}

std::string NIDManager::dumpNamedIDs()
{
	return fmt::format(
		"{}|{}|{}|{}|{}|{}",
		g_namedGroups.dump(),
		g_namedCollisions.dump(),
		g_namedCounters.dump(),
		g_namedTimers.dump(),
		g_namedEffects.dump(),
		g_namedColors.dump()
	);
}

geode::Result<> NIDManager::importNamedIDs(const std::string& str, bool setDirty)
{
	auto parseStr = [](NamedIDs& namedIDs, std::string_view& str, const std::string_view name) -> geode::Result<> {
		if (auto namedIDsRes = NamedIDs::from(str))
		{
			namedIDs = std::move(namedIDsRes.unwrap());
			return geode::Ok();
		}
		else
			return geode::Err("Unable to parse {} NamedIDs: {}", name, namedIDsRes.unwrapErr());
	};

	auto strView = std::string_view{ str };

	auto firstDelimPos = strView.find('|');
	auto secondDelimPos = strView.find('|', firstDelimPos + 1);
	auto thirdDelimPos = strView.find('|', secondDelimPos + 1);
	auto fourthDelimPos = thirdDelimPos == std::string_view::npos
		? std::string_view::npos
		: strView.find('|', thirdDelimPos + 1);
	auto fifthDelimPos = fourthDelimPos == std::string_view::npos
		? std::string_view::npos
		: strView.find('|', fourthDelimPos + 1);

	if (
		firstDelimPos == std::string_view::npos ||
		secondDelimPos == std::string_view::npos
	)
		return geode::Err("Malformed NamedIDs string: Required delimiters not present");

	auto groupsStr = strView.substr(0, firstDelimPos);
	auto blocksStr = strView.substr(firstDelimPos + 1, secondDelimPos - firstDelimPos - 1);
	auto itemsStr = strView.substr(secondDelimPos + 1, thirdDelimPos - secondDelimPos - 1);
	// these were added in later updates
	auto timersStr = thirdDelimPos == std::string_view::npos
		? "" : strView.substr(thirdDelimPos + 1, fourthDelimPos - thirdDelimPos - 1);
	auto effectsStr = fourthDelimPos == std::string_view::npos
		? "" : strView.substr(fourthDelimPos + 1, fifthDelimPos - fourthDelimPos - 1);
	auto colorsStr = fifthDelimPos == std::string_view::npos
		? "" : strView.substr(fifthDelimPos + 1, strView.size() - fifthDelimPos - 1);

	if (auto res = parseStr(g_namedGroups, groupsStr, "Group"); res.isErr())
		return res;

	if (auto res = parseStr(g_namedCollisions, blocksStr, "Collision"); res.isErr())
		return res;

	if (auto res = parseStr(g_namedCounters, itemsStr, "Counter"); res.isErr())
		return res;

	if (thirdDelimPos != std::string_view::npos)
		if (auto res = parseStr(g_namedTimers, timersStr, "Timer"); res.isErr())
			return res;

	if (fourthDelimPos != std::string_view::npos)
		if (auto res = parseStr(g_namedEffects, effectsStr, "Effect"); res.isErr())
			return res;

	if (fifthDelimPos != std::string_view::npos)
		if (auto res = parseStr(g_namedColors, colorsStr, "Color"); res.isErr())
			return res;

	g_isDirty = g_isDirty || setDirty;

	return geode::Ok();
}

std::unordered_map<std::string, short, geode::utils::StringHash, std::equal_to<>>& NIDManager::getMutNamedIDs(NID nid)
{
	static std::unordered_map<NID, NamedIDs&> cache{
		{ NID::GROUP, g_namedGroups },
		{ NID::COLLISION, g_namedCollisions },
		{ NID::DYNAMIC_COUNTER_TIMER, g_namedCounters },
		{ NID::COUNTER, g_namedCounters },
		{ NID::TIMER, g_namedTimers },
		{ NID::EFFECT, g_namedEffects },
		{ NID::COLOR, g_namedColors },
	};

	return cache.at(nid).namedIDs;
}

void NIDManager::reset()
{
	g_namedGroups.namedIDs.clear();
	g_namedCollisions.namedIDs.clear();
	g_namedCounters.namedIDs.clear();
	g_namedTimers.namedIDs.clear();
	g_namedEffects.namedIDs.clear();
	g_namedColors.namedIDs.clear();

	g_isDirty = false;
}

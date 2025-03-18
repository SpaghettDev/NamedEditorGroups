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

NamedIDs& containerForID(NID id)
{
	switch (id)
	{
		case NID::GROUP:
			return g_namedGroups;

		case NID::COLLISION:
			return g_namedCollisions;

		case NID::DYNAMIC_COUNTER_TIMER: [[fallthrough]];
		case NID::COUNTER:
			return g_namedCounters;

		case NID::TIMER:
			return g_namedTimers;

		default:
			throw "Invalid NID enum value";
	}
}


geode::Result<std::string> NIDManager::getNameForID(NID nid, short id)
{
	const auto& ids = containerForID(nid);

	auto it = std::find_if(
		ids.namedIDs.begin(),
		ids.namedIDs.end(),
		[&](auto&& p) { return p.second == id; }
	);

	if (it == ids.namedIDs.end())
		return geode::Err("ID {} has no name associated to it", id);

	return geode::Ok(it->first);
}

geode::Result<short> NIDManager::getIDForName(NID nid, const std::string& name)
{
	const auto& ids = containerForID(nid);

	if (!ids.namedIDs.contains(name))
		return geode::Err("Name {} has no ID associated to it", name);

	return geode::Ok(ids[name]);
}

geode::Result<> NIDManager::saveNamedID(NID nid, std::string&& name, short id)
{
	if (id <= 0)
		return geode::Err("Invalid ID!");

	if (auto sanitizeRes = ng::utils::sanitizeName(name); sanitizeRes.isErr())
		return sanitizeRes;

	auto& ids = containerForID(nid);

	if (auto idName = getNameForID(nid, id); idName.isOk())
		ids.namedIDs.erase(idName.unwrap());

	NewNamedIDEvent(nid, name, id).post();
	g_isDirty = true;

	ids.namedIDs[std::move(name)] = id;

	return geode::Ok();
}

geode::Result<> NIDManager::removeNamedID(NID nid, std::string&& name)
{
	auto& ids = containerForID(nid);

	if (!ids.namedIDs.contains(name))
		return geode::Err("No saved Named ID {}", name);

	RemovedNamedIDEvent(nid, name, ids[name]).post();
	g_isDirty = true;

	ids.namedIDs.erase(std::move(name));

	return geode::Ok();
}

geode::Result<> NIDManager::removeNamedID(NID nid, short id)
{
	auto& ids = containerForID(nid);
	auto&& name = getNameForID(nid, id);

	if (name.isErr())
		return geode::Err(name.unwrapErr());

	RemovedNamedIDEvent(nid, name.unwrap(), id).post();
	g_isDirty = true;

	ids.namedIDs.erase(std::move(name.unwrap()));

	return geode::Ok();
}

const std::unordered_map<std::string, short>& NIDManager::getNamedIDs(NID nid)
{
	return containerForID(nid).namedIDs;
}


bool NIDManager::isDirty() { return g_isDirty; }

std::string NIDManager::dumpNamedIDs()
{
	return fmt::format(
		"{}|{}|{}|{}",
		g_namedGroups.dump(),
		g_namedCollisions.dump(),
		g_namedCounters.dump(),
		g_namedTimers.dump()
	);
}

geode::Result<> NIDManager::importNamedIDs(const std::string& str)
{
	auto strView = std::string_view{ std::move(str) };

	auto firstDelimPos = strView.find('|');
	auto secondDelimPos = strView.find('|', firstDelimPos + 1);
	auto thirdDelimPos = strView.find('|', secondDelimPos + 1);
	auto fourthDelimPos = strView.find('|', thirdDelimPos + 1);
	auto fifthDelimPos = strView.find('|', fourthDelimPos + 1);

	if (
		firstDelimPos == std::string_view::npos ||
		secondDelimPos == std::string_view::npos
	)
		return geode::Err("Malformed NamedIDs string: Required delimiters not present");

	auto groupsStr = strView.substr(0, firstDelimPos);
	auto blocksStr = strView.substr(firstDelimPos + 1, secondDelimPos - firstDelimPos - 1);
	auto itemsStr = strView.substr(secondDelimPos + 1, thirdDelimPos - secondDelimPos - 1);
	// these were added in later updates
	auto timersStr = thirdDelimPos == std::string::npos
		? "" : strView.substr(thirdDelimPos + 1, fourthDelimPos - thirdDelimPos - 1);
	auto colorsStr = fourthDelimPos == std::string::npos
		? "" : strView.substr(fourthDelimPos + 1, fifthDelimPos - fourthDelimPos - 1);
	auto animationsStr = fifthDelimPos == std::string::npos
		? "" : strView.substr(fifthDelimPos + 1);

	if (auto&& namedGroupsRes = NamedIDs::from(std::move(groupsStr)); namedGroupsRes.isOk())
		g_namedGroups = std::move(namedGroupsRes.unwrap());
	else
		return geode::Err("Unable to parse Group NamedIDs: {}", namedGroupsRes.unwrapErr());

	if (auto&& namedCollisionsRes = NamedIDs::from(std::move(blocksStr)); namedCollisionsRes.isOk())
		g_namedCollisions = std::move(namedCollisionsRes.unwrap());
	else
		return geode::Err("Unable to parse Collision NamedIDs: {}", namedCollisionsRes.unwrapErr());

	if (auto&& namedCountersRes = NamedIDs::from(std::move(itemsStr)); namedCountersRes.isOk())
		g_namedCounters = std::move(namedCountersRes.unwrap());
	else
		return geode::Err("Unable to parse Counter NamedIDs: {}", namedCountersRes.unwrapErr());

	if (auto&& namedTimersRes = NamedIDs::from(std::move(timersStr)); namedTimersRes.isOk())
		g_namedTimers = std::move(namedTimersRes.unwrap());
	else
		return geode::Err("Unable to parse Counter NamedIDs: {}", namedTimersRes.unwrapErr());

	return geode::Ok();
}

void NIDManager::reset()
{
	g_namedGroups.namedIDs.clear();
	g_namedCollisions.namedIDs.clear();
	g_namedCounters.namedIDs.clear();
	g_namedTimers.namedIDs.clear();

	g_isDirty = false;
}

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
		return std::move(geode::Err("ID {} has no name associated to it", id));

	return std::move(geode::Ok(it->first));
}

geode::Result<short> NIDManager::getIDForName(NID nid, const std::string& name)
{
	const auto& ids = containerForID(nid);

	if (!ids.namedIDs.contains(name))
		return std::move(geode::Err("Name {} has no ID associated to it", name));

	return std::move(geode::Ok(ids[name]));
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

void NIDManager::importNamedIDs(const std::string& str)
{
	auto strView = std::string_view{ std::move(str) };

	auto firstDelimPos = strView.find('|');
	auto secondDelimPos = strView.find('|', firstDelimPos + 1);
	auto thirdDelimPos = strView.find('|', secondDelimPos + 1);

	auto groupsStr = strView.substr(0, firstDelimPos);
	auto blocksStr = strView.substr(firstDelimPos + 1, secondDelimPos - firstDelimPos - 1);
	auto itemsStr = strView.substr(secondDelimPos + 1, thirdDelimPos - secondDelimPos - 1);
	auto timersStr = thirdDelimPos == std::string::npos
		? "" : strView.substr(thirdDelimPos + 1);

	g_namedGroups = g_namedGroups.from(std::move(groupsStr));
	g_namedCollisions = g_namedCollisions.from(std::move(blocksStr));
	g_namedCounters = g_namedCounters.from(std::move(itemsStr));
	g_namedTimers = g_namedTimers.from(std::move(timersStr));
}

void NIDManager::reset()
{
	g_namedGroups.namedIDs.clear();
	g_namedCollisions.namedIDs.clear();
	g_namedCounters.namedIDs.clear();
	g_namedTimers.namedIDs.clear();

	g_isDirty = false;
}

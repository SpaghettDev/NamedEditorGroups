#include <NIDManager.hpp>

#include "NamedIDs.hpp"

#include "../events/NewNamedIDEvent.hpp"
#include "../events/RemovedNamedIDEvent.hpp"

#include "utils.hpp"

static NamedIDs g_namedGroups;
static NamedIDs g_namedCollisions;
static NamedIDs g_namedCounters;

NamedIDs& containerForID(NID id)
{
	switch (id)
	{
		case NID::GROUP:
			return g_namedGroups;

		case NID::COLLISION:
			return g_namedCollisions;

		case NID::COUNTER:
			return g_namedCounters;

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
	if (auto sanitizeRes = ng::utils::sanitizeName(name); sanitizeRes.isErr())
		return sanitizeRes;

	auto& ids = containerForID(nid);

	if (auto idName = getNameForID(nid, id); idName.isOk())
		ids.namedIDs.erase(idName.unwrap());

	NewNamedIDEvent(nid, name, id).post();

	ids.namedIDs[std::move(name)] = id;

	return geode::Ok();
}

geode::Result<> NIDManager::removeNamedID(NID nid, std::string&& name)
{
	auto& ids = containerForID(nid);

	if (!ids.namedIDs.contains(name))
		return geode::Err("No saved Named ID {}", name);

	RemovedNamedIDEvent(nid, name, ids[name]).post();

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

	ids.namedIDs.erase(std::move(name.unwrap()));

	return geode::Ok();
}

const std::unordered_map<std::string, short>& NIDManager::getNamedIDs(NID nid)
{
	return containerForID(nid).namedIDs;
}

std::string NIDManager::dumpNamedIDs()
{
	return fmt::format(
		"{}|{}|{}",
		g_namedGroups.dump(),
		g_namedCollisions.dump(),
		g_namedCounters.dump()
	);
}

void NIDManager::importNamedIDs(const std::string& str)
{
	auto strView = std::string_view{ std::move(str) };

	auto firstDelimPos = strView.find('|');
	auto secondDelimPos = strView.find('|', firstDelimPos + 1);

	auto groupsStr = strView.substr(0, firstDelimPos);
	auto blocksStr = strView.substr(firstDelimPos + 1, secondDelimPos - firstDelimPos - 1);
	auto itemsStr = strView.substr(secondDelimPos + 1);

	g_namedGroups = g_namedGroups.from(std::move(groupsStr));
	g_namedCollisions = g_namedCollisions.from(std::move(blocksStr));
	g_namedCounters = g_namedCounters.from(std::move(itemsStr));
}

void NIDManager::reset()
{
	g_namedGroups.namedIDs.clear();
	g_namedCollisions.namedIDs.clear();
	g_namedCounters.namedIDs.clear();
}

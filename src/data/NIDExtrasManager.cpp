#define GEODE_DEFINE_EVENT_EXPORTS
#include <NIDExtrasManager.hpp>
#undef GEODE_DEFINE_EVENT_EXPORTS

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-inline"
#include <NIDManager.hpp>
#pragma clang diagnostic pop

#include <types/NamedIDExtras.hpp>

#include <string_view>
#include <filesystem>
#include <fstream>

#include "events/NewNamedIDExtrasEvent.hpp"
#include "events/RemovedNamedIDExtrasEvent.hpp"

#include "globals.hpp"

#define LEVEL_ID_API_CHECK()                 \
	if (!ng::globals::g_isEditorIDAPILoaded) \
		return geode::Err("Level ID API is not installed")

static std::filesystem::path s_saveDataDir = "";
static int g_levelID = 0;
static bool g_isDirty = false;
static NamedIDsExtras g_namedGroupIDsExtras;
static NamedIDsExtras g_namedCollisionIDsExtras;
static NamedIDsExtras g_namedCounterIDsExtras;
static NamedIDsExtras g_namedTimerIDsExtras;
static NamedIDsExtras g_namedEffectIDsExtras;
static NamedIDsExtras g_namedColorIDsExtras;

geode::Result<NamedIDsExtras&> extrasContainerForNID(NID id)
{
	switch (id)
	{
		case NID::GROUP:
			return geode::Ok(g_namedGroupIDsExtras);

		case NID::COLLISION:
			return geode::Ok(g_namedCollisionIDsExtras);

		case NID::DYNAMIC_COUNTER_TIMER: [[fallthrough]];
		case NID::COUNTER:
			return geode::Ok(g_namedCounterIDsExtras);

		case NID::TIMER:
			return geode::Ok(g_namedTimerIDsExtras);

		case NID::EFFECT:
			return geode::Ok(g_namedEffectIDsExtras);

		case NID::COLOR:
			return geode::Ok(g_namedColorIDsExtras);

		default:
			return geode::Err("Invalid NID enum value");
	}
}


geode::Result<bool> NIDExtrasManager::getIsNamedIDPreviewed(NID nid, short id)
{
	LEVEL_ID_API_CHECK();

	const auto& ids = GEODE_UNWRAP(extrasContainerForNID(nid));

	if (!ids.extras.contains(id))
		return geode::Ok(true);

	return geode::Ok(ids[id].isPreviewed);
}

geode::Result<bool> NIDExtrasManager::getIsNamedIDPreviewed(NID nid, std::string_view name)
{
	LEVEL_ID_API_CHECK();

	auto res = NIDManager::getIDForName(nid, name);
	if (res.isErr())
		return geode::Err(res.unwrapErr());

	return getIsNamedIDPreviewed(nid, res.unwrap());
}

geode::Result<std::string> NIDExtrasManager::getNamedIDDescription(NID nid, short id)
{
	LEVEL_ID_API_CHECK();

	const auto& ids = GEODE_UNWRAP(extrasContainerForNID(nid));

	if (!ids.extras.contains(id))
		return geode::Err("ID {} doesn't have a description", id);

	return geode::Ok(ids[id].description);
}

geode::Result<std::string> NIDExtrasManager::getNamedIDDescription(NID nid, std::string_view name)
{
	LEVEL_ID_API_CHECK();

	auto res = NIDManager::getIDForName(nid, name);
	if (res.isErr())
		return geode::Err(res.unwrapErr());

	return getNamedIDDescription(nid, res.unwrap());
}

geode::Result<> NIDExtrasManager::setNamedIDIsPreviewed(NID nid, short id, bool state)
{
	LEVEL_ID_API_CHECK();

	auto idsRes = extrasContainerForNID(nid);
	if (idsRes.isErr())
		return geode::Err(idsRes.unwrapErr());
	auto& ids = idsRes.unwrap();

	if (!ids.extras.contains(id))
		ids.extras[id] = { "", state };
	else
		ids[id].isPreviewed = state;

	g_isDirty = true;
	NewNamedIDExtrasEvent().send(nid, id, ids[id]);

	return geode::Ok();
}

geode::Result<> NIDExtrasManager::setNamedIDIsPreviewed(NID nid, std::string_view name, bool state)
{
	LEVEL_ID_API_CHECK();

	auto res = NIDManager::getIDForName(nid, name);
	if (res.isErr())
		return geode::Err(res.unwrapErr());

	return setNamedIDIsPreviewed(nid, res.unwrap(), state);
}

geode::Result<> NIDExtrasManager::setNamedIDDescription(NID nid, short id, std::string_view description)
{
	LEVEL_ID_API_CHECK();

	auto idsRes = extrasContainerForNID(nid);
	if (idsRes.isErr())
		return geode::Err(idsRes.unwrapErr());
	auto& ids = idsRes.unwrap();

	if (!ids.extras.contains(id))
		ids.extras[id] = { std::string{ description }, true };
	else
		ids[id].description = description;

	g_isDirty = true;
	NewNamedIDExtrasEvent().send(nid, id, ids[id]);

	return geode::Ok();
}

geode::Result<> NIDExtrasManager::setNamedIDDescription(NID nid, std::string_view name, std::string_view description)
{
	LEVEL_ID_API_CHECK();

	auto res = NIDManager::getIDForName(nid, name);
	if (res.isErr())
		return geode::Err(res.unwrapErr());

	return setNamedIDDescription(nid, res.unwrap(), description);
}

geode::Result<NamedIDExtra> NIDExtrasManager::getNamedIDExtras(NID nid, short id)
{
	LEVEL_ID_API_CHECK();

	const auto& ids = GEODE_UNWRAP(extrasContainerForNID(nid));
	if (!ids.extras.contains(id))
		return geode::Err("ID {} doesn't have extra data", id);

	return geode::Ok(ids.extras.at(id));
}

geode::Result<NamedIDExtra> NIDExtrasManager::getNamedIDExtras(NID nid, const std::string& name)
{
	LEVEL_ID_API_CHECK();

	auto res = NIDManager::getIDForName(nid, name);
	if (res.isErr())
		return geode::Err(res.unwrapErr());

	return getNamedIDExtras(nid, res.unwrap());
}

geode::Result<> NIDExtrasManager::setNamedIDExtras(NID nid, short id, NamedIDExtra extras)
{
	LEVEL_ID_API_CHECK();

	auto idsRes = extrasContainerForNID(nid);
	if (idsRes.isErr())
		return geode::Err(idsRes.unwrapErr());
	auto& ids = idsRes.unwrap();

	ids.extras[id] = std::move(extras);

	g_isDirty = true;
	NewNamedIDExtrasEvent().send(nid, id, ids.extras[id]);

	return geode::Ok();
}

geode::Result<> NIDExtrasManager::setNamedIDExtras(NID nid, std::string_view name, NamedIDExtra extras)
{
	LEVEL_ID_API_CHECK();

	auto res = NIDManager::getIDForName(nid, name);
	if (res.isErr())
		return geode::Err(res.unwrapErr());

	return setNamedIDExtras(nid, res.unwrap(), std::move(extras));
}

geode::Result<> NIDExtrasManager::removeNamedIDExtras(NID nid, short id)
{
	LEVEL_ID_API_CHECK();

	auto idsRes = extrasContainerForNID(nid);
	if (idsRes.isErr())
		return geode::Err(idsRes.unwrapErr());
	auto& ids = idsRes.unwrap();

	if (!ids.extras.contains(id))
		return geode::Err("ID {} doesn't have extra data", id);

	ids.extras.erase(id);

	g_isDirty = true;
	RemovedNamedIDExtrasEvent().send(nid, id);

	return geode::Ok();
}

geode::Result<> NIDExtrasManager::removeNamedIDExtras(NID nid, std::string_view name)
{
	LEVEL_ID_API_CHECK();

	auto res = NIDManager::getIDForName(nid, name);
	if (res.isErr())
		return geode::Err(res.unwrapErr());

	return removeNamedIDExtras(nid, res.unwrap());
}

geode::Result<NamedIDsExtras> NIDExtrasManager::getNIDExtras(NID nid)
{
	LEVEL_ID_API_CHECK();

	auto idsRes = extrasContainerForNID(nid);
	if (idsRes.isErr())
		return geode::Err(idsRes.unwrapErr());

	return geode::Ok(idsRes.unwrap());
}

bool NIDExtrasManager::isDirty() { return g_isDirty; }

void NIDExtrasManager::init(int levelID)
{
	g_levelID = levelID;

	if (s_saveDataDir.empty())
	{
		s_saveDataDir = geode::Mod::get()->getSaveDir() / "extras";
		std::filesystem::create_directories(s_saveDataDir);
	}

	std::ifstream fileIn(s_saveDataDir / fmt::format("{}.nide", g_levelID), std::ios::binary);
	fileIn.seekg(0, std::ios::end);

	auto readNIDExtras = [&fileIn](NamedIDsExtras& nide) {
		std::size_t size;

		fileIn.read(reinterpret_cast<char*>(&size), sizeof(size));
		nide.extras.reserve(size);

		for (std::size_t i = 0; i < size; i++)
		{
			short id;
			std::size_t descLen;
			NamedIDExtra extras;

			fileIn.read(reinterpret_cast<char*>(&id), sizeof(id));

			fileIn.read(reinterpret_cast<char*>(&descLen), sizeof(descLen));
			extras.description.resize(descLen);
			fileIn.read(&extras.description.front(), descLen);

			fileIn.read(reinterpret_cast<char*>(&extras.isPreviewed), sizeof(extras.isPreviewed));

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
			fileIn.read(reinterpret_cast<char*>(&extras._reserved), sizeof(extras._reserved));
#pragma clang diagnostic pop

			nide.extras[id] = std::move(extras);
		}
	};

	if (fileIn.fail())
	{
		fileIn.close();
		return save();
	}

	fileIn.seekg(0);

	std::size_t version;
	fileIn.read(reinterpret_cast<char*>(&version), sizeof(version));

	// TODO: maybe transition helper here
	if (NamedIDExtra::VERSION - version >= 10)
	{
		geode::log::warn("Extras save data version mismatch: {} (cur. savedata ver.) != {} (file ver.)", NamedIDExtra::VERSION, version);
		return;
	}

	readNIDExtras(g_namedGroupIDsExtras);
	readNIDExtras(g_namedCollisionIDsExtras);
	readNIDExtras(g_namedCounterIDsExtras);
	readNIDExtras(g_namedTimerIDsExtras);
}

void NIDExtrasManager::save()
{
	std::ofstream fileOut(s_saveDataDir / fmt::format("{}.nide", g_levelID), std::ios::binary);

	auto writeNIDExtras = [&fileOut](NamedIDsExtras& nide) {
		std::size_t size = nide.extras.size();

		fileOut.write(reinterpret_cast<const char*>(&size), sizeof(size));
		for (const auto& [id, data] : nide.extras)
		{
			fileOut.write(reinterpret_cast<const char*>(&id), sizeof(id));

			std::size_t descLen = data.description.size();
			fileOut.write(reinterpret_cast<const char*>(&descLen), sizeof(descLen));
			fileOut.write(data.description.c_str(), descLen);

			fileOut.write(reinterpret_cast<const char*>(&data.isPreviewed), sizeof(data.isPreviewed));

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
			fileOut.write(reinterpret_cast<const char*>(&data._reserved), sizeof(data._reserved));
#pragma clang diagnostic pop
		}
	};

	std::size_t version = NamedIDExtra::VERSION;
	fileOut.write(reinterpret_cast<const char*>(&version), sizeof(version));

	writeNIDExtras(g_namedGroupIDsExtras);
	writeNIDExtras(g_namedCollisionIDsExtras);
	writeNIDExtras(g_namedCounterIDsExtras);
	writeNIDExtras(g_namedTimerIDsExtras);
}

void NIDExtrasManager::reset()
{
	g_namedGroupIDsExtras.extras.clear();
	g_namedCollisionIDsExtras.extras.clear();
	g_namedCounterIDsExtras.extras.clear();
	g_namedTimerIDsExtras.extras.clear();

	g_isDirty = false;
	g_levelID = 0;
}

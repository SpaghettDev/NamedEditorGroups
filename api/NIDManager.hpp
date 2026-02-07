#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include <Geode/loader/Dispatch.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/utils/StringMap.hpp>
#include <Geode/Result.hpp>

#include "NIDEnum.hpp"

#define MY_MOD_ID "spaghettdev.named-editor-groups"

namespace NIDManager
{
	inline geode::Result<std::string> getNameForID(NID nid, short id) GEODE_EVENT_EXPORT(&getNameForID, (nid, id));
	template <NID ID>
	geode::Result<std::string> getNameForID(short id)
	{
		return getNameForID(ID, id);
	}

	inline geode::Result<short> getIDForName(NID nid, std::string_view name) GEODE_EVENT_EXPORT(&getIDForName, (nid, name));
	template <NID ID>
	geode::Result<short> getIDForName(std::string_view name)
	{
		return getIDForName(ID, name);
	}

	inline geode::Result<const std::unordered_map<std::string, short, geode::utils::StringHash, std::equal_to<>>&> getNamedIDs(NID nid) GEODE_EVENT_EXPORT(&getNamedIDs, (nid));
	template <NID ID>
	const std::unordered_map<std::string, short, geode::utils::StringHash, std::equal_to<>>& getNamedIDs()
	{
		return getNamedIDs(ID).unwrap();
	}

	inline geode::Result<> saveNamedID(NID nid, std::string_view name, short id) GEODE_EVENT_EXPORT(&saveNamedID, (nid, name, id));
	template <NID ID>
	geode::Result<> saveNamedID(std::string_view name, short id)
	{
		return saveNamedID(ID, name, id);
	}

	inline geode::Result<> removeNamedID(NID nid, std::string_view name) GEODE_EVENT_EXPORT(&removeNamedID, (nid, name));
	template <NID ID>
	geode::Result<> removeNamedID(std::string_view name)
	{
		return removeNamedID(ID, name);
	}

	inline geode::Result<> removeNamedID(NID nid, short id) GEODE_EVENT_EXPORT_ID(static_cast<geode::Result<>(*)(NID, short)>(removeNamedID), (nid, id), MY_MOD_ID "/" "removeNamedID");
	template <NID ID>
	geode::Result<> removeNamedID(short id)
	{
		return removeNamedID(ID, id);
	}

#ifdef SPAGHETTDEV_NAMED_EDITOR_GROUPS_EXPORTING
	bool isDirty();
	bool isEmpty();
	std::string dumpNamedIDs();
	geode::Result<> importNamedIDs(const std::string& str, bool setDirty = false);
	std::unordered_map<std::string, short, geode::utils::StringHash, std::equal_to<>>& getMutNamedIDs(NID nid);

	void reset();
#endif // !SPAGHETTDEV_NAMED_EDITOR_GROUPS_EXPORTING
}

#undef MY_MOD_ID

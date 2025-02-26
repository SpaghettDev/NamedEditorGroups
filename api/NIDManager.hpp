#pragma once

#include <string>
#include <unordered_map>

#include <Geode/loader/Dispatch.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/Result.hpp>

#include "NIDEnum.hpp"

#ifdef GEODE_IS_WINDOWS
	#ifdef SPAGHETTDEV_NAMED_EDITOR_GROUPS_EXPORTING
		#define NAMED_EDITOR_GROUPS_DLL __declspec(dllexport)
	#else
		#define NAMED_EDITOR_GROUPS_DLL __declspec(dllimport)
	#endif
#else
	#define NAMED_EDITOR_GROUPS_DLL __attribute__((visibility("default")))
#endif

namespace NIDManager
{
	namespace event
	{
		using EventGetNameForID = geode::DispatchEvent<std::string*, NID, short>;
		using EventGetIDForName = geode::DispatchEvent<short*, NID, std::string>;
		using EventGetNamedIDs = geode::DispatchEvent<std::unordered_map<std::string, short>*, NID>;

		using EventSaveNamedID = geode::DispatchEvent<bool*, NID, std::string, short>;
		using EventRemoveNamedID1 = geode::DispatchEvent<bool*, NID, std::string>;
		using EventRemoveNamedID2 = geode::DispatchEvent<bool*, NID, short>;
	}

#ifndef NAMED_EDITOR_GROUPS_USE_EVENTS_API

	NAMED_EDITOR_GROUPS_DLL geode::Result<std::string> getNameForID(NID nid, short id);
	template <NID ID>
	geode::Result<std::string> getNameForID(short id)
	{
		return getNameForID(ID, id);
	}

	NAMED_EDITOR_GROUPS_DLL geode::Result<short> getIDForName(NID nid, const std::string& name);
	template <NID ID>
	geode::Result<short> getIDForName(const std::string& name)
	{
		return getIDForName(ID, name);
	}

	NAMED_EDITOR_GROUPS_DLL const std::unordered_map<std::string, short>& getNamedIDs(NID nid);
	template <NID ID>
	const std::unordered_map<std::string, short>& getNamedIDs()
	{
		return getNamedIDs(ID);
	}

	NAMED_EDITOR_GROUPS_DLL geode::Result<> saveNamedID(NID nid, std::string&& name, short id);
	template <NID ID>
	geode::Result<> saveNamedID(std::string&& name, short id)
	{
		return saveNamedID(ID, static_cast<std::string&&>(name), id);
	}

	NAMED_EDITOR_GROUPS_DLL geode::Result<> removeNamedID(NID nid, std::string&& name);
	template <NID ID>
	geode::Result<> removeNamedID(std::string&& name)
	{
		return removeNamedID(ID, static_cast<std::string&&>(name));
	}

	NAMED_EDITOR_GROUPS_DLL geode::Result<> removeNamedID(NID nid, short id);
	template <NID ID>
	geode::Result<> removeNamedID(short id)
	{
		return removeNamedID(ID, id);
	}

#ifdef SPAGHETTDEV_NAMED_EDITOR_GROUPS_EXPORTING
	bool isDirty();
	std::string dumpNamedIDs();
	void importNamedIDs(const std::string& str);

	void reset();
#endif // !SPAGHETTDEV_NAMED_EDITOR_GROUPS_EXPORTING

#endif // !NAMED_EDITOR_GROUPS_USE_EVENTS_API
}

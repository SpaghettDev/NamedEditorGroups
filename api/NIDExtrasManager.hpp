#pragma once

#include <string>

#include <Geode/loader/Dispatch.hpp>
#include <Geode/Result.hpp>

#include "NIDEnum.hpp"
#include "types/NamedIDExtras.hpp"

#ifndef NAMED_EDITOR_GROUPS_DLL
	#ifdef GEODE_IS_WINDOWS
		#ifdef SPAGHETTDEV_NAMED_EDITOR_GROUPS_EXPORTING
			#define NAMED_EDITOR_GROUPS_DLL __declspec(dllexport)
		#else
			#define NAMED_EDITOR_GROUPS_DLL __declspec(dllimport)
		#endif
	#else
		#define NAMED_EDITOR_GROUPS_DLL __attribute__((visibility("default")))
	#endif
#endif

namespace NIDExtrasManager
{
	// TODO: add others and app to API.cpp
	namespace event
	{
		using EventGetIsNamedIDPreviewed = geode::DispatchEvent<bool*, NID>;
		using EventGetNamedIDDescription = geode::DispatchEvent<std::string*, NID>;
	}

#ifndef NAMED_EDITOR_GROUPS_USE_EVENTS_API

	NAMED_EDITOR_GROUPS_DLL geode::Result<bool> getIsNamedIDPreviewed(NID nid, short id);
	template <NID ID>
	geode::Result<bool> getIsNamedIDPreviewed(short id)
	{
		return getIsNamedIDPreviewed(ID, id);
	}

	NAMED_EDITOR_GROUPS_DLL geode::Result<bool> getIsNamedIDPreviewed(NID nid, std::string&& name);
	template <NID ID>
	geode::Result<bool> getIsNamedIDPreviewed(std::string&& name)
	{
		return getIsNamedIDPreviewed(ID, static_cast<std::string&&>(name));
	}

	NAMED_EDITOR_GROUPS_DLL geode::Result<std::string> getNamedIDDescription(NID nid, short id);
	template <NID ID>
	geode::Result<std::string> getNamedIDDescription(short id)
	{
		return getNamedIDDescription(ID, id);
	}

	NAMED_EDITOR_GROUPS_DLL geode::Result<std::string> getNamedIDDescription(NID nid, std::string&& name);
	template <NID ID>
	geode::Result<std::string> getNamedIDDescription(std::string&& name)
	{
		return getNamedIDDescription(ID, static_cast<std::string&&>(name));
	}

	NAMED_EDITOR_GROUPS_DLL geode::Result<> setNamedIDIsPreviewed(NID nid, short id, bool state);
	template <NID ID>
	geode::Result<> setNamedIDIsPreviewed(short id, bool state)
	{
		return setNamedIDIsPreviewed(ID, id, state);
	}

	NAMED_EDITOR_GROUPS_DLL geode::Result<> setNamedIDIsPreviewed(NID nid, std::string&& name, bool state);
	template <NID ID>
	geode::Result<> setNamedIDIsPreviewed(std::string&& name, bool state)
	{
		return setNamedIDIsPreviewed(ID, name, state);
	}

	NAMED_EDITOR_GROUPS_DLL geode::Result<> setNamedIDDescription(NID nid, short id, std::string&& description);
	template <NID ID>
	geode::Result<> setNamedIDDescription(short id, std::string&& description)
	{
		return setNamedIDDescription(ID, id, static_cast<std::string&&>(description));
	}

	NAMED_EDITOR_GROUPS_DLL geode::Result<> setNamedIDDescription(NID nid, std::string&& name, std::string&& description);
	template <NID ID>
	geode::Result<> setNamedIDDescription(std::string&& name, std::string&& description)
	{
		return setNamedIDDescription(ID, name, static_cast<std::string&&>(description));
	}

	NAMED_EDITOR_GROUPS_DLL geode::Result<> setNamedIDExtras(NID nid, short id, NamedIDExtra&& extras);
	template <NID ID>
	geode::Result<> setNamedIDExtras(short id, NamedIDExtra&& extras)
	{
		return setNamedIDExtras(ID, id, extras);
	}

	NAMED_EDITOR_GROUPS_DLL geode::Result<> setNamedIDExtras(NID nid, std::string&& name, NamedIDExtra&& extras);
	template <NID ID>
	geode::Result<> setNamedIDExtras(std::string&& name, NamedIDExtra&& extras)
	{
		return setNamedIDExtras(ID, static_cast<std::string&&>(name), extras);
	}

	NAMED_EDITOR_GROUPS_DLL geode::Result<> removeNamedIDExtras(NID nid, short id);
	template <NID ID>
	geode::Result<> removeNamedIDExtras(short id)
	{
		return removeNamedIDExtras(ID, id);
	}

	NAMED_EDITOR_GROUPS_DLL geode::Result<> removeNamedIDExtras(NID nid, std::string&& name);
	template <NID ID>
	geode::Result<> removeNamedIDExtras(std::string&& name)
	{
		return removeNamedIDExtras(ID, static_cast<std::string&&>(name));
	}

#ifdef SPAGHETTDEV_NAMED_EDITOR_GROUPS_EXPORTING
	bool isDirty();
	void init(int);
	void save();

	void reset();
#endif // !SPAGHETTDEV_NAMED_EDITOR_GROUPS_EXPORTING

#endif // !NAMED_EDITOR_GROUPS_USE_EVENTS_API
}

#pragma once

#include <string_view>

#include <Geode/loader/Dispatch.hpp>
#include <Geode/Result.hpp>

#include "NIDEnum.hpp"
#include "types/NamedIDExtras.hpp"

#define MY_MOD_ID "spaghettdev.named-editor-groups"

namespace NIDExtrasManager
{
	inline geode::Result<bool> getIsNamedIDPreviewed(NID nid, short id) GEODE_EVENT_EXPORT(&getIsNamedIDPreviewed, (nid, id));
	template <NID ID>
	geode::Result<bool> getIsNamedIDPreviewed(short id)
	{
		return getIsNamedIDPreviewed(ID, id);
	}

	inline geode::Result<bool> getIsNamedIDPreviewed(NID nid, std::string_view name) GEODE_EVENT_EXPORT_ID(static_cast<geode::Result<bool>(*)(NID, std::string_view)>(getIsNamedIDPreviewed), (nid, name), MY_MOD_ID "/" "getIsNamedIDPreviewed");
	template <NID ID>
	geode::Result<bool> getIsNamedIDPreviewed(std::string_view name)
	{
		return getIsNamedIDPreviewed(ID, name);
	}

	inline geode::Result<std::string> getNamedIDDescription(NID nid, short id) GEODE_EVENT_EXPORT(&getNamedIDDescription, (nid, id));
	template <NID ID>
	geode::Result<std::string> getNamedIDDescription(short id)
	{
		return getNamedIDDescription(ID, id);
	}

	inline geode::Result<std::string> getNamedIDDescription(NID nid, std::string_view name) GEODE_EVENT_EXPORT_ID(static_cast<geode::Result<std::string>(*)(NID, std::string_view)>(getNamedIDDescription), (nid, name), MY_MOD_ID "/" "getNamedIDDescription");
	template <NID ID>
	geode::Result<std::string> getNamedIDDescription(std::string_view name)
	{
		return getNamedIDDescription(ID, name);
	}

	inline geode::Result<> setNamedIDIsPreviewed(NID nid, short id, bool state) GEODE_EVENT_EXPORT(&setNamedIDIsPreviewed, (nid, id, state));
	template <NID ID>
	geode::Result<> setNamedIDIsPreviewed(short id, bool state)
	{
		return setNamedIDIsPreviewed(ID, id, state);
	}

	inline geode::Result<> setNamedIDIsPreviewed(NID nid, std::string_view name, bool state) GEODE_EVENT_EXPORT_ID(static_cast<geode::Result<>(*)(NID, std::string_view, bool)>(setNamedIDIsPreviewed), (nid, name, state), MY_MOD_ID "/" "setNamedIDIsPreviewed");
	template <NID ID>
	geode::Result<> setNamedIDIsPreviewed(std::string_view name, bool state)
	{
		return setNamedIDIsPreviewed(ID, name, state);
	}

	inline geode::Result<> setNamedIDDescription(NID nid, short id, std::string_view description) GEODE_EVENT_EXPORT(&setNamedIDDescription, (nid, id, description));
	template <NID ID>
	geode::Result<> setNamedIDDescription(short id, std::string_view description)
	{
		return setNamedIDDescription(ID, id, description);
	}

	inline geode::Result<> setNamedIDDescription(NID nid, std::string_view name, std::string_view description) GEODE_EVENT_EXPORT_ID(static_cast<geode::Result<>(*)(NID, std::string_view, std::string_view)>(setNamedIDDescription), (nid, name, description), MY_MOD_ID "/" "setNamedIDDescription");
	template <NID ID>
	geode::Result<> setNamedIDDescription(std::string_view name, std::string_view description)
	{
		return setNamedIDDescription(ID, name, description);
	}

	inline geode::Result<NamedIDExtra> getNamedIDExtras(NID nid, short id) GEODE_EVENT_EXPORT(&getNamedIDExtras, (nid, id));
	template <NID ID>
	geode::Result<NamedIDExtra> getNamedIDExtras(short id)
	{
		return getNamedIDExtras(ID, id);
	}

	inline geode::Result<NamedIDExtra> getNamedIDExtras(NID nid, const std::string& name) GEODE_EVENT_EXPORT_ID(static_cast<geode::Result<NamedIDExtra>(*)(NID, const std::string&)>(getNamedIDExtras), (nid, name), MY_MOD_ID "/" "getNamedIDExtras");
	template <NID ID>
	geode::Result<NamedIDExtra> getNamedIDExtras(const std::string& name)
	{
		return getNamedIDExtras(ID, name);
	}

	inline geode::Result<> setNamedIDExtras(NID nid, short id, NamedIDExtra extras) GEODE_EVENT_EXPORT(&setNamedIDExtras, (nid, id, extras));
	template <NID ID>
	geode::Result<> setNamedIDExtras(short id, NamedIDExtra extras)
	{
		return setNamedIDExtras(ID, id, extras);
	}

	inline geode::Result<> setNamedIDExtras(NID nid, std::string_view name, NamedIDExtra extras) GEODE_EVENT_EXPORT_ID(static_cast<geode::Result<>(*)(NID, std::string_view, NamedIDExtra)>(setNamedIDExtras), (nid, name, extras), MY_MOD_ID "/" "setNamedIDExtras");
	template <NID ID>
	geode::Result<> setNamedIDExtras(std::string_view name, NamedIDExtra extras)
	{
		return setNamedIDExtras(ID, name, extras);
	}

	inline geode::Result<> removeNamedIDExtras(NID nid, short id) GEODE_EVENT_EXPORT(&removeNamedIDExtras, (nid, id));
	template <NID ID>
	geode::Result<> removeNamedIDExtras(short id)
	{
		return removeNamedIDExtras(ID, id);
	}

	inline geode::Result<> removeNamedIDExtras(NID nid, std::string_view name) GEODE_EVENT_EXPORT_ID(static_cast<geode::Result<>(*)(NID, std::string_view)>(removeNamedIDExtras), (nid, name), MY_MOD_ID "/" "removeNamedIDExtras");
	template <NID ID>
	geode::Result<> removeNamedIDExtras(std::string_view name)
	{
		return removeNamedIDExtras(ID, name);
	}

	inline geode::Result<NamedIDsExtras> getNIDExtras(NID nid) GEODE_EVENT_EXPORT(&getNIDExtras, (nid));
	template <NID ID>
	geode::Result<NamedIDsExtras> getNIDExtras()
	{
		return getNIDExtras(ID);
	}

#ifdef SPAGHETTDEV_NAMED_EDITOR_GROUPS_EXPORTING
	bool isDirty();
	void init(int);
	void save();

	void reset();
#endif // !SPAGHETTDEV_NAMED_EDITOR_GROUPS_EXPORTING
}

#undef MY_MOD_ID

#include <Geode/loader/ModEvent.hpp>

#include "globals.hpp"

using namespace geode::prelude;

$on_mod(Loaded)
{
	ng::globals::g_isEditorIDAPILoaded = Loader::get()->isModLoaded("cvolton.level-id-api");
	ng::globals::g_isBetterEditLoaded = Loader::get()->isModLoaded("hjfod.betteredit");
	ng::globals::g_isImprovedGroupViewLoaded = Loader::get()->isModLoaded("alphalaneous.improved_group_view");

	ng::globals::g_buildHelperRawNameFormat = Mod::get()->getSettingValue<std::string>("auto-name-format");

	{
		std::string rawNameFormat = ng::globals::g_buildHelperRawNameFormat;

		std::string nameFormat;
		nameFormat = rawNameFormat.replace(rawNameFormat.find("{name}"), 6, "{}");
		nameFormat = nameFormat.replace(nameFormat.find("{id}"), 4, "{}");

		ng::globals::g_buildHelperNameFormat = std::move(nameFormat);
	}

	geode::listenForSettingChanges<std::string>("auto-name-format", [](std::string value) {
		ng::globals::g_buildHelperRawNameFormat = value;

		std::string nameFormat;
		nameFormat = value.replace(value.find("{name}"), 6, "{}");
		nameFormat = nameFormat.replace(nameFormat.find("{id}"), 4, "{}");

		ng::globals::g_buildHelperNameFormat = std::move(nameFormat);
	});
}

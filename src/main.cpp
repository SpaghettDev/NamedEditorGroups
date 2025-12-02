#include <Geode/loader/ModEvent.hpp>

#include "globals.hpp"

$on_mod(Loaded)
{
	new geode::EventListener(+[](geode::GameEvent*) {
		ng::globals::g_isEditorIDAPILoaded = geode::Loader::get()->isModLoaded("cvolton.level-id-api");
		ng::globals::g_isBetterEditLoaded = geode::Loader::get()->isModLoaded("hjfod.betteredit");

		ng::globals::g_buildHelperRawNameFormat = geode::Mod::get()->getSettingValue<std::string>("auto-name-format");

		{
			std::string rawNameFormat = ng::globals::g_buildHelperRawNameFormat;

			std::string nameFormat;
			nameFormat = rawNameFormat.replace(rawNameFormat.find("{name}"), 6, "{}");
			nameFormat = nameFormat.replace(nameFormat.find("{id}"), 4, "{}");

			ng::globals::g_buildHelperNameFormat = std::move(nameFormat);
		}
	}, geode::GameEventFilter(geode::GameEventType::Loaded));

	geode::listenForSettingChanges<std::string>("auto-name-format", [](std::string value) {
		ng::globals::g_buildHelperRawNameFormat = value;

		std::string nameFormat;
		nameFormat = value.replace(value.find("{name}"), 6, "{}");
		nameFormat = nameFormat.replace(nameFormat.find("{id}"), 4, "{}");

		ng::globals::g_buildHelperNameFormat = std::move(nameFormat);
	});
}

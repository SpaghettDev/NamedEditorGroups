#include <Geode/loader/GameEvent.hpp>

#include "globals.hpp"

$execute
{
	new geode::EventListener(+[](geode::GameEvent*) {
		ng::globals::g_isEditorIDAPILoaded = geode::Loader::get()->isModLoaded("cvolton.level-id-api");
		ng::globals::g_isBetterEditLoaded = geode::Loader::get()->isModLoaded("hjfod.betteredit");
	}, geode::GameEventFilter(geode::GameEventType::Loaded));
}

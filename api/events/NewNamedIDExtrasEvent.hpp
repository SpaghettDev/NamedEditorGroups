#pragma once

#include <Geode/loader/Event.hpp>

#include "../NIDEnum.hpp"
#include "../types/NamedIDExtras.hpp"

struct NewNamedIDExtrasEvent : geode::GlobalEvent<NewNamedIDExtrasEvent, bool(NID, short, const NamedIDExtra&), NID>
{
	using GlobalEvent::GlobalEvent;
};

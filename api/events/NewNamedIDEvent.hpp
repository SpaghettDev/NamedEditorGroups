#pragma once

#include <Geode/loader/Event.hpp>

#include "../NIDEnum.hpp"

struct NewNamedIDEvent : geode::GlobalEvent<NewNamedIDEvent, bool(NID, std::string_view, short), NID>
{
	using GlobalEvent::GlobalEvent;
};

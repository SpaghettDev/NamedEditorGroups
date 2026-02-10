#pragma once

#include <Geode/loader/Event.hpp>

#include "../NIDEnum.hpp"

struct RemovedNamedIDEvent : geode::GlobalEvent<RemovedNamedIDEvent, bool(NID, std::string_view, short), NID>
{
	using GlobalEvent::GlobalEvent;
};

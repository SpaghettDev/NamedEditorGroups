#pragma once

#include <Geode/loader/Event.hpp>

#include "../NIDEnum.hpp"

struct RemovedNamedIDExtrasEvent : geode::GlobalEvent<RemovedNamedIDExtrasEvent, bool(NID, short), NID>
{
	using GlobalEvent::GlobalEvent;
};

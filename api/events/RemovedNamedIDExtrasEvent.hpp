#pragma once

#include <Geode/loader/Event.hpp>

#include "../NIDEnum.hpp"

struct RemovedNamedIDExtrasEvent : geode::SimpleEvent<RemovedNamedIDExtrasEvent, NID, short>
{
	using SimpleEvent::SimpleEvent;

	NID getIDType() const { return m_id_type; }
	short getID() const { return m_id; }

protected:
	NID m_id_type;
	short m_id;
};

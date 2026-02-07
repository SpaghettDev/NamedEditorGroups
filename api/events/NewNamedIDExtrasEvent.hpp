#pragma once

#include <Geode/loader/Event.hpp>

#include "../NIDEnum.hpp"
#include "../types/NamedIDExtras.hpp"

struct NewNamedIDExtrasEvent : geode::SimpleEvent<NewNamedIDExtrasEvent, NID, short, const NamedIDExtra&>
{
	using SimpleEvent::SimpleEvent;

	NID getIDType() const { return m_id_type; }
	short getID() const { return m_id; }
	NamedIDExtra getExtras() const { return m_extras; }

protected:
	NID m_id_type;
	short m_id;
	NamedIDExtra m_extras;
};

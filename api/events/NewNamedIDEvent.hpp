#pragma once

#include <Geode/loader/Event.hpp>

#include "../NIDEnum.hpp"

struct NewNamedIDEvent : geode::SimpleEvent<NewNamedIDEvent, NID, std::string_view, short>
{
	using SimpleEvent::SimpleEvent;

	NID getIDType() const { return m_id_type; }
	std::string_view getName() const { return m_name; };
	short getID() const { return m_id; }

protected:
	NID m_id_type;
	std::string_view m_name;
	short m_id;
};

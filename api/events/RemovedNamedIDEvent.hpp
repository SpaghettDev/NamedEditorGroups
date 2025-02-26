#pragma once

#include <Geode/loader/Event.hpp>

#include "../NIDEnum.hpp"

class RemovedNamedIDEvent : public geode::Event
{
public:
	RemovedNamedIDEvent(NID nid, const std::string& name, short id)
		: m_id_type(nid), m_name(name), m_id(id)
	{}

	NID getIDType() const { return m_id_type; }
	const std::string& getName() const& { return m_name; };
	short getID() const { return m_id; }

protected:
	NID m_id_type;
	std::string m_name;
	short m_id;
};

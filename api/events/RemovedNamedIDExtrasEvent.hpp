#pragma once

#include <Geode/loader/Event.hpp>

#include "../NIDEnum.hpp"

class RemovedNamedIDExtrasEvent : public geode::Event
{
public:
	RemovedNamedIDExtrasEvent(NID nid, short id)
		: m_id_type(nid), m_id(id)
	{}

	NID getIDType() const { return m_id_type; }
	short getID() const { return m_id; }

protected:
	NID m_id_type;
	short m_id;
};

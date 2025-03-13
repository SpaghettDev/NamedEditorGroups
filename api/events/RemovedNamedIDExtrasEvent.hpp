#pragma once

#include <Geode/loader/Event.hpp>

#include "../NIDEnum.hpp"

class RemovedNamedIDExtrasEvent : public geode::Event
{
public:
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
	RemovedNamedIDExtrasEvent(NID nid, short id)
		: m_id_type(nid), m_id(id)
	{}
#pragma clang diagnostic pop

	NID getIDType() const { return m_id_type; }
	short getID() const { return m_id; }

protected:
	NID m_id_type;
	short m_id;
};

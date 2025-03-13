#pragma once

#include <Geode/loader/Event.hpp>

#include "../NIDEnum.hpp"
#include "../types/NamedIDExtras.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
class NewNamedIDExtrasEvent : public geode::Event
{
public:
	NewNamedIDExtrasEvent(NID nid, short id, const NamedIDExtra& extras)
		: m_id_type(nid), m_id(id), m_extras(extras)
	{}

	NID getIDType() const { return m_id_type; }
	short getID() const { return m_id; }
	NamedIDExtra getExtras() const { return m_extras; }

protected:
	NID m_id_type;
	short m_id;
	NamedIDExtra m_extras;
};
#pragma clang diagnostic pop

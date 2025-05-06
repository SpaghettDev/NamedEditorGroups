#pragma once

#pragma region ChanceObject
#include <Geode/binding/ChanceObject.hpp>

inline bool operator<(const ChanceObject& lhs, const ChanceObject& rhs) noexcept
{
	if (lhs.m_groupID != rhs.m_groupID) 
		return lhs.m_groupID < rhs.m_groupID;
	if (lhs.m_oldGroupID != rhs.m_oldGroupID) 
		return lhs.m_oldGroupID < rhs.m_oldGroupID;
	if (lhs.m_chance != rhs.m_chance)
		return lhs.m_chance < rhs.m_chance;
	return lhs.m_unk00c < rhs.m_unk00c;
}
#pragma endregion ChanceObject

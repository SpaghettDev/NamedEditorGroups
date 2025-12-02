#pragma once

#include <array>

#include <Geode/binding/GameObject.hpp>

namespace ng::types
{
	/**
	 * @brief Struct that saves some data members from a GameObject
	*/
	struct GameObjectData
	{
		inline GameObjectData(GameObject* obj)
			: m_classType(obj->m_classType),
				m_groupCount(obj->m_groupCount),
				m_groups(obj->m_groupCount > 0 ? *obj->m_groups : std::array<short, 10>{})
		{
			switch (m_classType)
			{
				case GameObjectClassType::Effect: {
					auto effectObj = static_cast<EffectGameObject*>(obj);
					m_targetGroupID = effectObj->m_targetGroupID;
					m_centerGroupID = effectObj->m_centerGroupID;
					m_targetModCenterID = effectObj->m_targetModCenterID;
					m_rotationTargetID = effectObj->m_rotationTargetID;
					m_copyColorID = effectObj->m_copyColorID;
					m_animationID = effectObj->m_animationID;
					m_itemID2 = effectObj->m_itemID2;
					m_controlID = effectObj->m_controlID;
					m_targetControlID = effectObj->m_targetControlID;
					m_itemID = effectObj->m_itemID;
					m_collectibleParticleID = effectObj->m_collectibleParticleID;
					m_forceModID = effectObj->m_forceModID;
					m_secretCoinID = effectObj->m_secretCoinID;
				}
				break;

				default: break;
			}
		}

		std::array<short, 10> m_groups;
		short m_groupCount;
		GameObjectClassType m_classType;
		int m_targetGroupID;
		int m_centerGroupID;
		int m_targetModCenterID;
		int m_rotationTargetID;
		int m_copyColorID;
		int m_animationID;
		int m_itemID2;
		int m_controlID;
		bool m_targetControlID;
		int m_itemID;
		int m_collectibleParticleID;
		int m_forceModID;
		int m_secretCoinID;
	};
}

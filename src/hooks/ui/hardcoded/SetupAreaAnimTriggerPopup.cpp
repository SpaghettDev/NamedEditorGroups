#include <Geode/modify/SetupAreaAnimTriggerPopup.hpp>

#include "../SetupPopups.hpp"

using namespace geode::prelude;

// This popup is weird because its property can either be a Group ID or an Effect ID
struct NIDSetupAreaAnimTriggerPopup : geode::Modify<NIDSetupAreaAnimTriggerPopup, SetupAreaAnimTriggerPopup>
{
	static constexpr std::uint16_t GROUP_AND_EFFECT_ID_PROPERTY = 51;
	static constexpr std::uint16_t USE_EFFECT_ID_PROPERTY = 355;

	void valueDidChange(int property, float value)
	{
		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);
		bool multipleObjs = !!this->m_gameObjects;

		switch (property)
		{
			case GROUP_AND_EFFECT_ID_PROPERTY: {
				static bool first_call = true;

				if (multipleObjs && first_call)
				{
					first_call = false;
					STP->m_fields->m_id_inputs.at(property).namedIDInput->setString("");
					return;
				}
			};
			break;

			case USE_EFFECT_ID_PROPERTY: {
				if (!STP->m_fields->m_id_inputs.contains(GROUP_AND_EFFECT_ID_PROPERTY)) break;

				auto& inputInfo = STP->m_fields->m_id_inputs.at(GROUP_AND_EFFECT_ID_PROPERTY);

				inputInfo.idType = value != .0f ? NID::EFFECT : NID::GROUP;
				this->textChanged(inputInfo.idInput);

				// why doesn't the game do this by default this game is stupid
				// if (multipleObjs)
				// {
				// 	for (auto obj : CCArrayExt<EnterEffectObject*>(this->m_gameObjects))
				// 		obj->m_useEffectID = value != .0f;
				// }
			};
			break;
		}
		
		SetupAreaAnimTriggerPopup::valueDidChange(property, value);
	}
};

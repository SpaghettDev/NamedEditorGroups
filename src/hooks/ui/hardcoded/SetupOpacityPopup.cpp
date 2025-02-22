#include <Geode/modify/SetupOpacityPopup.hpp>

#include "../SetupPopups.hpp"

#include "utils.hpp"

using namespace geode::prelude;

struct NIDSetupOpacityPopup : geode::Modify<NIDSetupOpacityPopup, SetupOpacityPopup>
{
	static constexpr std::uint16_t GROUP_ID_PROPERTY = 3;

	bool init(EffectGameObject* p0, CCArray* p1)
	{
		if (!SetupOpacityPopup::init(p0, p1)) return false;

		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		std::vector<CCNode*> nodes;
		nodes.reserve(5);

		// :broken_heart:
		nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "Group ID";
			}
		));
		nodes.emplace_back(this->m_mainLayer->getChildByTag(GROUP_ID_PROPERTY));
		nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[inputNodePos = nodes[1]->getPosition()](CCNode* c) {
				auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
				return castedNode && (castedNode->getPosition() == inputNodePos);
			}
		));
		nodes.emplace_back(this->m_buttonMenu->getChildByTag(1));
		nodes.emplace_back(this->m_buttonMenu->getChildByTag(2));

		auto inputInfo = STP->commonSetup(
			NID::GROUP,
			GROUP_ID_PROPERTY,
			std::move(nodes),
			this->m_mainLayer,
			this->m_buttonMenu
		);
		STP->m_fields->m_id_inputs[GROUP_ID_PROPERTY] = std::move(inputInfo);

		return true;
	}

	void onTargetIDArrow(CCObject* sender)
	{
		SetupOpacityPopup::onTargetIDArrow(sender);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->triggerArrowWasClicked(GROUP_ID_PROPERTY, sender->getTag() == 2);
	}

	void textChanged(CCTextInputNode* input)
	{
		SetupOpacityPopup::textChanged(input);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->textWasChanged(input);
	}
};

#include <Geode/modify/SetupTouchTogglePopup.hpp>

#include "../SetupPopups.hpp"

#include "utils.hpp"

using namespace geode::prelude;

struct NIDSetupTouchTogglePopup : geode::Modify<NIDSetupTouchTogglePopup, SetupTouchTogglePopup>
{
	static constexpr std::uint16_t GROUP_ID_PROPERTY = 3;

	bool init(EffectGameObject* p0, CCArray* p1)
	{
		if (!SetupTouchTogglePopup::init(p0, p1)) return false;

		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		std::vector<CCNode*> groupIDNodes;
		groupIDNodes.reserve(5);

		// :broken_heart:
		groupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "Group ID";
			}
		));
		groupIDNodes.emplace_back(this->m_mainLayer->getChildByTag(GROUP_ID_PROPERTY));
		groupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[inputNodePos = groupIDNodes[1]->getPosition()](CCNode* c) {
				auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
				return castedNode && (castedNode->getPosition() == inputNodePos);
			}
		));
		groupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCMenuItemSpriteExtra*>(c);
				return castedNode && castedNode->getTag() == 1;
		}));
		groupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCMenuItemSpriteExtra*>(c);
				return castedNode && castedNode->getTag() == 2;
		}));

		auto inputInfo = STP->commonSetup(
			NID::GROUP,
			GROUP_ID_PROPERTY,
			std::move(groupIDNodes),
			this->m_mainLayer,
			this->m_buttonMenu
		);
		STP->m_fields->m_id_inputs[GROUP_ID_PROPERTY] = std::move(inputInfo);

		return true;
	}

	void onTargetIDArrow(CCObject* sender)
	{
		SetupTouchTogglePopup::onTargetIDArrow(sender);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->triggerArrowWasClicked(GROUP_ID_PROPERTY, sender->getTag() == 2);
	}

	void textChanged(CCTextInputNode* input)
	{
		SetupTouchTogglePopup::textChanged(input);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->textWasChanged(input);
	}
};

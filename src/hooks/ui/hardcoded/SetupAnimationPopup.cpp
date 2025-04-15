#include <Geode/modify/SetupAnimationPopup.hpp>

#include "../SetupPopups.hpp"

#include "utils.hpp"

using namespace geode::prelude;

struct NIDSetupAnimationPopup : geode::Modify<NIDSetupAnimationPopup, SetupAnimationPopup>
{
	static constexpr std::uint16_t GROUP_ID_PROPERTY = 0;

	bool init(EffectGameObject* p0, CCArray* p1)
	{
		if (!SetupAnimationPopup::init(p0, p1)) return false;

		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		std::vector<CCNode*> groupIDNodes;
		groupIDNodes.reserve(5);


		groupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "Group ID";
			}
		));
		groupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCTextInputNode*>(c);
				return castedNode && castedNode->getTag() == GROUP_ID_PROPERTY;
			}
		));
		groupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[inputNodePos = groupIDNodes[1]->getPosition()](CCNode* c) {
				auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
				return castedNode && (castedNode->getPosition() == inputNodePos);
			}
		));
		groupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(groupIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 1 && c->getPositionY() == inputPosY;
			}
		));
		groupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(groupIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 2 && c->getPositionY() == inputPosY;
			}
		));

		auto groupIDInputInfo = STP->commonInputSetup(
			this,
			NID::GROUP,
			GROUP_ID_PROPERTY,
			std::move(groupIDNodes),
			this->m_mainLayer,
			this->m_buttonMenu
		);
		STP->m_fields->m_id_inputs[GROUP_ID_PROPERTY] = std::move(groupIDInputInfo);


		return true;
	}

	void onTargetIDArrow(CCObject* sender)
	{
		SetupAnimationPopup::onTargetIDArrow(sender);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->triggerArrowWasClicked(GROUP_ID_PROPERTY, sender->getTag() == 2);
	}

	void textChanged(CCTextInputNode* input)
	{
		SetupAnimationPopup::textChanged(input);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->textWasChanged(input);
	}
};

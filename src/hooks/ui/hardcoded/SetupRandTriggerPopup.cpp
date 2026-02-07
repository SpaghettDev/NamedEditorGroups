#include <Geode/modify/SetupRandTriggerPopup.hpp>

#include "../SetupPopups.hpp"

#include "utils.hpp"

using namespace geode::prelude;

struct NIDSetupRandTriggerPopup : geode::Modify<NIDSetupRandTriggerPopup, SetupRandTriggerPopup>
{
	static constexpr std::uint16_t TARGET_GROUP_ID_1_PROPERTY = 3;
	static constexpr std::uint16_t TARGET_GROUP_ID_2_PROPERTY = 5;

	bool init(EffectGameObject* p0, CCArray* p1)
	{
		if (!SetupRandTriggerPopup::init(p0, p1)) return false;

		// why robert.....
		ng::utils::cocos::getChildByPredicate(this->m_mainLayer, [](CCNode* c) {
			auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
			return c && c->getTag() == 1;
		})->setTag(-9);

		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		std::vector<CCNode*> groupID1Nodes;
		groupID1Nodes.reserve(5);

		std::vector<CCNode*> groupID2Nodes;
		groupID2Nodes.reserve(5);


		groupID1Nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "Group ID 1";
			}
		));
		groupID1Nodes.emplace_back(this->m_mainLayer->getChildByTag(TARGET_GROUP_ID_1_PROPERTY));
		groupID1Nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[inputNodePos = groupID1Nodes[1]->getPosition()](CCNode* c) {
				auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
				return castedNode && (castedNode->getPosition() == inputNodePos);
			}
		));
		groupID1Nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(groupID1Nodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 1 && c->getPositionY() == inputPosY;
			}
		));
		groupID1Nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(groupID1Nodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 2 && c->getPositionY() == inputPosY;
			}
		));


		groupID2Nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "Group ID 2";
			}
		));
		groupID2Nodes.emplace_back(this->m_mainLayer->getChildByTag(TARGET_GROUP_ID_2_PROPERTY));
		groupID2Nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[inputNodePos = groupID2Nodes[1]->getPosition()](CCNode* c) {
				auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
				return castedNode && (castedNode->getPosition() == inputNodePos);
			}
		));
		groupID2Nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					otherButton = groupID1Nodes[3],
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->convertToWorldSpace(groupID2Nodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 1 && c->getPositionY() == inputPosY && c != otherButton;
			}
		));
		groupID2Nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					otherButton = groupID1Nodes[4],
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->convertToWorldSpace(groupID2Nodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 2 && c->getPositionY() == inputPosY && c != otherButton;
			}
		));


		auto groupID1InputInfo = STP->commonInputSetup(
			this,
			NID::GROUP,
			TARGET_GROUP_ID_1_PROPERTY,
			std::span{ groupID1Nodes },
			this->m_mainLayer,
			this->m_buttonMenu
		);
		STP->m_fields->m_id_inputs.insert({ TARGET_GROUP_ID_1_PROPERTY, std::move(groupID1InputInfo) });

		auto groupID2InputInfo = STP->commonInputSetup(
			this,
			NID::GROUP,
			TARGET_GROUP_ID_2_PROPERTY,
			std::span{ groupID2Nodes },
			this->m_mainLayer,
			this->m_buttonMenu
		);
		STP->m_fields->m_id_inputs.insert({ TARGET_GROUP_ID_2_PROPERTY, std::move(groupID2InputInfo) });

		return true;
	}

	void onTargetIDArrow(CCObject* sender)
	{
		SetupRandTriggerPopup::onTargetIDArrow(sender);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->triggerArrowWasClicked(TARGET_GROUP_ID_1_PROPERTY, sender->getTag() == 2);
	}

	void onTargetID2Arrow(CCObject* sender)
	{
		SetupRandTriggerPopup::onTargetID2Arrow(sender);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->triggerArrowWasClicked(TARGET_GROUP_ID_2_PROPERTY, sender->getTag() == 2);
	}

	void textChanged(CCTextInputNode* input)
	{
		SetupRandTriggerPopup::textChanged(input);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->textWasChanged(input);
	}
};

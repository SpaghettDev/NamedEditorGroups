#include <Geode/modify/SetupCollisionTriggerPopup.hpp>

#include "../SetupPopups.hpp"

#include "utils.hpp"

using namespace geode::prelude;

struct NIDSetupCollisionTriggerPopup : geode::Modify<NIDSetupCollisionTriggerPopup, SetupCollisionTriggerPopup>
{
	static constexpr std::uint16_t BLOCK_A_ID_PROPERTY = 0;
	static constexpr std::uint16_t BLOCK_B_ID_PROPERTY = 1;
	static constexpr std::uint16_t GROUP_ID_PROPERTY = 2;

	bool init(EffectGameObject* p0, CCArray* p1)
	{
		if (!SetupCollisionTriggerPopup::init(p0, p1)) return false;

		// why robert.....
		ng::utils::cocos::getChildByPredicate(this->m_mainLayer, [](CCNode* c) {
			auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
			return c && c->getTag() == 1;
		})->setTag(-9);

		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		std::vector<CCNode*> blockANodes;
		blockANodes.reserve(5);

		std::vector<CCNode*> blockBNodes;
		blockBNodes.reserve(5);

		std::vector<CCNode*> targetIDNodes;
		targetIDNodes.reserve(5);


		blockANodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "BlockA ID";
			}
		));
		blockANodes.emplace_back(this->m_mainLayer->getChildByTag(BLOCK_A_ID_PROPERTY));
		blockANodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[inputNodePos = blockANodes[1]->getPosition()](CCNode* c) {
				auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
				return castedNode && (castedNode->getPosition() == inputNodePos);
			}
		));
		blockANodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(blockANodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 1 && c->getPositionY() == inputPosY;
			}
		));
		blockANodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(blockANodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 2 && c->getPositionY() == inputPosY;
			}
		));


		blockBNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "BlockB ID";
			}
		));
		blockBNodes.emplace_back(this->m_mainLayer->getChildByTag(BLOCK_B_ID_PROPERTY));
		blockBNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[inputNodePos = blockBNodes[1]->getPosition()](CCNode* c) {
				auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
				return castedNode && (castedNode->getPosition() == inputNodePos);
			}
		));
		blockBNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					otherButton = blockANodes[3],
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->convertToWorldSpace(blockBNodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 1 && c->getPositionY() == inputPosY && c != otherButton;
			}
		));
		blockBNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					otherButton = blockANodes[4],
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->convertToWorldSpace(blockBNodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 2 && c->getPositionY() == inputPosY && c != otherButton;
			}
		));


		targetIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "Target ID";
			}
		));
		targetIDNodes.emplace_back(this->m_mainLayer->getChildByTag(GROUP_ID_PROPERTY));
		targetIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[inputNodePos = targetIDNodes[1]->getPosition()](CCNode* c) {
				auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
				return castedNode && (castedNode->getPosition() == inputNodePos);
			}
		));
		targetIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->convertToWorldSpace(targetIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 1 && c->getPositionY() == inputPosY;
			}
		));
		targetIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->convertToWorldSpace(targetIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 2 && c->getPositionY() == inputPosY;
			}
		));

		for (auto child : targetIDNodes)
			child->setPositionY(child->getPositionY() - 10.f);


		auto blockAInputInfo = STP->commonInputSetup(
			this,
			NID::COLLISION,
			BLOCK_A_ID_PROPERTY,
			std::move(blockANodes),
			this->m_mainLayer,
			this->m_buttonMenu
		);
		STP->m_fields->m_id_inputs.insert({ BLOCK_A_ID_PROPERTY, std::move(blockAInputInfo) });

		auto blockBInputInfo = STP->commonInputSetup(
			this,
			NID::COLLISION,
			BLOCK_B_ID_PROPERTY,
			std::move(blockBNodes),
			this->m_mainLayer,
			this->m_buttonMenu
		);
		STP->m_fields->m_id_inputs.insert({ BLOCK_B_ID_PROPERTY, std::move(blockBInputInfo) });

		auto targetIDInputInfo = STP->commonInputSetup(
			this,
			NID::GROUP,
			GROUP_ID_PROPERTY,
			std::move(targetIDNodes),
			this->m_mainLayer,
			this->m_buttonMenu
		);
		STP->m_fields->m_id_inputs.insert({ GROUP_ID_PROPERTY, std::move(targetIDInputInfo) });

		return true;
	}

	void onItemIDArrow(CCObject* sender)
	{
		SetupCollisionTriggerPopup::onItemIDArrow(sender);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->triggerArrowWasClicked(BLOCK_A_ID_PROPERTY, sender->getTag() == 2);
	}

	void onItemID2Arrow(CCObject* sender)
	{
		SetupCollisionTriggerPopup::onItemID2Arrow(sender);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->triggerArrowWasClicked(BLOCK_B_ID_PROPERTY, sender->getTag() == 2);
	}

	void onTargetIDArrow(CCObject* sender)
	{
		SetupCollisionTriggerPopup::onTargetIDArrow(sender);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->triggerArrowWasClicked(GROUP_ID_PROPERTY, sender->getTag() == 2);
	}

	void textChanged(CCTextInputNode* input)
	{
		SetupCollisionTriggerPopup::textChanged(input);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->textWasChanged(input);
	}
};

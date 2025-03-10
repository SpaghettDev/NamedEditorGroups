#include <Geode/modify/SetupInstantCountPopup.hpp>

#include "../SetupPopups.hpp"

#include "utils.hpp"

using namespace geode::prelude;

struct NIDSetupInstantCountPopup : geode::Modify<NIDSetupInstantCountPopup, SetupInstantCountPopup>
{
	static constexpr std::uint16_t ITEM_ID_PROPERTY = 0;
	static constexpr std::uint16_t TARGET_GROUP_ID_PROPERTY = 2;

	bool init(CountTriggerGameObject* p0, CCArray* p1)
	{
		if (!SetupInstantCountPopup::init(p0, p1)) return false;

		// why robert.....
		ng::utils::cocos::getChildByPredicate(this->m_mainLayer, [](CCNode* c) {
			auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
			return c && c->getTag() == 1;
		})->setTag(-9);

		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		std::vector<CCNode*> itemIDNodes;
		itemIDNodes.reserve(5);

		std::vector<CCNode*> targetIDNodes;
		targetIDNodes.reserve(5);


		itemIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "Item ID";
			}
		));
		itemIDNodes.emplace_back(this->m_mainLayer->getChildByTag(ITEM_ID_PROPERTY));
		itemIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[inputNodePos = itemIDNodes[1]->getPosition()](CCNode* c) {
				auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
				return castedNode && (castedNode->getPosition() == inputNodePos);
			}
		));
		itemIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(itemIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 1 && c->getPositionY() == inputPosY;
			}
		));
		itemIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(itemIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 2 && c->getPositionY() == inputPosY;
			}
		));


		targetIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "Target ID";
			}
		));
		targetIDNodes.emplace_back(this->m_mainLayer->getChildByTag(TARGET_GROUP_ID_PROPERTY));
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
					otherButton = itemIDNodes[3],
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->convertToWorldSpace(targetIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 1 && c->getPositionY() == inputPosY && c != otherButton;
			}
		));
		targetIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					otherButton = itemIDNodes[4],
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->convertToWorldSpace(targetIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 2 && c->getPositionY() == inputPosY && c != otherButton;
			}
		));


		auto itemIDInputInfo = STP->commonInputSetup(
			this,
			NID::COUNTER,
			ITEM_ID_PROPERTY,
			std::move(itemIDNodes),
			this->m_mainLayer,
			this->m_buttonMenu
		);
		STP->m_fields->m_id_inputs[ITEM_ID_PROPERTY] = std::move(itemIDInputInfo);

		auto targetIDInputInfo = STP->commonInputSetup(
			this,
			NID::GROUP,
			TARGET_GROUP_ID_PROPERTY,
			std::move(targetIDNodes),
			this->m_mainLayer,
			this->m_buttonMenu
		);
		STP->m_fields->m_id_inputs[TARGET_GROUP_ID_PROPERTY] = std::move(targetIDInputInfo);

		return true;
	}

	void onTargetID2Arrow(CCObject* sender)
	{
		SetupInstantCountPopup::onTargetID2Arrow(sender);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->triggerArrowWasClicked(ITEM_ID_PROPERTY, sender->getTag() == 2);
	}

	void onTargetIDArrow(CCObject* sender)
	{
		SetupInstantCountPopup::onTargetIDArrow(sender);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->triggerArrowWasClicked(TARGET_GROUP_ID_PROPERTY, sender->getTag() == 2);
	}

	void textChanged(CCTextInputNode* input)
	{
		SetupInstantCountPopup::textChanged(input);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->textWasChanged(input);
	}
};

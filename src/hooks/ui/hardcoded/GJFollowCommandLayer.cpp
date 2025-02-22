#include <Geode/modify/GJFollowCommandLayer.hpp>

#include "../SetupPopups.hpp"

#include "utils.hpp"

using namespace geode::prelude;

struct NIDGJFollowCommandLayer : geode::Modify<NIDGJFollowCommandLayer, GJFollowCommandLayer>
{
	static constexpr std::uint16_t TARGET_GROUP_ID_PROPERTY = 2;
	static constexpr std::uint16_t FOLLOW_GROUP_ID_PROPERTY = 4;

	bool init(EffectGameObject* p0, CCArray* p1)
	{
		if (!GJFollowCommandLayer::init(p0, p1)) return false;

		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		std::vector<CCNode*> targetGroupIDNodes;
		targetGroupIDNodes.reserve(4);

		std::vector<CCNode*> followGroupIDNodes;
		followGroupIDNodes.reserve(4);


		auto targetLabelNode = ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "Target\nGroup ID";
			}
		);
		targetGroupIDNodes.emplace_back(this->m_mainLayer->getChildByTag(TARGET_GROUP_ID_PROPERTY));
		targetGroupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[inputNodePos = targetGroupIDNodes[0]->getPosition()](CCNode* c) {
				auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
				return castedNode && (castedNode->getPosition() == inputNodePos);
			}
		));
		targetGroupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					labelPosX = this->m_buttonMenu->convertToNodeSpace(
						this->convertToWorldSpace(targetLabelNode->getPosition())
					).x
			](CCNode* c) {
				return c->getTag() == 2 && c->getPositionX() == labelPosX;
			}
		));
		targetGroupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					labelPosX = this->m_buttonMenu->convertToNodeSpace(
						this->convertToWorldSpace(targetLabelNode->getPosition())
					).x
			](CCNode* c) {
				return c->getTag() == 1 && c->getPositionX() == labelPosX;
			}
		));

		{
			targetGroupIDNodes[2]->setRotation(90.f);
			static_cast<CCMenuItemSpriteExtra*>(
				targetGroupIDNodes[2]
			)->getNormalImage()->setScale(1.f);

			targetGroupIDNodes[3]->setRotation(90.f);
			static_cast<CCMenuItemSpriteExtra*>(
				targetGroupIDNodes[3]
			)->getNormalImage()->setScale(1.f);


			targetLabelNode->setPositionX(targetLabelNode->getPositionX() - 50.f);
			for (auto node : targetGroupIDNodes)
				node->setPosition({ node->getPositionX() - 50.f, node->getPositionY() + 24.f });
		}


		auto followLabelNode = ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "Follow\nGroup ID";
			}
		);
		followGroupIDNodes.emplace_back(this->m_mainLayer->getChildByTag(FOLLOW_GROUP_ID_PROPERTY));
		followGroupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[inputNodePos = followGroupIDNodes[0]->getPosition()](CCNode* c) {
				auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
				return castedNode && (castedNode->getPosition() == inputNodePos);
			}
		));
		followGroupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[&](CCNode* c) {
				return c->getTag() == 2 && c != targetGroupIDNodes[2];
			}
		));
		followGroupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[&](CCNode* c) {
				return c->getTag() == 1 && c != targetGroupIDNodes[3];
			}
		));

		{
			followGroupIDNodes[2]->setRotation(90.f);
			static_cast<CCMenuItemSpriteExtra*>(
				followGroupIDNodes[2]
			)->getNormalImage()->setScale(1.f);

			followGroupIDNodes[3]->setRotation(90.f);
			static_cast<CCMenuItemSpriteExtra*>(
				followGroupIDNodes[3]
			)->getNormalImage()->setScale(1.f);


			followLabelNode->setPositionX(followLabelNode->getPositionX() - 8.f);
			for (auto node : followGroupIDNodes)
				node->setPosition({ node->getPositionX() - 8.f, node->getPositionY() + 24.f});
		}


		auto targetInputInfo = STP->commonSetup(
			NID::GROUP,
			TARGET_GROUP_ID_PROPERTY,
			std::move(targetGroupIDNodes),
			this->m_mainLayer,
			this->m_buttonMenu
		);
		STP->m_fields->m_id_inputs[TARGET_GROUP_ID_PROPERTY] = std::move(targetInputInfo);
		auto followInputInfo = STP->commonSetup(
			NID::GROUP,
			FOLLOW_GROUP_ID_PROPERTY,
			std::move(followGroupIDNodes),
			this->m_mainLayer,
			this->m_buttonMenu
		);
		STP->m_fields->m_id_inputs[FOLLOW_GROUP_ID_PROPERTY] = std::move(followInputInfo);

		this->m_buttonMenu->getChildByID("edit-group-name-button-4"_spr)->setVisible(false);

		return true;
	}

	void onUpdateGroupID(CCObject* sender)
	{
		GJFollowCommandLayer::onUpdateGroupID(sender);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->triggerArrowWasClicked(TARGET_GROUP_ID_PROPERTY, sender->getTag() == 2);
	}

	void onUpdateGroupID2(CCObject* sender)
	{
		GJFollowCommandLayer::onUpdateGroupID2(sender);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->triggerArrowWasClicked(FOLLOW_GROUP_ID_PROPERTY, sender->getTag() == 2);
	}

	void textChanged(CCTextInputNode* input)
	{
		GJFollowCommandLayer::textChanged(input);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->textWasChanged(input);
	}
};

#include <Geode/modify/GJPFollowCommandLayer.hpp>

#include "../SetupPopups.hpp"

#include "utils.hpp"

using namespace geode::prelude;

struct NGGJPFollowCommandLayer : geode::Modify<NGGJPFollowCommandLayer, GJPFollowCommandLayer>
{
	static constexpr std::uint16_t GROUP_ID_PROPERTY = 2;

	bool init(EffectGameObject* p0, CCArray* p1)
	{
		if (!GJPFollowCommandLayer::init(p0, p1)) return false;

		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		std::vector<CCNode*> targetGroupIDNodes;
		targetGroupIDNodes.reserve(4);


		auto targetLabelNode = ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "Target\nGroup ID";
			}
		);
		targetGroupIDNodes.emplace_back(this->m_mainLayer->getChildByTag(GROUP_ID_PROPERTY));
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

		for (auto node : targetGroupIDNodes)
			node->setPosition({ node->getPositionX() + 20.f, node->getPositionY() - 40.f });

		{
			targetGroupIDNodes[2]->setRotation(90.f);
			static_cast<CCMenuItemSpriteExtra*>(
				targetGroupIDNodes[2]
			)->getNormalImage()->setScale(1.f);

			targetGroupIDNodes[3]->setRotation(90.f);
			static_cast<CCMenuItemSpriteExtra*>(
				targetGroupIDNodes[3]
			)->getNormalImage()->setScale(1.f);


			targetLabelNode->setPosition({
				targetLabelNode->getPositionX() - 25.f,
				targetLabelNode->getPositionY() - 35.f
			});
			for (auto node : targetGroupIDNodes)
				node->setPosition({ node->getPositionX() - 50.f, node->getPositionY() + 24.f });
		}

		auto targetInputInfo = STP->commonInputSetup(
			this,
			NID::GROUP,
			GROUP_ID_PROPERTY,
			std::span{ targetGroupIDNodes },
			this->m_mainLayer,
			this->m_buttonMenu
		);
		STP->m_fields->m_id_inputs.insert({ GROUP_ID_PROPERTY, std::move(targetInputInfo) });

		return true;
	}

	void onUpdateGroupID(CCObject* sender)
	{
		GJPFollowCommandLayer::onUpdateGroupID(sender);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->triggerArrowWasClicked(GROUP_ID_PROPERTY, sender->getTag() == 2);
	}

	void textChanged(CCTextInputNode* input)
	{
		GJPFollowCommandLayer::textChanged(input);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->textWasChanged(input);
	}
};

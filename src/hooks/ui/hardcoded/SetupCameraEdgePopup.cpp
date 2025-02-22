#include <Geode/modify/SetupCameraEdgePopup.hpp>

#include "../SetupPopups.hpp"

#include "utils.hpp"

using namespace geode::prelude;

struct NIDSetupCameraEdgePopup : geode::Modify<NIDSetupCameraEdgePopup, SetupCameraEdgePopup>
{
	static constexpr std::uint16_t TARGET_GROUP_ID_PROPERTY = 51;

	bool init(CameraTriggerGameObject* p0, CCArray* p1)
	{
		if (!SetupCameraEdgePopup::init(p0, p1)) return false;

		// why robert.....
		ng::utils::cocos::getChildByPredicate(this->m_mainLayer, [](CCNode* c) {
			auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
			return c && c->getTag() == 1;
		})->setTag(-9);

		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		std::vector<CCNode*> targetIDNodes;
		targetIDNodes.reserve(5);


		targetIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "Target ID";
			}
		));
		targetIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[labelPosX = targetIDNodes[0]->getPositionX()](CCNode* c) {
				auto castedNode = typeinfo_cast<CCTextInputNode*>(c);
				return castedNode && castedNode->getPositionX() == labelPosX;
			}
		));
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
						this->m_mainLayer->convertToWorldSpace(targetIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 1 && c->getPositionY() == inputPosY;
			}
		));
		targetIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(targetIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 2 && c->getPositionY() == inputPosY;
			}
		));


		// thank you robert
		targetIDNodes[1]->setTag(TARGET_GROUP_ID_PROPERTY);


		auto targetIDInputInfo = STP->commonSetup(
			NID::GROUP,
			TARGET_GROUP_ID_PROPERTY,
			std::move(targetIDNodes),
			this->m_mainLayer,
			this->m_buttonMenu
		);
		STP->m_fields->m_id_inputs[TARGET_GROUP_ID_PROPERTY] = std::move(targetIDInputInfo);

		return true;
	}

	void onTargetIDArrow(CCObject* sender)
	{
		SetupCameraEdgePopup::onTargetIDArrow(sender);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->triggerArrowChanged(TARGET_GROUP_ID_PROPERTY, sender->getTag() == 2);
	}

	void textChanged(CCTextInputNode* input)
	{
		SetupCameraEdgePopup::textChanged(input);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->textChanged(input);
	}

	void onUnlockEdge(CCObject* sender)
	{
		SetupCameraEdgePopup::onUnlockEdge(sender);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->textChanged(
			static_cast<CCTextInputNode*>(this->m_mainLayer->getChildByTag(TARGET_GROUP_ID_PROPERTY))
		);
	}
};

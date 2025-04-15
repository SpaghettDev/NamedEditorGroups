#include <Geode/modify/SetupPulsePopup.hpp>

#include "../SetupPopups.hpp"

#include "utils.hpp"

using namespace geode::prelude;

struct NIDSetupPulsePopup : geode::Modify<NIDSetupPulsePopup, SetupPulsePopup>
{
	static constexpr std::uint16_t GROUP_OR_CHANNEL_ID_PROPERTY = 4;
	static constexpr std::uint16_t COLOR_ID_PROPERTY = 5;

	bool init(EffectGameObject* p0, CCArray* p1)
	{
		if (!SetupPulsePopup::init(p0, p1)) return false;

		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		std::vector<CCNode*> colorIDNodes;
		colorIDNodes.reserve(5);

		std::vector<CCNode*> groupIDNodes;
		groupIDNodes.reserve(5);


		colorIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			// why in the everliving fuck rob
			this,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "Color ID";
			}
		));
		colorIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCTextInputNode*>(c);
				return castedNode && castedNode->getTag() == COLOR_ID_PROPERTY;
			}
		));
		colorIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[inputNodePos = colorIDNodes[1]->getPosition()](CCNode* c) {
				auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
				return castedNode && (castedNode->getPosition() == inputNodePos);
			}
		));
		colorIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(colorIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 1 && c->getPositionY() == inputPosY;
			}
		));
		colorIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(colorIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 2 && c->getPositionY() == inputPosY;
			}
		));


		groupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && (
					std::string_view(castedNode->getString()) == "Group ID" ||
					std::string_view(castedNode->getString()) == "Channel ID"
				);
			}
		));
		groupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCTextInputNode*>(c);
				return castedNode && castedNode->getTag() == GROUP_OR_CHANNEL_ID_PROPERTY;
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


		auto colorIDInputInfo = STP->commonInputSetup(
			this,
			NID::COLOR,
			COLOR_ID_PROPERTY,
			std::move(colorIDNodes),
			this->m_mainLayer,
			this->m_buttonMenu
		);
		colorIDInputInfo.editInputButton->setVisible(this->m_pulseMode == 1);
		colorIDInputInfo.namedIDInput->setVisible(this->m_pulseMode == 1);
		STP->m_fields->m_id_inputs[COLOR_ID_PROPERTY] = std::move(colorIDInputInfo);

		auto groupIDInputInfo = STP->commonInputSetup(
			this,
			this->m_pulseTargetType == 1 ? NID::GROUP : NID::COLOR,
			GROUP_OR_CHANNEL_ID_PROPERTY,
			std::move(groupIDNodes),
			this->m_mainLayer,
			this->m_buttonMenu
		);
		STP->m_fields->m_id_inputs[GROUP_OR_CHANNEL_ID_PROPERTY] = std::move(groupIDInputInfo);

		return true;
	}

	void onSelectTargetMode(CCObject* sender)
	{
		SetupPulsePopup::onSelectTargetMode(sender);

		auto& inputs = reinterpret_cast<NIDSetupTriggerPopup*>(this)->m_fields->m_id_inputs;
		if (!inputs.contains(GROUP_OR_CHANNEL_ID_PROPERTY)) return;

		auto& idInputInfo = inputs.at(GROUP_OR_CHANNEL_ID_PROPERTY);

		idInputInfo.idType = this->m_pulseTargetType == 1 ? NID::GROUP : NID::COLOR;
	}

	void onSelectPulseMode(CCObject* sender)
	{
		SetupPulsePopup::onSelectPulseMode(sender);

		auto& inputs = reinterpret_cast<NIDSetupTriggerPopup*>(this)->m_fields->m_id_inputs;
		if (!inputs.contains(COLOR_ID_PROPERTY)) return;

		auto& idInputInfo = inputs.at(COLOR_ID_PROPERTY);

		idInputInfo.editInputButton->setVisible(this->m_pulseMode == 1);
		idInputInfo.namedIDInput->setVisible(this->m_pulseMode == 1);
	}

	void onUpdateCustomColor(CCObject* sender)
	{
		SetupPulsePopup::onUpdateCustomColor(sender);

		if (this->m_pulseTargetType == 1)
			reinterpret_cast<NIDSetupTriggerPopup*>(this)->triggerArrowWasClicked(GROUP_OR_CHANNEL_ID_PROPERTY, sender->getTag() == 2);
	}

	void onUpdateCopyColor(CCObject* sender)
	{
		SetupPulsePopup::onUpdateCopyColor(sender);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->triggerArrowWasClicked(COLOR_ID_PROPERTY, sender->getTag() == 2);
	}

	void textChanged(CCTextInputNode* input)
	{
		SetupPulsePopup::textChanged(input);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->textWasChanged(input);
	}
};

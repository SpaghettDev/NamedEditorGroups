#include "SetupPopups.hpp"

#include <array>
#include <unordered_map>

#include <Geode/modify/SetupMoveCommandPopup.hpp>
#include <Geode/modify/SetupTriggerPopup.hpp>

#include "../popups/EditNamedIDPopup.hpp"

#include <NIDManager.hpp>

#include "constants.hpp"

using namespace geode::prelude;

cocos2d::CCArray* NIDSetupTriggerPopup::createValueControlAdvanced(int property, gd::string label, CCPoint position, float scale, bool unk1, InputValueType valueType, int unk2, bool unk3, float sliderMin, float sliderMax, int page, int group, GJInputStyle inputStyle, int decimalPlaces, bool unk4)
{
	// it seems like the game does nothing with the returned array, but we should add our stuff either way
	// in case it ever does
	auto nodes = SetupTriggerPopup::createValueControlAdvanced(property, label, position, scale, unk1, valueType, unk2, unk3, sliderMin, sliderMax, page, group, inputStyle, decimalPlaces, unk4);

	if (
		// this method is called from LevelOptionsLayer :broken_heart:
		!this->m_gameObject ||
		!ng::constants::OBJECT_ID_TO_PROPERTIES_INFO.contains(this->m_gameObject->m_objectID) ||
		!ng::constants::OBJECT_ID_TO_PROPERTIES_INFO.at(this->m_gameObject->m_objectID).contains(property)
	)
		return nodes;

	CCArray* pageContainer = static_cast<CCArray*>(this->m_pageContainers->objectAtIndex(page));

	handleSpecialCases(this->m_gameObject->m_objectID, property, nodes);

	auto inputInfo = commonSetup(
		ng::constants::OBJECT_ID_TO_PROPERTIES_INFO.at(this->m_gameObject->m_objectID).at(property),
		property,
		std::move(geode::cocos::ccArrayToVector<CCNode*>(nodes)),
		this->m_mainLayer,
		this->m_buttonMenu,
		[&](std::vector<CCNode*>&& newNodes) {
			for (auto& node : newNodes)
			{
				nodes->addObject(node);
				pageContainer->addObject(node);
			}
		}
	);
	m_fields->m_id_inputs[property] = std::move(inputInfo);

	return nodes;
}

void NIDSetupTriggerPopup::triggerArrowLeft(CCObject* sender)
{
	SetupTriggerPopup::triggerArrowLeft(sender);

	triggerArrowWasClicked(sender->getTag(), false);
}

void NIDSetupTriggerPopup::triggerArrowRight(CCObject* sender)
{
	SetupTriggerPopup::triggerArrowRight(sender);

	triggerArrowWasClicked(sender->getTag(), true);
}

void NIDSetupTriggerPopup::textChanged(CCTextInputNode* input)
{
	SetupTriggerPopup::textChanged(input);

	textWasChanged(input);
}


void NIDSetupTriggerPopup::triggerArrowWasClicked(int senderTag, bool isRight)
{
	auto inputNode = static_cast<CCTextInputNode*>(this->m_mainLayer->getChildByTag(senderTag));

	if (!m_fields->m_id_inputs.contains(senderTag)) return;

	auto parsedIdInputValue = geode::utils::numFromString<short>(inputNode->getString());
	if (parsedIdInputValue.isErr()) return;
	short idInputValue = parsedIdInputValue.unwrap();

	auto& idInputInfo = m_fields->m_id_inputs.at(senderTag);

	idInputInfo.namedIDInput->getInputNode()->onClickTrackNode(false);
	idInputInfo.namedIDInput->setString(
		NIDManager::getNameForID(evaluateDynamicType(idInputInfo.idType, senderTag), idInputValue).unwrapOr("")
	);
}

void NIDSetupTriggerPopup::textWasChanged(CCTextInputNode* input)
{
	if (!m_fields->m_id_inputs.contains(input->getTag())) return;

	auto& idInputInfo = m_fields->m_id_inputs.at(input->getTag());

	if (auto parsedNum = numFromString<short>(idInputInfo.idInput->getString()); parsedNum.isOk())
		idInputInfo.namedIDInput->setString(
			NIDManager::getNameForID(evaluateDynamicType(idInputInfo.idType, input->getTag()), parsedNum.unwrap()).unwrapOr("")
		);
}

void NIDSetupTriggerPopup::onEditIDNameButton(CCObject* sender)
{
	auto& idInputInfo = m_fields->m_id_inputs.at(sender->getTag());

	ShowEditNamedIDPopup(
		evaluateDynamicType(idInputInfo.idType, sender->getTag()),
		geode::utils::numFromString<short>(idInputInfo.idInput->getString()).unwrapOr(0),
		[&](short id) {
			idInputInfo.idInput->setString(fmt::format("{}", id));
		},
		[&] {
			this->textChanged(idInputInfo.idInput);
		}
	);
}

void NIDSetupTriggerPopup::onEditInput(NIDSetupTriggerPopup* self, std::uint16_t property, const std::string& str)
{
	auto& idInputInfo = self->m_fields->m_id_inputs.at(property);

	if (auto name = NIDManager::getIDForName(self->evaluateDynamicType(idInputInfo.idType, property), str); name.isOk())
		idInputInfo.idInput->setString(fmt::format("{}", name.unwrap()));
}

NIDSetupTriggerPopup::IDInputInfo NIDSetupTriggerPopup::commonSetup(NID nid, std::uint16_t property, std::vector<CCNode*>&& nodes, CCNode* mainLayer, CCNode* buttonMenu, std::function<void(std::vector<CCNode*>&&)>&& finishedCallback)
{
	NIDSetupTriggerPopup::IDInputInfo inputInfo;
	CCPoint inputNodePos;
	float scale = 1.f;

	inputInfo.idType = nid;
	NID currentNid = evaluateDynamicType(nid, property);

	for (auto node : nodes)
		if (auto bg = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(node))
		{
			scale = node->getScale();
			bg->setContentSize({ 50.f, 30.f });
			break;
		}

	for (auto node : nodes)
	{
		if (typeinfo_cast<CCLabelBMFont*>(node))
		{
			node->setPositionY(node->getPositionY() + 11.f * scale);
			node->setScale(node->getScale() + .1f);
		}
		else
			node->setPositionY(node->getPositionY() + 15.f * scale);

		if (auto buttonNode = typeinfo_cast<CCMenuItemSpriteExtra*>(node))
			buttonNode->getNormalImage()->setScale(buttonNode->getScale() - .1f);
		else
			node->setScale(node->getScale() - .1f);

		if (auto castedNode = typeinfo_cast<CCTextInputNode*>(node))
		{
			inputInfo.idInput = castedNode;
			inputNodePos = castedNode->getPosition();

			castedNode->setPositionY(castedNode->getPositionY() + 1.f);
			castedNode->setScale(castedNode->getScale() + .1f);
		}
		else if (auto castedNode = typeinfo_cast<CCMenuItemSpriteExtra*>(node))
			inputInfo.arrowButtons[!inputInfo.arrowButtons[0] ? 0 : 1] = castedNode;
	}

	for (auto arrowBtn : inputInfo.arrowButtons)
	{
		if (!arrowBtn) break;

		CCPoint pos = buttonMenu->convertToNodeSpace(inputInfo.idInput->getPosition());
		arrowBtn->setPosition({ pos.x + (arrowBtn == inputInfo.arrowButtons[0] ? -1 : 1) * 40.f * (scale - .1f), pos.y });
		arrowBtn->getNormalImage()->setScale(scale);
	}

	auto groupNameInput = geode::TextInput::create(110.f, "Unnamed");
	groupNameInput->setContentHeight(20.f);
	groupNameInput->setFilter(ng::constants::VALID_NAMED_ID_CHARACTERS);
	groupNameInput->setCallback([&, property](const std::string& str) {
		NIDSetupTriggerPopup::onEditInput(this, property, std::move(str));
	});
	groupNameInput->setPosition({ inputNodePos.x, inputNodePos.y - 30.f * scale });
	groupNameInput->setID(fmt::format("group-name-input-{}"_spr, property));
	groupNameInput->getBGSprite()->setContentSize({ 220.f, 55.f });
	groupNameInput->setScale(std::clamp(scale - .1f, .1f, .8f));
	mainLayer->addChild(groupNameInput);
	inputInfo.namedIDInput = groupNameInput;

	auto editInputButtonSprite = CCSprite::create("pencil.png"_spr);
	editInputButtonSprite->setScale(std::clamp((scale - .1f) - .2f, .1f, .4f));
	auto editInputButton = CCMenuItemSpriteExtra::create(
		editInputButtonSprite,
		this,
		menu_selector(NIDSetupTriggerPopup::onEditIDNameButton)
	);
	editInputButton->setTag(property);
	editInputButton->setPosition(
		buttonMenu->convertToNodeSpace(
			this->convertToWorldSpace({
				inputNodePos.x + groupNameInput->getScaledContentWidth() / 2.f + 15.f * scale,
				groupNameInput->getPositionY() + groupNameInput->getScaledContentHeight() / 2.f - 4.f * scale
			})
		)
	);
	editInputButton->setID(fmt::format("edit-group-name-button-{}"_spr, property));
	buttonMenu->addChild(editInputButton);
	inputInfo.editInputButton = editInputButton;

	auto inputIDNum = numFromString<short>(inputInfo.idInput->getString()).unwrapOr(0);

	if (auto name = NIDManager::getNameForID(currentNid, inputIDNum); name.isOk())
		groupNameInput->setString(name.unwrap());

	if (finishedCallback)
		finishedCallback({ groupNameInput, editInputButton });

	return std::move(inputInfo);
}

void NIDSetupTriggerPopup::handleSpecialCases(int gameObjectID, std::uint16_t property, CCArray* nodes)
{
	switch (gameObjectID)
	{
		// Edit Area Move trigger
		case 3011u:
		{
			if (property == 51)
				for (auto node : CCArrayExt<CCNode*>(nodes))
					node->setPositionY(node->getPositionY() - 10.f);

			break;
		}

		default:
			break;
	}
}

NID NIDSetupTriggerPopup::evaluateDynamicType(NID nid, short tag) {
	if (nid != NID::DYNAMIC_COUNTER_TIMER) return nid;

	auto& toggleMap = ng::constants::DNAMIC_PROPERTIES_TOGGLES.at(this->m_gameObject->m_objectID);
	auto& toggleInfo = toggleMap.at(tag);
	auto propVal = this->getTriggerValue(toggleInfo.togglePropID, this->m_gameObject);

	return propVal == toggleInfo.counterState ? NID::COUNTER : NID::TIMER;
}

// Persistent Item & Counter (Binary Toggles)
void NIDSetupTriggerPopup::valueChanged(int property, float value) {
	SetupTriggerPopup::valueChanged(property, value);

	if (!ng::constants::DNAMIC_PROPERTIES_TOGGLES.contains(this->m_gameObject->m_objectID)) return;
	for (auto const& [key, val] : ng::constants::DNAMIC_PROPERTIES_TOGGLES.at(this->m_gameObject->m_objectID)) {
		if (val.togglePropID != property) continue;

		NID type = val.counterState == value ? NID::COUNTER : NID::TIMER;

		auto inputNode = typeinfo_cast<CCTextInputNode*>(this->m_mainLayer->getChildByTag(key));

		if (!m_fields->m_id_inputs.contains(key)) return;

		auto parsedIdInputValue = geode::utils::numFromString<short>(inputNode->getString());
		if (parsedIdInputValue.isErr()) return;
		short idInputValue = parsedIdInputValue.unwrap();

		auto& idInputInfo = m_fields->m_id_inputs.at(key);

		idInputInfo.namedIDInput->getInputNode()->onClickTrackNode(false);
		idInputInfo.namedIDInput->setString(
			NIDManager::getNameForID(type, idInputValue).unwrapOr("")
		);
		break;
	}
};

// Item Edit & Item Comp (Multiple Choice Toggles)
void NIDSetupTriggerPopup::updateValue(int property, float value) {
	SetupTriggerPopup::updateValue(property, value);

	if (!ng::constants::DNAMIC_PROPERTIES_CHOICES.contains(this->m_gameObject->m_objectID)) return;
	auto& choiceProperties = ng::constants::DNAMIC_PROPERTIES_CHOICES.at(this->m_gameObject->m_objectID);

	if (!choiceProperties.contains(property)) return;
	auto& choiceInfo = choiceProperties.at(property);

	auto targetTag = choiceInfo.targetPropID;
	NID type = choiceInfo.timerState == value ? NID::TIMER :
		choiceInfo.counterState == value ? NID::COUNTER : NID::DYNAMIC_COUNTER_TIMER;

	auto inputNode = typeinfo_cast<CCTextInputNode*>(this->m_mainLayer->getChildByTag(targetTag));

	if (!m_fields->m_id_inputs.contains(targetTag)) return;

	auto parsedIdInputValue = geode::utils::numFromString<short>(inputNode->getString());
	if (parsedIdInputValue.isErr()) return;
	short idInputValue = parsedIdInputValue.unwrap();

	auto& idInputInfo = m_fields->m_id_inputs.at(targetTag);

	idInputInfo.namedIDInput->getInputNode()->onClickTrackNode(false);
	idInputInfo.namedIDInput->setString(
		type == NID::DYNAMIC_COUNTER_TIMER ? "" :
		NIDManager::getNameForID(type, idInputValue).unwrapOr("")
	);
};
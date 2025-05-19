#include "SetupPopups.hpp"

#include <array>
#include <unordered_map>

#include <Geode/modify/SetupTriggerPopup.hpp>
#include <Geode/modify/SetupAreaAnimTriggerPopup.hpp>

#include "../popups/EditNamedIDPopup.hpp"

#include <NIDManager.hpp>

#include "globals.hpp"
#include "constants.hpp"
#include "DynamicPropertyTypes.hpp"

using namespace geode::prelude;


cocos2d::CCArray* NIDSetupTriggerPopup::createValueControlAdvanced(
	int property, gd::string label, CCPoint position,
	float scale, bool unk1, InputValueType valueType,
	int unk2, bool unk3, float sliderMin,
	float sliderMax, int page, int group,
	GJInputStyle inputStyle, int decimalPlaces, bool unk4
) {
	// it seems like the game does nothing with the returned array, but we should add our stuff either way
	// in case it ever does
	auto nodes = SetupTriggerPopup::createValueControlAdvanced(
		property, label, position,
		scale, unk1, valueType,
		unk2, unk3, sliderMin,
		sliderMax, page, group,
		inputStyle, decimalPlaces, unk4
	);

	// this is -1 even if m_gameObjects has different objects since this method is never called if
	// the game objects are different
	m_fields->m_objectID = this->m_gameObject
		? this->m_gameObject->m_objectID
		: this->m_gameObjects
			? this->m_gameObjects->count()
				? static_cast<GameObject*>(this->m_gameObjects->objectAtIndex(0))->m_objectID
				: -1
			: -1;

	if (this->m_gameObject || this->m_gameObjects)
	{
		if (
			m_fields->m_objectID == -1 ||
			!ng::constants::OBJECT_ID_TO_PROPERTIES_INFO.contains(m_fields->m_objectID) ||
			!ng::constants::OBJECT_ID_TO_PROPERTIES_INFO.at(m_fields->m_objectID).contains(property) ||
			// see OBJECT_ID_TO_PROPERTIES_INFO comment
			ng::constants::OBJECT_ID_TO_PROPERTIES_INFO.at(m_fields->m_objectID).at(property) == NID::_UNKNOWN
		)
			return nodes;
	}
	else
	{
		// this method is called from LevelOptionsLayer :broken_heart:
		if (!typeinfo_cast<LevelOptionsLayer*>(this))
			return nodes;
	}

	CCArray* pageContainer = static_cast<CCArray*>(this->m_pageContainers->objectAtIndex(page));
	CCArray* groupContainer = static_cast<CCArray*>(this->m_groupContainers->objectAtIndex(group));

	handleSpecialCasesPre(property, nodes);

	auto inputInfo = commonInputSetup(
		this,
		ng::constants::OBJECT_ID_TO_PROPERTIES_INFO.at(m_fields->m_objectID).at(property),
		property,
		std::move(geode::cocos::ccArrayToVector<CCNode*>(nodes)),
		this->m_mainLayer,
		this->m_buttonMenu,
		[&](std::vector<CCNode*>&& newNodes) {
			for (auto& node : newNodes)
			{
				nodes->addObject(node);
				pageContainer->addObject(node);
				groupContainer->addObject(node);
			}
		}
	);
	m_fields->m_id_inputs[property] = std::move(inputInfo);

	handleSpecialCasesPost(property, nodes);

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

void NIDSetupTriggerPopup::onToggleTriggerValue(CCObject* sender)
{
	SetupTriggerPopup::onToggleTriggerValue(sender);

	// Use Control ID
	if (sender->getTag() == 535)
	{
		if (!m_fields->m_id_inputs.contains(51)) return;

		auto& inputNodeInfo = m_fields->m_id_inputs.at(51);

		inputNodeInfo.namedIDInput->setEnabled(this->getValue(535) == .0f);
		inputNodeInfo.editInputButton->setEnabled(this->getValue(535) == .0f);
		inputNodeInfo.namedIDInput->getInputNode()->setDelegate(nullptr);
		inputNodeInfo.namedIDInput->setString("");
	}
}

void NIDSetupTriggerPopup::textChanged(CCTextInputNode* input)
{
	SetupTriggerPopup::textChanged(input);

	textWasChanged(input);
}


void NIDSetupTriggerPopup::triggerArrowWasClicked(int senderTag, bool isRight)
{
	if (!m_fields->m_id_inputs.contains(senderTag)) return;

	auto& idInputInfo = m_fields->m_id_inputs.at(senderTag);

	auto parsedIdInputValue = geode::utils::numFromString<short>(idInputInfo.idInput->getString());
	if (parsedIdInputValue.isErr()) return;
	short idInputValue = parsedIdInputValue.unwrap();

	idInputInfo.namedIDInput->getInputNode()->onClickTrackNode(false);
	idInputInfo.namedIDInput->setString(
		NIDManager::getNameForID(
			evaluateDynamicType(this, idInputInfo.idType, senderTag),
			idInputValue
		).unwrapOr("")
	);
}

void NIDSetupTriggerPopup::textWasChanged(CCTextInputNode* input)
{
	if (!m_fields->m_id_inputs.contains(input->getTag())) return;

	auto& idInputInfo = m_fields->m_id_inputs.at(input->getTag());

	if (auto parsedNum = numFromString<short>(idInputInfo.idInput->getString()); parsedNum.isOk())
		idInputInfo.namedIDInput->setString(
			NIDManager::getNameForID(
				evaluateDynamicType(this, idInputInfo.idType, input->getTag()),
				parsedNum.unwrap()
			).unwrapOr("")
		);
}

void NIDSetupTriggerPopup::onEditIDNameButton(CCObject* sender)
{
	auto& idInputInfo = m_fields->m_id_inputs.at(sender->getTag());

	ShowEditNamedIDPopup(
		evaluateDynamicType(this, idInputInfo.idType, sender->getTag()),
		geode::utils::numFromString<short>(idInputInfo.idInput->getString()).unwrapOr(0),
		[&](short id) {
			idInputInfo.idInput->setString(fmt::format("{}", id));
			this->textChanged(idInputInfo.idInput);
		},
		[&] {
			this->textChanged(idInputInfo.idInput);
		},
		// color triggers
		(
			m_fields->m_objectID == 900 ||
			m_fields->m_objectID == 899 ||
			m_fields->m_objectID == 105 ||
			m_fields->m_objectID == 30 ||
			m_fields->m_objectID == 29
		) && ng::globals::g_isBetterColorPickerLoaded
	);
}

void NIDSetupTriggerPopup::onEditInput(NIDSetupTriggerPopup* self, std::uint16_t property, const std::string& str)
{
	auto& idInputInfo = self->m_fields->m_id_inputs.at(property);

	if (auto id = NIDManager::getIDForName(evaluateDynamicType(self, idInputInfo.idType, property), str); id.isOk())
	{
		idInputInfo.idInput->setString(fmt::format("{}", id.unwrap()));
		self->textChanged(idInputInfo.idInput);
	}
}

NIDSetupTriggerPopup::IDInputInfo NIDSetupTriggerPopup::commonInputSetup(
	CCLayer* self, NID nid, std::uint16_t property,
	std::vector<CCNode*>&& nodes, CCNode* mainLayer, CCNode* buttonMenu,
	std::function<void(std::vector<CCNode*>&&)>&& finishedCallback
) {
	NIDSetupTriggerPopup::IDInputInfo inputInfo;
	CCPoint inputNodePos;
	float scale = 1.f;

	inputInfo.idType = nid;
	NID currentNid = evaluateDynamicType(
		static_cast<SetupTriggerPopup*>(self),
		nid,
		property
	);

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

	auto groupNameInput = geode::TextInput::create(110.f, "Search...");
	groupNameInput->setContentHeight(20.f);
	groupNameInput->setFilter(ng::constants::VALID_NAMED_ID_CHARACTERS);
	groupNameInput->setPosition({ inputNodePos.x, inputNodePos.y - 30.f * scale });
	groupNameInput->setID(fmt::format("group-name-input-{}"_spr, property));
	groupNameInput->getBGSprite()->setContentSize({ 220.f, 55.f });
	groupNameInput->setScale(std::clamp(scale - .1f, .1f, .8f));
	mainLayer->addChild(groupNameInput);

	auto editInputButtonSprite = CCSprite::create("pencil.png"_spr);
	editInputButtonSprite->setScale(std::clamp((scale - .1f) - .2f, .1f, .4f));
	auto editInputButton = CCMenuItemSpriteExtra::create(
		editInputButtonSprite,
		self,
		menu_selector(NIDSetupTriggerPopup::onEditIDNameButton)
	);
	editInputButton->setTag(property);
	editInputButton->setPosition(
		buttonMenu->convertToNodeSpace(
			self->convertToWorldSpace({
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

	finishedCallback({ groupNameInput, editInputButton });

	inputInfo.namedIDInput = AutofillInput{
		nid, groupNameInput,
		[self, property](const std::string& str) {
			NIDSetupTriggerPopup::onEditInput(static_cast<NIDSetupTriggerPopup*>(self), property, str);
		},
		[idInput = inputInfo.idInput](NID nid, short id) {
			idInput->setString(fmt::format("{}", id));
		}
	};

	return inputInfo;
}

void NIDSetupTriggerPopup::handleSpecialCasesPre(std::uint16_t property, CCArray* nodes)
{
	switch (m_fields->m_objectID)
	{
		// Edit Area Move Trigger
		case 3011u: {
			if (property == 51)
				for (auto node : CCArrayExt<CCNode*>(nodes))
					node->setPositionY(node->getPositionY() - 10.f);
		}
		break;

		// Shockwave Trigger
		case 2905u: {
			if (property == 51)
				for (auto node : CCArrayExt<CCNode*>(nodes))
					node->setPositionY(node->getPositionY() - 2.f);
		}
		break;

		// Grayscale Trigger
		case 2919u: {
			for (auto node : CCArrayExt<CCNode*>(nodes))
				if (auto label = typeinfo_cast<CCLabelBMFont*>(node))
				{
					label->setPositionX(label->getPositionX() - 30.f);
					break;
				}
		}
		break;

		default:
			break;
	}
}

void NIDSetupTriggerPopup::handleSpecialCasesPost(std::uint16_t property, CCArray* nodes)
{
	switch (m_fields->m_objectID)
	{
		// Grayscale Trigger
		case 2919u: {
			for (auto node : CCArrayExt<CCNode*>(nodes))
				if (auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(node))
				{
					button->setPositionX(button->getPositionX() - 90.f);
					break;
				}
		}

		default:
			break;
	}
}

NID NIDSetupTriggerPopup::evaluateDynamicType(SetupTriggerPopup* self, NID nid, short property)
{
	if (nid != NID::DYNAMIC_COUNTER_TIMER) return nid;

	GameObject* obj = self->m_gameObject
		? self->m_gameObject
		: static_cast<GameObject*>(self->m_gameObjects->objectAtIndex(0));

	auto& toggleMap = ng::constants::DYNAMIC_PROPERTIES_TOGGLES.at(obj->m_objectID);
	auto& toggleInfo = toggleMap.at(property);
	auto propVal = self->getTriggerValue(toggleInfo.togglePropID, obj);

	return propVal == toggleInfo.counterState ? NID::COUNTER : NID::TIMER;
}

void NIDSetupTriggerPopup::valueChanged(int property, float value)
{
	SetupTriggerPopup::valueChanged(property, value);

	if (!ng::constants::DYNAMIC_PROPERTIES_TOGGLES.contains(m_fields->m_objectID)) return;

	for (auto const& [key, val] : ng::constants::DYNAMIC_PROPERTIES_TOGGLES.at(m_fields->m_objectID))
	{
		if (val.togglePropID != property) continue;

		NID type = val.counterState == value ? NID::COUNTER : NID::TIMER;

		if (!m_fields->m_id_inputs.contains(key)) return;

		auto& idInputInfo = m_fields->m_id_inputs.at(key);

		auto parsedIdInputValue = geode::utils::numFromString<short>(idInputInfo.idInput->getString());
		if (parsedIdInputValue.isErr()) return;
		short idInputValue = parsedIdInputValue.unwrap();

		idInputInfo.namedIDInput->getInputNode()->onClickTrackNode(false);
		idInputInfo.namedIDInput->setString(
			NIDManager::getNameForID(type, idInputValue).unwrapOr("")
		);
		break;
	}
}

void NIDSetupTriggerPopup::updateValue(int property, float value)
{
	SetupTriggerPopup::updateValue(property, value);

	if (!ng::constants::DYNAMIC_PROPERTIES_CHOICES.contains(m_fields->m_objectID)) return;
	auto& choiceProperties = ng::constants::DYNAMIC_PROPERTIES_CHOICES.at(m_fields->m_objectID);

	if (!choiceProperties.contains(property)) return;
	auto& choiceInfo = choiceProperties.at(property);

	auto targetTag = choiceInfo.targetPropID;
	NID type = choiceInfo.timerState == value
		? NID::TIMER
		: choiceInfo.counterState == value
			? NID::COUNTER
			: NID::DYNAMIC_COUNTER_TIMER;

	if (!m_fields->m_id_inputs.contains(targetTag)) return;

	auto& idInputInfo = m_fields->m_id_inputs.at(targetTag);

	auto parsedIdInputValue = geode::utils::numFromString<short>(idInputInfo.idInput->getString());
	if (parsedIdInputValue.isErr()) return;
	short idInputValue = parsedIdInputValue.unwrap();

	idInputInfo.namedIDInput->getInputNode()->onClickTrackNode(false);
	idInputInfo.namedIDInput->setString(
		type == NID::DYNAMIC_COUNTER_TIMER
			? ""
			: NIDManager::getNameForID(type, idInputValue).unwrapOr("")
	);
}

void NIDSetupTriggerPopup::goToPage(int page, bool hideAll)
{
	SetupTriggerPopup::goToPage(page, hideAll);

	m_fields->m_page_change_cb(page);
}


// SetupAreaAnimTriggerPopups are special
struct NIDSetupAreaAnimTriggerPopup : geode::Modify<NIDSetupAreaAnimTriggerPopup, SetupAreaAnimTriggerPopup>
{
	void updateTargetIDLabel()
	{
		SetupAreaAnimTriggerPopup::updateTargetIDLabel();

		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		if (STP->m_fields->m_id_inputs.contains(51))
		{
			auto& inputInfo = STP->m_fields->m_id_inputs.at(51);
			bool isEffectID = this->getValue(355) != .0f;

			inputInfo.idType = isEffectID ? NID::EFFECT : NID::GROUP;
			this->textChanged(inputInfo.idInput);
		}
	}
};

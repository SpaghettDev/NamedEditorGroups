#include <Geode/modify/ColorSelectPopup.hpp>

#include <NIDManager.hpp>

#include "../SetupPopups.hpp"
#include "../popups/EditNamedIDPopup.hpp"

#include "utils.hpp"
#include "constants.hpp"

using namespace geode::prelude;

struct NIDColorSelectPopup : geode::Modify<NIDColorSelectPopup, ColorSelectPopup>
{
	static constexpr std::uint16_t COLOR_ID_PROPERTY = 3;

	bool init(EffectGameObject* p0, CCArray* p1, ColorAction* p2)
	{
		if (!ColorSelectPopup::init(p0, p1, p2)) return false;

		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		// Edit Object for non-triggers
		if (!this->m_colorAction)
		{
			std::vector<CCNode*> nodes;
			nodes.reserve(5);

			// :broken_heart:
			nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
				this->m_mainLayer,
				[](CCNode* c) {
					auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
					return castedNode && std::string_view(castedNode->getString()) == "Color ID";
				}
			));
			nodes.emplace_back(this->m_colorInput);
			nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
				this->m_mainLayer,
				[inputNodePos = nodes[1]->getPosition()](CCNode* c) {
					auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
					return castedNode && (castedNode->getPosition() == inputNodePos);
				}
			));
			nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
				this->m_buttonMenu,
				[
						inputPosY = this->m_buttonMenu->convertToNodeSpace(
							this->convertToWorldSpace(nodes[1]->getPosition())
						).y
				](CCNode* c) {
					return c->getTag() == 2 && c->getPositionY() == inputPosY;
				}
			));
			nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
				this->m_buttonMenu,
				[
						inputPosY = this->m_buttonMenu->convertToNodeSpace(
							this->convertToWorldSpace(nodes[1]->getPosition())
						).y
				](CCNode* c) {
					return c->getTag() == 1 && c->getPositionY() == inputPosY;
				}
			));

			auto inputInfo = STP->commonInputSetup(
				this,
				NID::COLOR,
				COLOR_ID_PROPERTY,
				std::move(nodes),
				this->m_mainLayer,
				this->m_buttonMenu
			);
			STP->m_fields->m_id_inputs[COLOR_ID_PROPERTY] = std::move(inputInfo);

			return true;
		}

		short colorID = this->m_colorAction->m_colorID;

		if (colorID <= 0 || colorID >= 1000)
			return true;

		auto label = CCLabelBMFont::create("Color ID:", "goldFont.fnt");
		label->setPosition({ 150.f, 48.f });
		label->setScale(.625f);
		label->setID("color-id-label"_spr);
		this->m_buttonMenu->addChild(label);

		auto idInput = geode::TextInput::create(50.f, "ID");
		idInput->getBGSprite()->setContentHeight(55.f);
		idInput->setString(fmt::format("{}", colorID));
		idInput->setEnabled(false);
		idInput->setScale(.8f);
		idInput->setPosition({ 150.f, 25.f });
		idInput->setID("color-id-input"_spr);
		this->m_buttonMenu->addChild(idInput);

		auto colorNameInput = geode::TextInput::create(110.f, "Unnamed");
		colorNameInput->setFilter(ng::constants::VALID_NAMED_ID_CHARACTERS);
		colorNameInput->setEnabled(false);
		colorNameInput->setScale(.7f);
		colorNameInput->setPosition({ 150.f, .0f });
		colorNameInput->setID("color-name-input"_spr);
		this->m_buttonMenu->addChild(colorNameInput);

		auto editInputButtonSprite = CCSprite::create("pencil.png"_spr);
		editInputButtonSprite->setScale(.375f);
		auto editInputButton = CCMenuItemSpriteExtra::create(
			editInputButtonSprite,
			this,
			menu_selector(NIDColorSelectPopup::onEditIDNameButton)
		);
		editInputButton->setTag(colorID);
		editInputButton->setPosition({ 200.f, .0f });
		editInputButton->setID("edit-color-name-button"_spr);
		this->m_buttonMenu->addChild(editInputButton);

		if (auto name = NIDManager::getNameForID<NID::COLOR>(colorID))
			colorNameInput->setString(name.unwrap());

		return true;
	}

	void onUpdateCustomColor(CCObject* sender)
	{
		ColorSelectPopup::onUpdateCustomColor(sender);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->triggerArrowWasClicked(COLOR_ID_PROPERTY, sender->getTag() == 2);
	}

	void textChanged(CCTextInputNode* input)
	{
		ColorSelectPopup::textChanged(input);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->textWasChanged(input);
	}


	void onEditIDNameButton(CCObject* sender)
	{
		auto popup = EditNamedIDPopup<NID::COLOR>::create(
			sender->getTag(),
			[](short id) {},
			[this, tag = sender->getTag()] {
				auto newName = NIDManager::getNameForID<NID::COLOR>(tag).unwrapOr("");

				static_cast<geode::TextInput*>(
					this->m_buttonMenu->getChildByID("color-name-input"_spr)
				)->setString(newName);
			}
		);
		popup->getLeftArrowButton()->setVisible(false);
		popup->getRightArrowButton()->setVisible(false);
		popup->getIDInput()->setEnabled(false);
		popup->show();
	}
};

#include <Geode/modify/SetupSequenceTriggerPopup.hpp>
#include <Geode/binding/TextInputDelegate.hpp>

#include "../SetupPopups.hpp"
#include "../popups/EditNamedIDPopup.hpp"

#include <NIDManager.hpp>

#include "utils.hpp"
#include "vmthooker.hpp"
#include "touchprio_fix.hpp"

using namespace geode::prelude;

struct NIDModifiedSequenceInputDelegate : public TextInputDelegate
{
	NIDModifiedSequenceInputDelegate() = default;
	NIDModifiedSequenceInputDelegate(SetupTriggerPopup* popup)
		: m_popup(popup)
	{}

	void textChanged(CCTextInputNode* input) override
	{
		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(m_popup);

		m_popup->textChanged(input);

		if (!STP->m_fields->m_id_inputs.contains(input->getTag())) return;

		auto& idInputInfo = STP->m_fields->m_id_inputs.at(input->getTag());

		if (auto parsedNum = numFromString<short>(idInputInfo.idInput->getString()); parsedNum.isOk())
		{
			idInputInfo.namedIDInput->setString(
				NIDManager::getNameForID<NID::GROUP>(parsedNum.unwrap()).unwrapOr("")
			);

			// garbage.
			auto delegate = input->m_delegate;
			input->setDelegate(nullptr);
			STP->updateValue(-1, parsedNum.unwrap());
			input->setDelegate(delegate);
		}
	}

	void textInputOpened(CCTextInputNode* input) override
	{ m_popup->textInputOpened(input); }

	void textInputClosed(CCTextInputNode* input) override
	{ m_popup->textInputClosed(input); }

	void textInputShouldOffset(CCTextInputNode* input, float p1) override
	{ m_popup->textInputShouldOffset(input, p1); }

	void textInputReturn(CCTextInputNode* input) override
	{ m_popup->textInputReturn(input); }

	bool allowTextInput(CCTextInputNode* input) override
	{ return m_popup->allowTextInput(input); }

	void enterPressed(CCTextInputNode* input) override
	{ m_popup->enterPressed(input); }

private:
	SetupTriggerPopup* m_popup;
};


struct NIDSetupSequenceTriggerPopup : geode::Modify<NIDSetupSequenceTriggerPopup, SetupSequenceTriggerPopup>
{
	static constexpr std::uint16_t GROUP_ID_PROPERTY = 51;

	struct Fields
	{
		CCMenu* m_groups_list_menu;
		NIDModifiedSequenceInputDelegate m_modified_delegate;
	};

	bool init(SequenceTriggerGameObject* p0)
	{
		if (!SetupSequenceTriggerPopup::init(p0)) return false;

		ng::utils::cocos::fixTouchPriority(this);

		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		std::vector<CCNode*> groupIDNodes;
		groupIDNodes.reserve(5);

		// :broken_heart:
		groupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "GroupID:";
			}
		));
		groupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[labelXPos = groupIDNodes[0]->getPositionX()](CCNode* c) {
				auto castedNode = typeinfo_cast<CCTextInputNode*>(c);
				return castedNode && castedNode->getPositionX() == labelXPos;
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
				auto castedNode = typeinfo_cast<CCMenuItemSpriteExtra*>(c);
				return castedNode && castedNode->getPositionY() == inputPosY && castedNode->getTag() != -2;
		}));
		groupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					otherButton = groupIDNodes[3],
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(groupIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				auto castedNode = typeinfo_cast<CCMenuItemSpriteExtra*>(c);
				return castedNode && castedNode->getPositionY() == inputPosY && castedNode->getTag() != -2 && castedNode != otherButton;
		}));

		groupIDNodes[1]->setTag(GROUP_ID_PROPERTY);


		auto inputInfo = STP->commonInputSetup(
			this,
			NID::GROUP,
			GROUP_ID_PROPERTY,
			std::move(groupIDNodes),
			this->m_mainLayer,
			this->m_buttonMenu
		);
		m_fields->m_modified_delegate = NIDModifiedSequenceInputDelegate{ this };
		inputInfo.idInput->setDelegate(&m_fields->m_modified_delegate);
		inputInfo.namedIDInput.setEditInputCallback([&](const std::string& str) {
			NIDSetupSequenceTriggerPopup::onEditInput(this, str);
		});
		inputInfo.editInputButton->m_pfnSelector = menu_selector(NIDSetupSequenceTriggerPopup::onEditIDNameButton);
		STP->m_fields->m_id_inputs.insert({ GROUP_ID_PROPERTY, std::move(inputInfo) });


		auto groupsListMenu = CCMenu::create();
		groupsListMenu->setPosition({ .0f, 140.f });
		groupsListMenu->setContentSize({ 350.f, 75.f });
		groupsListMenu->setLayout(
			RowLayout::create()
				->setGap(10.f)
				->setCrossAxisReverse(false)
				->setGrowCrossAxis(true)
				->setCrossAxisOverflow(false)
				->setAxisAlignment(AxisAlignment::Start)
				->setCrossAxisAlignment(AxisAlignment::End)
		);
		groupsListMenu->setID("groups-list-menu"_spr);
		this->m_buttonMenu->addChild(groupsListMenu);
		m_fields->m_groups_list_menu = groupsListMenu;

		this->updateGroupIDButtons();

		return true;
	}

	void updateGroupIDButtons()
	{
		const auto& chanceObjects = static_cast<SequenceTriggerGameObject*>(
			this->m_gameObject
		)->m_chanceObjects;
		int selectedTag = -1;
		if (this->m_selectedButton)
			selectedTag = this->m_selectedButton->getTag();

		SetupSequenceTriggerPopup::updateGroupIDButtons();

		CCMenu* groupsListMenu = m_fields->m_groups_list_menu;
		if (!groupsListMenu) return;

		groupsListMenu->removeAllChildren();

		// thank you robert for adding the buttons to m_buttonMenu instead of an actual CCMenu
		for (auto button : CCArrayExt<CCMenuItemSpriteExtra*>(this->m_groupButtons))
			button->setVisible(false);

		for (std::size_t idx = 0; const auto& chanceObj : chanceObjects)
		{
			// idk maybe paginate in the future
			if (idx > 32) break;

			bool isSelectedButton = idx == selectedTag;
			short groupID = chanceObj.m_groupID;

			auto newButtonSprite = ButtonSprite::create(
				fmt::format("{}-{}", groupID, chanceObj.m_chance).c_str(),
				40, 0, .5f, true, "bigFont.fnt",
				isSelectedButton
					? "GJ_button_03.png"
					: "GJ_button_04.png",
				30.f
			);

			if (auto name = NIDManager::getNameForID<NID::GROUP>(groupID); name.isOk())
			{
				CCPoint buttonLabelPos = newButtonSprite->m_label->getPosition();

				newButtonSprite->m_label->setPositionY(buttonLabelPos.y + 4.f);

				auto nameLabel = CCLabelBMFont::create(name.unwrap().c_str(), "bigFont.fnt");
				nameLabel->setScale(.6f);
				nameLabel->limitLabelWidth(35.f, .5f, .1f);
				nameLabel->setZOrder(1);
				nameLabel->setPosition({ buttonLabelPos.x, buttonLabelPos.y - 6.f });
				nameLabel->setID("name-label"_spr);
				newButtonSprite->addChild(nameLabel);
			}

			auto newButton = CCMenuItemSpriteExtra::create(
				newButtonSprite,
				nullptr,
				this,
				menu_selector(NIDSetupSequenceTriggerPopup::onSelectNewButton)
			);
			newButton->setTag(idx);
			groupsListMenu->addChild(newButton);

			idx++;
		}

		groupsListMenu->updateLayout();
	}


	void onSelectNewButton(CCObject* sender)
	{
		if (this->m_selectedButton)
		{
			auto button = static_cast<ButtonSprite*>(static_cast<CCMenuItemSpriteExtra*>(
				m_fields->m_groups_list_menu->getChildByTag(this->m_selectedButton->getTag())
			)->getNormalImage());

			button->updateBGImage("GJ_button_04.png");

			if (auto nameLabel = button->getChildByID("name-label"_spr))
			{
				button->m_label->setPositionY(button->m_label->getPositionY() + 4.f);
				nameLabel->setPositionY(button->m_label->getPositionY() - 10.f);
			}
		}

		if (this->m_selectedButton && this->m_selectedButton->getTag() == sender->getTag())
			this->m_selectedButton = nullptr;
		else
		{
			this->m_selectedButton = static_cast<CCMenuItemSpriteExtra*>(
				m_fields->m_groups_list_menu->getChildByTag(sender->getTag())
			);
			auto button = static_cast<ButtonSprite*>(static_cast<CCMenuItemSpriteExtra*>(sender)->getNormalImage());

			button->updateBGImage("GJ_button_03.png");

			if (auto nameLabel = button->getChildByID("name-label"_spr))
			{
				button->m_label->setPositionY(button->m_label->getPositionY() + 4.f);
				nameLabel->setPositionY(button->m_label->getPositionY() - 10.f);
			}
		}
	}

	void onEditIDNameButton(CCObject*)
	{
		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		auto& idInputInfo = STP->m_fields->m_id_inputs.at(GROUP_ID_PROPERTY);

		EditNamedIDPopup<NID::GROUP>::create(
			geode::utils::numFromString<short>(idInputInfo.idInput->getString()).unwrapOr(0),
			[&](short id) {
				idInputInfo.idInput->setString(fmt::format("{}", id));
			},
			[&] {
				this->textChanged(idInputInfo.idInput);
				this->updateGroupIDButtons();
			}
		)->show();
	}

	static void onEditInput(NIDSetupSequenceTriggerPopup* self, const std::string& str)
	{
		auto& idInputInfo = reinterpret_cast<NIDSetupTriggerPopup*>(self)->m_fields->m_id_inputs.at(GROUP_ID_PROPERTY);

		if (auto idRes = NIDManager::getIDForName<NID::GROUP>(str); idRes.isOk())
		{
			auto id = idRes.unwrap();

			idInputInfo.idInput->setString(fmt::format("{}", id));
			// garbage.
			self->updateValue(-1, id);
		}
	}

	static void onExitHook(auto& original, SetupSequenceTriggerPopup* self)
	{
		CCTouchDispatcher::get()->removeDelegate(self);

		original(self);

		ng::utils::VMTHooker<&cocos2d::CCLayer::onExit, SetupSequenceTriggerPopup>::get(self)
			.toggleHook(NIDSetupSequenceTriggerPopup::onExitHook, false);
			geode::Ref<CCNode>();
	}
};

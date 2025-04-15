#include <Geode/modify/SetupRandAdvTriggerPopup.hpp>
#include <Geode/binding/TextInputDelegate.hpp>

#include "../SetupPopups.hpp"
#include "../popups/EditNamedIDPopup.hpp"

#include <NIDManager.hpp>

#include "utils.hpp"

using namespace geode::prelude;

struct NIDModifiedRandAdvInputDelegate : public TextInputDelegate
{
	NIDModifiedRandAdvInputDelegate() = default;
	NIDModifiedRandAdvInputDelegate(SetupTriggerPopup* popup)
		: m_popup(popup)
	{}

	void textChanged(CCTextInputNode* input) override
	{
		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(m_popup);

		m_popup->textChanged(input);

		if (!STP->m_fields->m_id_inputs.contains(input->getTag())) return;

		auto& idInputInfo = STP->m_fields->m_id_inputs.at(input->getTag());

		if (auto parsedNum = numFromString<short>(idInputInfo.idInput->getString()); parsedNum.isOk())
			idInputInfo.namedIDInput->setString(
				NIDManager::getNameForID(idInputInfo.idType, parsedNum.unwrap()).unwrapOr("")
			);
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


struct NIDSetupRandAdvTriggerPopup : geode::Modify<NIDSetupRandAdvTriggerPopup, SetupRandAdvTriggerPopup>
{
	static constexpr std::uint16_t GROUP_ID_PROPERTY = 1;

	struct Fields
	{
		CCMenu* m_groups_list_menu;
		NIDModifiedRandAdvInputDelegate m_modified_delegate;
	};

	bool init(RandTriggerGameObject* p0, CCArray* p1)
	{
		if (!SetupRandAdvTriggerPopup::init(p0, p1)) return false;

		// why robert.....
		ng::utils::cocos::getChildByPredicate(this->m_mainLayer, [](CCNode* c) {
			auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
			return c && c->getTag() == 1;
		})->setTag(-9);

		ng::utils::cocos::fixTouchPriority(this);

		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		std::vector<CCNode*> groupIDNodes;
		groupIDNodes.reserve(3);

		// :broken_heart:
		groupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "GroupID";
			}
		));
		groupIDNodes.emplace_back(this->m_mainLayer->getChildByTag(GROUP_ID_PROPERTY));
		groupIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[inputNodePos = groupIDNodes[1]->getPosition()](CCNode* c) {
				auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
				return castedNode && (castedNode->getPosition() == inputNodePos);
			}
		));


		auto inputInfo = STP->commonInputSetup(
			this,
			NID::GROUP,
			GROUP_ID_PROPERTY,
			std::move(groupIDNodes),
			this->m_mainLayer,
			this->m_buttonMenu
		);
		STP->m_fields->m_id_inputs[GROUP_ID_PROPERTY] = std::move(inputInfo);


		auto groupsListMenu = CCMenu::create();
		groupsListMenu->setPosition({ .0f, 115.f });
		groupsListMenu->setContentSize({ 330.f, 75.f });
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

		auto editInputButton = static_cast<CCMenuItemSpriteExtra*>(
			this->m_buttonMenu->getChildByID("edit-group-name-button-1"_spr)
		);
		editInputButton->setPositionX(editInputButton->getPositionX() - 115.f);
		editInputButton->m_pfnSelector = menu_selector(NIDSetupRandAdvTriggerPopup::onEditIDNameButton);

		m_fields->m_modified_delegate = NIDModifiedRandAdvInputDelegate(this);
		this->m_groupIDInput->setDelegate(&m_fields->m_modified_delegate);

		this->updateGroupIDButtons();

		return true;
	}

	void updateGroupIDButtons()
	{
		SetupRandAdvTriggerPopup::updateGroupIDButtons();

		CCMenu* groupsListMenu = m_fields->m_groups_list_menu;
		if (!groupsListMenu) return;

		groupsListMenu->removeAllChildren();

		// thank you robert for adding the buttons to m_buttonMenu instead of an actual CCMenu
		for (auto button : CCArrayExt<CCMenuItemSpriteExtra*>(this->m_groupButtons))
			button->setVisible(false);

		for (std::size_t idx; auto button : CCArrayExt<CCMenuItemSpriteExtra*>(this->m_groupButtons))
		{
			auto buttonSprite = button->getChildByType<ButtonSprite*>(0);
			std::uint16_t groupID;
			int count;
			int chance;

			{
				auto text = std::string_view{
					buttonSprite->getChildByType<CCLabelBMFont*>(0)->getString()
				};

				std::size_t firstDelimPos = text.find(" - ");
				std::size_t secondDelimPos = text.find('\n');

				groupID = geode::utils::numFromString<std::uint16_t>(text.substr(0, firstDelimPos)).unwrap();
				count = geode::utils::numFromString<int>(text.substr(firstDelimPos + 3, secondDelimPos)).unwrap();
				chance = geode::utils::numFromString<int>(text.substr(secondDelimPos + 1, text.find('%'))).unwrap();
			}

			auto newButtonSprite = ButtonSprite::create(
				fmt::format("{} - {}\n{}%", groupID, count, chance).c_str(),
				40, 0, .5f, true, "bigFont.fnt",
				"GJ_button_04.png", 40.f
			);

			if (auto name = NIDManager::getNameForID<NID::GROUP>(groupID); name.isOk())
			{
				CCPoint buttonLabelPos = newButtonSprite->m_label->getPosition();

				newButtonSprite->m_label->setPositionY(buttonLabelPos.y + 4.f);
				newButtonSprite->m_label->setScale(newButtonSprite->m_label->getScale() - .02f);

				auto nameLabel = CCLabelBMFont::create(name.unwrap().c_str(), "bigFont.fnt");
				nameLabel->setScale(.6f);
				nameLabel->limitLabelWidth(35.f, .5f, .1f);
				nameLabel->setZOrder(1);
				nameLabel->setPosition({ buttonLabelPos.x, buttonLabelPos.y - 10.f });
				newButtonSprite->addChild(nameLabel);
			}

			auto newButton = CCMenuItemSpriteExtra::create(
				newButtonSprite,
				this,
				button->m_pfnSelector
			);
			newButton->setTag(button->getTag());
			groupsListMenu->addChild(newButton);

			idx++;
		}

		groupsListMenu->updateLayout();
	}


	void onEditIDNameButton(CCObject*)
	{
		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		auto& idInputInfo = STP->m_fields->m_id_inputs.at(GROUP_ID_PROPERTY);

		ShowEditNamedIDPopup(
			idInputInfo.idType,
			geode::utils::numFromString<short>(idInputInfo.idInput->getString()).unwrapOr(0),
			[&](short id) {
				idInputInfo.idInput->setString(fmt::format("{}", id));
			},
			[&] {
				this->m_fields->m_modified_delegate.textChanged(idInputInfo.idInput);
				this->updateGroupIDButtons();
			}
		);
	}
};

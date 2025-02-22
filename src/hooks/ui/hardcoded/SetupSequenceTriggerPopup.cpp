#include <Geode/modify/SetupSequenceTriggerPopup.hpp>

#include "../SetupPopups.hpp"
#include "../popups/EditNamedIDPopup.hpp"

#include <NIDManager.hpp>

#include "utils.hpp"

using namespace geode::prelude;

struct NIDSetupSequenceTriggerPopup : geode::Modify<NIDSetupSequenceTriggerPopup, SetupSequenceTriggerPopup>
{
	static constexpr std::uint16_t GROUP_ID_PROPERTY = 51;

	struct Fields
	{
		CCMenu* m_groups_list_menu;
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


		auto inputInfo = STP->commonSetup(
			NID::GROUP,
			GROUP_ID_PROPERTY,
			std::move(groupIDNodes),
			this->m_mainLayer,
			this->m_buttonMenu
		);
		STP->m_fields->m_id_inputs[GROUP_ID_PROPERTY] = std::move(inputInfo);


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

		static_cast<CCMenuItemSpriteExtra*>(
			this->m_buttonMenu->getChildByID("edit-group-name-button-51"_spr)
		)->m_pfnSelector = menu_selector(NIDSetupSequenceTriggerPopup::onEditIDNameButton);

		this->updateGroupIDButtons();

		return true;
	}

	void textChanged(CCTextInputNode* input)
	{
		SetupSequenceTriggerPopup::textChanged(input);

		reinterpret_cast<NIDSetupTriggerPopup*>(this)->textWasChanged(input);
	}

	void updateGroupIDButtons()
	{
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

		for (std::size_t idx; auto button : CCArrayExt<CCMenuItemSpriteExtra*>(this->m_groupButtons))
		{
			bool isSelectedButton = button->getTag() == selectedTag;
			auto buttonSprite = button->getChildByType<ButtonSprite*>(0);
			std::uint16_t groupID;
			int count;

			{
				auto text = std::string_view{ buttonSprite->getChildByType<CCLabelBMFont*>(0)->getString() };

				groupID = geode::utils::numFromString<std::uint16_t>(text.substr(0, text.find('-'))).unwrap();
				count = geode::utils::numFromString<int>(text.substr(text.find('-') + 1)).unwrap();
			}

			auto newButtonSprite = ButtonSprite::create(
				fmt::format("{}-{}", groupID, count).c_str(),
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
				newButtonSprite->addChild(nameLabel);
			}

			auto newButton = CCMenuItemSpriteExtra::create(
				newButtonSprite,
				this,
				menu_selector(SetupSequenceTriggerPopup::onSelect)
			);
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
				this->textChanged(idInputInfo.idInput);
				this->updateGroupIDButtons();
			}
		);
	}
};

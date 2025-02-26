#include <Geode/modify/SetGroupIDLayer.hpp>

#include "../LevelEditorLayerData.hpp"
#include "../popups/EditNamedIDPopup.hpp"

#include <NIDManager.hpp>

#include "constants.hpp"

struct NIDSetGroupIDLayer : geode::Modify<NIDSetGroupIDLayer, SetGroupIDLayer>
{
	// used to make the named group button
	static constexpr float PADDING = 10.f;
	static constexpr float TOTAL_PADDING = PADDING * 2.f;

	struct Fields
	{
		geode::TextInput* m_input;
		CCMenuItemSpriteExtra* m_input_button;
	};

	bool init(GameObject* obj, CCArray* objs)
	{
		if (!SetGroupIDLayer::init(obj, objs)) return false;

		auto winSize = CCDirector::get()->getWinSize();

		auto groupIDMenu = m_mainLayer->getChildByID("add-group-id-menu");
		groupIDMenu->setScale(.9f);
		groupIDMenu->setPositionY(225.f);

		m_mainLayer->getChildByID("next-free-menu")->setPositionY(210.f);
		m_mainLayer->getChildByID("add-group-id-buttons-menu")->setPositionY(210.f);

		static_cast<CCMenuItemSpriteExtra*>(
			groupIDMenu->getChildByID("add-group-id-input")
		)->setPositionY(23.f);
		static_cast<CCMenuItemSpriteExtra*>(
			groupIDMenu->getChildByID("add-group-id-prev-button")
		)->setPositionY(23.f);
		static_cast<CCMenuItemSpriteExtra*>(
			groupIDMenu->getChildByID("add-group-id-next-button")
		)->setPositionY(23.f);

		auto groupNameInput = geode::TextInput::create(100.f, "Unnamed");
		groupNameInput->setContentHeight(20.f);
		groupNameInput->setFilter(ng::constants::VALID_NAMED_ID_CHARACTERS);
		groupNameInput->setCallback([&](const std::string& str) { NIDSetGroupIDLayer::onEditInput(this, std::move(str)); });
		groupNameInput->setPosition({ 60.f, -13.f });
		groupNameInput->setID("group-name-input"_spr);
		groupNameInput->getBGSprite()->setContentSize({ 220.f, 55.f });
		groupIDMenu->addChild(groupNameInput);
		m_fields->m_input = groupNameInput;

		if (auto name = NIDManager::getNameForID<NID::GROUP>(this->m_groupIDValue); name.isOk())
			groupNameInput->setString(name.unwrap());

		auto editInputButtonSprite = CCSprite::create("pencil.png"_spr);
		editInputButtonSprite->setScale(.4f);
		auto editInputButton = CCMenuItemSpriteExtra::create(
			editInputButtonSprite,
			this,
			menu_selector(NIDSetGroupIDLayer::onEditGroupNameButton)
		);
		editInputButton->setID("group-name-input-button"_spr);
		editInputButton->setPosition({ 127.f, -7.f });
		groupIDMenu->addChild(editInputButton);
		m_fields->m_input_button = editInputButton;


		auto groupsListMenu = m_mainLayer->getChildByID("groups-list-menu");
		groupsListMenu->setPosition({ winSize.width / 2.f, winSize.height / 2.f - 17.f });
		groupsListMenu->setAnchorPoint({ .5f, .5f });
		groupsListMenu->ignoreAnchorPointForPosition(false);
		groupsListMenu->setContentSize({ 340.f, 50.f });
		groupsListMenu->setLayout(
			RowLayout::create()
				->setGap(10.f)
				->setCrossAxisReverse(false)
				->setGrowCrossAxis(true)
				->setCrossAxisOverflow(false)
				->setAxisAlignment(AxisAlignment::Start)
				->setCrossAxisAlignment(AxisAlignment::End)
		);

		return true;
	}

	void onArrow(int tag, int increment)
	{
		SetGroupIDLayer::onArrow(tag, increment);

		m_fields->m_input->getInputNode()->onClickTrackNode(false);

		m_fields->m_input->setString(
			NIDManager::getNameForID<NID::GROUP>(this->m_groupIDValue).unwrapOr("")
		);
	}

	void textChanged(CCTextInputNode* input)
	{
		SetGroupIDLayer::textChanged(input);

		if (!m_fields->m_input) return;

		if (input == m_groupIDInput)
			m_fields->m_input->setString(
				NIDManager::getNameForID<NID::GROUP>(this->m_groupIDValue).unwrapOr("")
			);
	}

	void updateGroupIDButtons()
	{
		SetGroupIDLayer::updateGroupIDButtons();

		const auto LEL = LevelEditorLayer::get();
		bool isOnlyObject = static_cast<bool>(this->m_targetObject);
		std::set<short> uniqueIDs;
		std::set<short> groupParentIDs;

		if (!isOnlyObject)
		{
			auto firstObject = static_cast<GameObject*>(this->m_targetObjects->firstObject());

			if (firstObject->m_groups)
				uniqueIDs.insert(firstObject->m_groups->begin(), firstObject->m_groups->end());

			for (bool first = true; auto obj : CCArrayExt<GameObject*>(this->m_targetObjects))
			{
				if (obj->m_groups)
				{
					for (short id : *obj->m_groups)
						if (obj == LEL->m_parentGroupsDict->objectForKey(id))
							groupParentIDs.insert(id);
				}

				if (first)
				{
					first = false;
					continue;
				}

				if (obj->m_groups)
				{
					for (short id : *obj->m_groups)
						if (uniqueIDs.contains(id))
							uniqueIDs.erase(id);
						else
							uniqueIDs.insert(id);
				}
			}
		}
		else if (LEL->m_parentGroupsDict && this->m_targetObject->m_groups)
		{
			for (short id : *this->m_targetObject->m_groups)
				if (this->m_targetObject == LEL->m_parentGroupsDict->objectForKey(id))
					groupParentIDs.insert(id);
		}


		// looping over and modifying a CCArray is a no no
		auto groupIDArrayCopy = this->m_groupIDObjects->shallowCopy();
		for (std::size_t idx = 0; auto button : CCArrayExt<CCMenuItemSpriteExtra*>(groupIDArrayCopy))
		{
			// stupid layer items ordering bug fix
			button->setZOrder(button->getTag());

			if (auto name = NIDManager::getNameForID<NID::GROUP>(button->getTag()); name.isOk())
			{
				bool isGroupParent = groupParentIDs.contains(button->getTag());

				auto nameLabel = CCLabelBMFont::create(
					name.unwrap().c_str(),
					"bigFont.fnt"
				);
				nameLabel->setScale(.5f);
				nameLabel->limitLabelWidth(70.f, .5f, .1f);

				auto idLabel = CCLabelBMFont::create(
					fmt::format("{}", button->getTag()).c_str(),
					"goldFont.fnt"
				);
				idLabel->setScale(.5f);
				idLabel->setZOrder(-1);

				std::string buttonSpriteSpr = "";

				if (isOnlyObject)
					buttonSpriteSpr = isGroupParent
						? "GJ_button_03.png"
						: "GJ_button_04.png";
				else
					buttonSpriteSpr = isGroupParent
						? "GJ_button_03.png"
						: uniqueIDs.contains(button->getTag())
							? "GJ_button_05.png"
							: "GJ_button_04.png";

				auto buttonSprite = ButtonSprite::create(
					"",
					nameLabel->getScaledContentWidth() + idLabel->getScaledContentWidth() + PADDING,
					0, .5f, true, "goldFont.fnt",
					buttonSpriteSpr.c_str(),
					20.f
				);
				buttonSprite->m_label->removeFromParent();

				auto labelsContainer = CCNode::create();
				labelsContainer->addChild(idLabel);
				labelsContainer->addChild(nameLabel);
				labelsContainer->setContentSize(buttonSprite->getContentSize());
				labelsContainer->setAnchorPoint({ .5f, .5f });
				labelsContainer->setPosition(buttonSprite->m_BGSprite->getPosition());
				buttonSprite->addChild(labelsContainer);

				// labels positioning (layouts are brokey in small content sizes :broken_heart:)
				{
					const float idLabelWidth = idLabel->getScaledContentWidth();
					const float nameLabelWidth = nameLabel->getScaledContentWidth();
					const auto parentSize = labelsContainer->getContentSize();

					// add 2.f to Y pos because the exact middle makes the label overlap with the shadow of the background
					idLabel->setPosition({
						PADDING + idLabelWidth / 2.f,
						parentSize.height / 2.f + 2.f
					});
					nameLabel->setPosition({
						parentSize.width - PADDING - nameLabelWidth / 2.f,
						parentSize.height / 2.f + 2.f
					});
				}

				// ID label background
				{
					const auto idLabelPos = idLabel->getPosition();
					const auto idLabelSize = idLabel->getScaledContentSize();

					auto background = CCSprite::create("square02b_001.png");
					background->setScaleX(idLabelSize.width / background->getScaledContentWidth() + .05f);
					background->setScaleY(idLabelSize.height / background->getScaledContentHeight() - .02f);
					background->setColor({ 0, 0, 0 });
					background->setOpacity(100);
					background->setPosition({  idLabelPos.x, idLabelPos.y - 1.5f });
					labelsContainer->addChild(background, -2);
				}

				auto newButton = CCMenuItemSpriteExtra::create(
					buttonSprite,
					this,
					menu_selector(SetGroupIDLayer::onRemoveFromGroup)
				);
				button->getParent()->addChild(newButton);
				newButton->setTag(button->getTag());
				newButton->setOrderOfArrival(button->getOrderOfArrival());
				newButton->setZOrder(button->getTag());

				button->removeFromParent();
				this->m_groupIDObjects->replaceObjectAtIndex(idx, newButton);
			}

			idx++;
		}

		// this method is called within init, and groups-list-menu doesn't exist by then
		if (auto groupsListMenu = m_mainLayer->getChildByID("groups-list-menu"))
			groupsListMenu->updateLayout();
	}


	void onEditGroupNameButton(CCObject*)
	{
		EditNamedIDPopup<NID::GROUP>::create(this->m_groupIDValue,
			[&](short id) {
				this->m_groupIDInput->setString(fmt::format("{}", id));
			},
			[&] {
				this->textChanged(this->m_groupIDInput);
				this->updateGroupIDButtons();
		})->show();
	}

	static void onEditInput(NIDSetGroupIDLayer* self, const std::string& str)
	{
		if (auto name = NIDManager::getIDForName<NID::GROUP>(str); name.isOk())
			self->m_groupIDInput->setString(fmt::format("{}", name.unwrap()));
	}
};

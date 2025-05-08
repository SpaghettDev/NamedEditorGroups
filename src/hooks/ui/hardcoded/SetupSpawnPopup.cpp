#include <Geode/modify/SetupSpawnPopup.hpp>
#include <Geode/binding/TextInputDelegate.hpp>

#include "../SetupPopups.hpp"
#include "../popups/EditNamedIDPopup.hpp"

#include <NIDManager.hpp>

#include <ranges>

#include "utils.hpp"
#include "operators.hpp" // IWYU pragma: keep
#ifdef __APPLE__
#include "joined_spans.hpp"
#endif

using namespace geode::prelude;

struct NIDSetupSpawnPopup : geode::Modify<NIDSetupSpawnPopup, SetupSpawnPopup>
{
	static constexpr std::uint16_t ORIGINAL_ID_PROPERTY = -1;
	static constexpr std::uint16_t NEW_ID_PROPERTY = -2;

	struct Fields
	{
		CCMenu* m_remaps_list_menu;
	};

	bool init(EffectGameObject* p0, CCArray* p1)
	{
		if (!SetupSpawnPopup::init(p0, p1)) return false;

		ng::utils::cocos::fixTouchPriority(this);

		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		std::vector<CCNode*> originalIDNodes;
		originalIDNodes.reserve(5);

		std::vector<CCNode*> newIDNodes;
		newIDNodes.reserve(5);


		originalIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "OriginalID:";
			}
		));
		originalIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[labelXPos = originalIDNodes[0]->getPositionX()](CCNode* c) {
				auto castedNode = typeinfo_cast<CCTextInputNode*>(c);
				return castedNode && castedNode->getPositionX() == labelXPos;
			}
		));
		originalIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[inputNodePos = originalIDNodes[1]->getPosition()](CCNode* c) {
				auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
				return castedNode && (castedNode->getPosition() == inputNodePos);
			}
		));
		originalIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(originalIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				auto castedNode = typeinfo_cast<CCMenuItemSpriteExtra*>(c);
				return castedNode && castedNode->getPositionY() == inputPosY && castedNode->getTag() != -2;
		}));
		originalIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					otherButton = originalIDNodes[3],
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(originalIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				auto castedNode = typeinfo_cast<CCMenuItemSpriteExtra*>(c);
				return castedNode && castedNode->getPositionY() == inputPosY && castedNode->getTag() != -2 && castedNode != otherButton;
		}));


		newIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "NewID:";
			}
		));
		newIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[labelXPos = newIDNodes[0]->getPositionX()](CCNode* c) {
				auto castedNode = typeinfo_cast<CCTextInputNode*>(c);
				return castedNode && castedNode->getPositionX() == labelXPos;
			}
		));
		newIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[inputNodePos = newIDNodes[1]->getPosition()](CCNode* c) {
				auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
				return castedNode && (castedNode->getPosition() == inputNodePos);
			}
		));
		newIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(newIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				auto castedNode = typeinfo_cast<CCMenuItemSpriteExtra*>(c);
				return castedNode && castedNode->getPositionY() == inputPosY && castedNode->getTag() != -1;
		}));
		newIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					otherButton = newIDNodes[3],
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(newIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				auto castedNode = typeinfo_cast<CCMenuItemSpriteExtra*>(c);
				return castedNode && castedNode->getPositionY() == inputPosY && castedNode->getTag() != -1 && castedNode != otherButton;
		}));


		auto originalIDInputInfo = STP->commonInputSetup(
			this,
			NID::GROUP,
			ORIGINAL_ID_PROPERTY,
			std::move(originalIDNodes),
			this->m_mainLayer,
			this->m_buttonMenu,
			[&](std::vector<CCNode*>&& newNodes) {
				for (auto& node : newNodes)
				{
					node->setVisible(false);
					static_cast<CCArray*>(this->m_pageContainers->objectAtIndex(1))->addObject(node);
					static_cast<CCArray*>(this->m_groupContainers->objectAtIndex(0))->addObject(node);
				}
			}
		);
		originalIDInputInfo.namedIDInput->setCallback([&](const std::string& str) {
			NIDSetupSpawnPopup::onEditInput(this, -1, str);
		});
		originalIDInputInfo.editInputButton->m_pfnSelector = menu_selector(NIDSetupSpawnPopup::onEditIDNameButton);
		STP->m_fields->m_id_inputs[ORIGINAL_ID_PROPERTY] = std::move(originalIDInputInfo);

		auto newIDInputInfo = STP->commonInputSetup(
			this,
			NID::GROUP,
			NEW_ID_PROPERTY,
			std::move(newIDNodes),
			this->m_mainLayer,
			this->m_buttonMenu,
			[&](std::vector<CCNode*>&& newNodes) {
				for (auto& node : newNodes)
				{
					node->setVisible(false);
					static_cast<CCArray*>(this->m_pageContainers->objectAtIndex(1))->addObject(node);
					static_cast<CCArray*>(this->m_groupContainers->objectAtIndex(0))->addObject(node);
				}
			}
		);
		newIDInputInfo.namedIDInput->setCallback([&](const std::string& str) {
			NIDSetupSpawnPopup::onEditInput(this, -2, str);
		});
		newIDInputInfo.editInputButton->m_pfnSelector = menu_selector(NIDSetupSpawnPopup::onEditIDNameButton);
		STP->m_fields->m_id_inputs[NEW_ID_PROPERTY] = std::move(newIDInputInfo);


		auto remapsListMenu = CCMenu::create();
		remapsListMenu->setPosition({ .0f, 117.f });
		remapsListMenu->setContentSize({ 350.f, 75.f });
		remapsListMenu->setLayout(
			RowLayout::create()
				->setGap(10.f)
				->setCrossAxisReverse(false)
				->setGrowCrossAxis(true)
				->setCrossAxisOverflow(false)
				->setAxisAlignment(AxisAlignment::Start)
				->setCrossAxisAlignment(AxisAlignment::End)
		);
		remapsListMenu->setID("remaps-list-menu"_spr);
		this->m_buttonMenu->addChild(remapsListMenu);
		m_fields->m_remaps_list_menu = remapsListMenu;

		this->updateRemapButtons(420.f);

		STP->m_fields->m_page_change_cb = [this](int page) {
			if (page != 1) return;

			for (auto button : CCArrayExt<CCMenuItemSpriteExtra*>(this->m_remapButtons))
				button->setVisible(false);
		};

		return true;
	}

	void updateRemapButtons(float dt)
	{
		std::vector<std::span<ChanceObject>> remapVecViews;
		if (this->m_gameObjects)
		{
			remapVecViews.reserve(this->m_gameObjects->count());

			for (auto obj : CCArrayExt<SpawnTriggerGameObject*>(this->m_gameObjects))
				remapVecViews.emplace_back(&obj->m_remapObjects[0], obj->m_remapObjects.size());
		}
		else
		{
			auto& remapObjects = static_cast<SpawnTriggerGameObject*>(this->m_gameObject)->m_remapObjects;

			remapVecViews.emplace_back(&remapObjects[0], remapObjects.size());
		}

#ifdef __APPLE__
		// workaround appleclang having jack shit
		auto remapObjects = ng::utils::ranges::join_spans(remapVecViews);
#else
		auto remapObjects = std::views::join(remapVecViews);
#endif
		auto commonRemapObjects = ng::utils::multiSetIntersection<ChanceObject*>(std::move(remapVecViews));
		std::set<ChanceObject*, ng::utils::impl::PtrRefComparator<ChanceObject>> uniqueRemapObjects;
		std::ranges::transform(
			remapObjects,
			std::inserter(uniqueRemapObjects, uniqueRemapObjects.end()),
			[](auto& obj) { return &obj; }
		);

		SetupSpawnPopup::updateRemapButtons(dt);

		CCMenu* remapsListMenu = m_fields->m_remaps_list_menu;
		if (!remapsListMenu) return;

		remapsListMenu->removeAllChildren();

		for (auto button : CCArrayExt<CCMenuItemSpriteExtra*>(this->m_remapButtons))
			button->setVisible(false);

		for (std::size_t idx = 0; const auto& remapObj : uniqueRemapObjects)
		{
			// maybe paginate this as well
			if (idx > 20) break;

			auto newButtonSprite = ButtonSprite::create(
				fmt::format("{}\n{}", remapObj->m_groupID, remapObj->m_chance).c_str(),
				40, 0, .35f, true, "bigFont.fnt",
				(this->m_remapOriginalID == remapObj->m_groupID && this->m_remapNewID == remapObj->m_chance)
					? "GJ_button_03.png"
					: !commonRemapObjects.contains(remapObj)
						? "GJ_button_05.png"
						: "GJ_button_04.png",
				30.f
			);

			auto oldIDNameRes = NIDManager::getNameForID<NID::GROUP>(remapObj->m_groupID);
			auto newIDNameRes = NIDManager::getNameForID<NID::GROUP>(remapObj->m_chance);

			if (oldIDNameRes.isOk() && newIDNameRes.isOk())
			{
				auto& oldIDName = oldIDNameRes.unwrap();
				auto& newIDName = newIDNameRes.unwrap();

				newButtonSprite->m_label->setString(
					fmt::format(
						"{} ({})",
						oldIDName, remapObj->m_groupID
					).c_str()
				);

				auto buttonLabelPos = newButtonSprite->m_label->getPosition();
				auto newNameLabel = CCLabelBMFont::create(
					fmt::format("{} ({})", newIDName, remapObj->m_chance).c_str(),
					"bigFont.fnt"
				);
				newNameLabel->limitLabelWidth(48.f, .35f, .1f);
				newButtonSprite->m_label->setPositionY(buttonLabelPos.y + 4.f);
				newNameLabel->setPosition({ buttonLabelPos.x, buttonLabelPos.y - 6.f });
				newButtonSprite->addChild(newNameLabel);
			}

			auto newButton = CCMenuItemSpriteExtra::create(
				newButtonSprite,
				nullptr,
				this,
				menu_selector(NIDSetupSpawnPopup::onSelectNewRemap)
			);
			newButton->setTag(idx);
			remapsListMenu->addChild(newButton);

			// hide the buttons if its the first call after init
			if (dt == 420.f) newButton->setVisible(false);
			static_cast<CCArray*>(this->m_pageContainers->objectAtIndex(1))->addObject(newButton);
			static_cast<CCArray*>(this->m_groupContainers->objectAtIndex(0))->addObject(newButton);

			idx++;
		}

		remapsListMenu->updateLayout();
	}


	void onSelectNewRemap(CCObject* sender)
	{
		int tag = sender->getTag();

		// m_remapGroups is actually just a flat vector of pairs of ints
		// i think robert forgot std::pair exists
		int remapOrigID = this->m_remapGroups.at(tag << 1);
		int remapNewID = this->m_remapGroups.at((tag << 1) + 1);

		if (this->m_remapOriginalID == remapOrigID && this->m_remapNewID == remapNewID)
		{
			this->m_remapOriginalID = 0;
			this->m_remapNewID = 0;
		}
		else
		{
			this->m_remapOriginalID = remapOrigID;
			this->m_remapNewID = remapNewID;

			this->updateValue(-1, remapOrigID);
			this->updateValue(-2, remapNewID);
		}

		CCScheduler::get()->scheduleSelector(
			schedule_selector(SetupSpawnPopup::updateRemapButtons),
			this, .0f, 0, .0f, false
		);
	}

	void onEditIDNameButton(CCObject* sender)
	{
		auto STP = reinterpret_cast<NIDSetupTriggerPopup*>(this);

		auto& idInputInfo = STP->m_fields->m_id_inputs.at(sender->getTag());

		EditNamedIDPopup<NID::GROUP>::create(
			geode::utils::numFromString<short>(idInputInfo.idInput->getString()).unwrapOr(0),
			[&](short id) {
				idInputInfo.idInput->setString(fmt::format("{}", id));
			},
			[&] {
				this->textChanged(idInputInfo.idInput);
				this->updateRemapButtons(.0f);
			}
		)->show();
	}

	static void onEditInput(NIDSetupSpawnPopup* self, std::uint16_t property, const std::string& str)
	{
		auto& idInputInfo = reinterpret_cast<NIDSetupTriggerPopup*>(self)->m_fields->m_id_inputs.at(property);

		if (auto idRes = NIDManager::getIDForName<NID::GROUP>(str); idRes.isOk())
		{
			auto id = idRes.unwrap();

			idInputInfo.idInput->setString(fmt::format("{}", id));
			// garbage.
			self->updateValue(property, id);
		}
	}
};

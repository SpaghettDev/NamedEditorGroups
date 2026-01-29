#include "NamedIDsPopup.hpp"

#include <vector>
#include <algorithm>

#include "AddNamedIDPopup.hpp"
#include "SelectIDFilterPopup.hpp"
#include "SharePopup.hpp"
#include "cells/NamedIDCell.hpp"

#include <NIDManager.hpp>

#include "utils.hpp"
#include "globals.hpp"
#include "fuzzy_match.hpp"

using namespace geode::prelude;

NamedIDsPopup* NamedIDsPopup::create(bool readOnly)
{
	auto ret = new NamedIDsPopup();

	if (ret && ret->initAnchored(300.f, 260.f, readOnly))
		ret->autorelease();
	else
	{
		delete ret;
		ret = nullptr;
	}

	return ret;
}

bool NamedIDsPopup::setup(bool readOnly)
{
	this->setID("NamedIDsPopup");
	this->setTitle("Edit Named IDs");

	m_read_only = readOnly;

	if (m_read_only)
	{
		this->m_title->setPositionY(this->m_title->getPositionY() + 7.f);

		auto readOnlyLabel = CCLabelBMFont::create("(Read-Only)", "goldFont.fnt");
		readOnlyLabel->setScale(.5f);
		readOnlyLabel->setPosition(this->m_title->getPosition() - CCPoint{ .0f, 15.f });
		this->m_mainLayer->addChild(readOnlyLabel, 2);
	}

	if (ng::globals::g_isEditorIDAPILoaded)
	{
		auto settingsButtonSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
		settingsButtonSpr->setScale(.5f);
		auto settingsButton = CCMenuItemSpriteExtra::create(
			settingsButtonSpr,
			this,
			menu_selector(NamedIDsPopup::onSettingsButton)
		);
		settingsButton->setEnabled(!readOnly);
		settingsButton->setColor(readOnly ? ccColor3B{ 125, 125, 125 } : ccColor3B{ 255, 255, 255 });
		this->m_buttonMenu->addChildAtPosition(settingsButton, Anchor::TopLeft, { 65.f, -20.f });
	}

	auto filterButtonSpr = ButtonSprite::create(
		CCSprite::createWithSpriteFrameName("GJ_filterIcon_001.png"),
		30, 0, 30.5f, 1.f, false, "GJ_button_04.png", true
	);
	filterButtonSpr->setScale(.6f);
	auto filterButton = CCMenuItemSpriteExtra::create(
		filterButtonSpr,
		this,
		menu_selector(NamedIDsPopup::onFilterButton)
	);
	this->m_buttonMenu->addChildAtPosition(filterButton, Anchor::TopRight, { -20.f, -20.f });

	auto addButtonSpr = CCSprite::create("plus.png"_spr);
	addButtonSpr->setScale(.4f);
	auto addButton = CCMenuItemSpriteExtra::create(
		addButtonSpr,
		this,
		menu_selector(NamedIDsPopup::onAddButton)
	);
	addButton->setEnabled(!readOnly);
	addButton->setColor(readOnly ? ccColor3B{ 125, 125, 125 } : ccColor3B{ 255, 255, 255 });
	this->m_buttonMenu->addChildAtPosition(addButton, Anchor::TopRight, { -49.f, -20.f });

	auto shareButtonSpr = CCSprite::createWithSpriteFrameName("GJ_shareBtn_001.png");
	shareButtonSpr->setScale(.48f);
	auto shareButton = CCMenuItemSpriteExtra::create(
		shareButtonSpr,
		this,
		menu_selector(NamedIDsPopup::onShareButton)
	);
	shareButton->setEnabled(!readOnly);
	shareButton->setColor(readOnly ? ccColor3B{ 125, 125, 125 } : ccColor3B{ 255, 255, 255 });
	this->m_buttonMenu->addChildAtPosition(shareButton, Anchor::BottomLeft, { 3.f, 3.f });

	m_layer_bg = CCLayerColor::create({ 0, 0, 0, 75 });
	m_layer_bg->setContentSize(SCROLL_LAYER_SIZE);
	m_layer_bg->ignoreAnchorPointForPosition(false);
	this->m_mainLayer->addChildAtPosition(m_layer_bg, Anchor::Center, { .0f, -15.f });

	m_search_container = CCMenu::create();
	m_search_container->setContentSize({ SCROLL_LAYER_SIZE.width, 30.f });
	m_search_input = geode::TextInput::create(
		(SCROLL_LAYER_SIZE.width - 15.f) / .7f - 40.f,
		fmt::format("Search {}s...", ng::utils::getNamedIDIndentifier(m_ids_type))
	);
	m_search_input->setTextAlign(TextInputAlign::Left);
	m_search_input->setScale(.7f);
	m_search_input->setCallback([this](const std::string& str) {
		this->updateState();
		this->m_list->moveToTop();
	});
	m_search_container->addChildAtPosition(m_search_input, Anchor::Left, { 7.5f, .0f }, { .0f, .5f });

	auto searchClearSpr = CCSprite::createWithSpriteFrameName("GJ_longBtn07_001.png");
	searchClearSpr->setScale(.7f);
	auto searchClearButton = CCMenuItemSpriteExtra::create(
		searchClearSpr,
		this,
		menu_selector(NamedIDsPopup::onClearSearchButton)
	);
	searchClearButton->setPosition({ 40.f, 40.f });
	m_search_container->addChildAtPosition(searchClearButton, Anchor::Right, { -18.f, .0f });

	m_layer_bg->addChildAtPosition(m_search_container, Anchor::Top, { .0f, -3.f }, { .5f, 1.f });

	m_list = ScrollLayer::create(SCROLL_LAYER_SIZE - CCPoint{ .0f, m_search_container->getContentHeight() });
	m_list->setTouchEnabled(true);

	updateList(NID::GROUP);

	m_list->m_contentLayer->setLayout(
		ColumnLayout::create()
			->setAxisReverse(true)
			->setAutoGrowAxis(m_list->getContentHeight())
			->setCrossAxisOverflow(false)
			->setAxisAlignment(AxisAlignment::End)
			->setGap(.0f)
	);
	m_list->moveToTop();

	const int buttonPrio = m_list->getTouchPriority() - 1;
	m_buttonMenu->setTouchPriority(buttonPrio);
	m_search_container->setTouchPriority(buttonPrio);

	m_layer_bg->addChildAtPosition(m_list, Anchor::BottomLeft);

	auto listBorders = geode::ListBorders::create();
	listBorders->setContentSize(SCROLL_LAYER_SIZE + CCSize{ 5.f, .0f });
	this->m_mainLayer->addChildAtPosition(listBorders, Anchor::Center, { .0f, -13.f });

	auto scrollBar = geode::Scrollbar::create(m_list);
	this->m_mainLayer->addChildAtPosition(scrollBar, Anchor::Center, { m_layer_bg->getContentWidth() / 2.f + 10.f, -13.f });

	return true;
}

void NamedIDsPopup::onClose(CCObject* sender)
{
	if (ng::globals::g_isEditorIDAPILoaded)
		ng::utils::editor::refreshObjectLabels();

	Popup::onClose(sender);
}

void NamedIDsPopup::onClearSearchButton(CCObject*)
{
	m_search_input->setString("");
	updateState();
}

void NamedIDsPopup::onFilterButton(CCObject*)
{
	SelectIDFilterPopup::create(m_ids_type, [&](NID nid) {
		this->m_ids_type = nid;

		this->m_search_input->setPlaceholder(fmt::format("Search {}s...", ng::utils::getNamedIDIndentifier(nid)));
		this->m_search_input->setString("");

		this->updateState();
		this->m_list->moveToTop();
	})->show();
}

void NamedIDsPopup::onAddButton(CCObject*)
{
	AddNamedIDPopup::create(m_ids_type, [&](const std::string&, short) {
		this->updateState();
	})->show();
}

void NamedIDsPopup::onSettingsButton(CCObject*)
{
	m_adv_mode = !m_adv_mode;

	for (auto& item : CCArrayExt<NamedIDCell<false>*>(m_list->m_contentLayer->getChildren()))
		item->showAdvancedOptions(m_adv_mode);
}

void NamedIDsPopup::onShareButton(CCObject*)
{
	SharePopup::create([&](bool success) {
		if (!success) return;

		this->updateState();
		ng::utils::editor::save();
	}, [](bool) {})->show();
}

void NamedIDsPopup::updateList(NID nid)
{
	m_ids_type = nid;

	{
		const std::unordered_map<std::string, short>& namedIDs = NIDManager::getNamedIDs(m_ids_type);

		std::vector<std::pair<std::string, short>> elements{ namedIDs.begin(), namedIDs.end() };
		std::sort(elements.begin(), elements.end(), [](auto& a, auto& b) { return a.second < b.second; });

		bool bg = false;

		for (auto& [name, id] : elements)
		{
			auto item = NamedIDCell<false>::create(m_ids_type, id, std::move(name), m_adv_mode, m_read_only, SCROLL_LAYER_SIZE.width);
			item->setDefaultBGColor({ 0, 0, 0, static_cast<GLubyte>(bg ? 60 : 20) });
			m_list->m_contentLayer->addChild(item);

			bg = !bg;
		}
	}
}

void NamedIDsPopup::updateState()
{
	auto query = m_search_input->getString();

	m_list->m_contentLayer->removeAllChildren();
	updateList(m_ids_type);

	if (!query.empty() && !NIDManager::getNamedIDs(m_ids_type).empty())
	{
		bool bg = false;

		for (auto& item : CCArrayExt<NamedIDCell<false>*>(m_list->m_contentLayer->getChildren()->shallowCopy()))
		{
			item->setDefaultBGColor({ 0, 0, 0, static_cast<GLubyte>(bg ? 60 : 20) });

			if (!ng::utils::fuzzy_match::matchesQuery(query, { item->getName(), item->getID() }))
				item->removeFromParent();
			else
				bg = !bg;
		}
	}

	m_list->m_contentLayer->updateLayout();
}

#include "NamedIDsPopup.hpp"

#include <vector>
#include <algorithm>

#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include <Geode/external/fts/fts_fuzzy_match.h>

#include "AddNamedIDPopup.hpp"
#include "SelectIDFilterPopup.hpp"
#include "items/NamedIDItem.hpp"

#include <NIDManager.hpp>

#include "utils.hpp"

using namespace geode::prelude;

namespace
{
	bool weightedFuzzyMatch(const std::string& str, const std::string& kw, double weight, double& out)
	{
		int score;

		if (fts::fuzzy_match(kw.c_str(), str.c_str(), score))
		{
			out = std::max(out, score * weight);
			return true;
		}

		return false;
	}

	static bool matchesSearch(const std::string& query, NamedIDItem* item)
	{
		bool doesMatch = false;
		double weighted = 0;

		doesMatch |= weightedFuzzyMatch(item->getName(), query, .5, weighted);
		doesMatch |= weightedFuzzyMatch(fmt::format("{}", item->getID()), query, 1.0, weighted);

		if (weighted < 50 + 5 * query.size())
			doesMatch = false;

		return doesMatch;
	}
}

NamedIDsPopup* NamedIDsPopup::create()
{
	auto ret = new NamedIDsPopup();

	if (ret && ret->initAnchored(300.f, 260.f))
		ret->autorelease();
	else
	{
		delete ret;
		ret = nullptr;
	}

	return ret;
}

bool NamedIDsPopup::setup()
{
	this->setID("NamedIDsPopup");
	this->setTitle("Edit Named IDs");

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
	this->m_buttonMenu->addChildAtPosition(addButton, Anchor::TopRight, { -49.f, -20.f });

	auto refreshButtonSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
	refreshButtonSpr->setScale(.8f);
	auto refreshButton = CCMenuItemSpriteExtra::create(
		refreshButtonSpr,
		this,
		menu_selector(NamedIDsPopup::onRefreshButton)
	);
	this->m_buttonMenu->addChildAtPosition(refreshButton, Anchor::BottomLeft, { 3.f, 3.f });

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
		m_list->moveToTop();
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

void NamedIDsPopup::onClearSearchButton(CCObject*)
{
	m_search_input->setString("");
	updateState();
}

void NamedIDsPopup::onFilterButton(CCObject*)
{
	SelectIDFilterPopup::create(m_ids_type, [&](NID nid) {
		m_ids_type = nid;

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

void NamedIDsPopup::onRefreshButton(CCObject*)
{
	ng::utils::editor::refreshObjectLabels();
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
			auto item = NamedIDItem::create(m_ids_type, id, std::move(name), SCROLL_LAYER_SIZE.width);
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

		for (auto& item : CCArrayExt<NamedIDItem*>(m_list->m_contentLayer->getChildren()->shallowCopy()))
		{
			item->setDefaultBGColor({ 0, 0, 0, static_cast<GLubyte>(bg ? 60 : 20) });

			if (!matchesSearch(query, item))
				item->removeFromParent();
			else
				bg = !bg;
		}
	}

	m_list->m_contentLayer->updateLayout();
}

#include "SelectIDFilterPopup.hpp"

#include <NIDManager.hpp>

#include "utils.hpp"

using namespace geode::prelude;

SelectIDFilterPopup* SelectIDFilterPopup::create(NID currentNid, std::function<void(NID)>&& onChangedCallback)
{
	auto ret = new SelectIDFilterPopup();

	if (ret && ret->initAnchored(220.f, 130.f, currentNid, std::move(onChangedCallback)))
		ret->autorelease();
	else
	{
		delete ret;
		ret = nullptr;
	}

	return ret;
}

bool SelectIDFilterPopup::setup(NID currentNid, std::function<void(NID)>&& onChangedCallback)
{
	m_on_changed_callback = std::move(onChangedCallback);

	this->setID("SelectIDFilterPopup");
	this->setTitle("Filter Named IDs");

	m_toggles_menu = CCMenu::create();
	m_toggles_menu->setContentSize({ 200.f, 75.f });
	m_toggles_menu->setLayout(
		ColumnLayout::create()
			->setAutoScale(true)
			->setAxisReverse(true)
			->setGrowCrossAxis(true)
			->setCrossAxisReverse(true)
			->setCrossAxisOverflow(false)
			->setAxisAlignment(AxisAlignment::Even)
			->setCrossAxisAlignment(AxisAlignment::Between)
			->setGap(10.f)
	);
	m_toggles_menu->setPosition({ 110.f, 55.f });
	this->m_buttonMenu->addChild(m_toggles_menu);

	for (NID nid = NID::GROUP; nid <= NID::_INTERNAL_TLAST; nid = static_cast<NID>(static_cast<int>(nid) + 1))
	{
		auto toggleMenu = CCMenu::create();
		toggleMenu->setContentSize({ 150.f, 50.f });
		toggleMenu->setLayout(
			RowLayout::create()
				->setGap(10.f)
				->setAutoScale(false)
		);

		auto buttonSpriteOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
		auto buttonSpriteOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
		buttonSpriteOn->setScale(.8f);
		buttonSpriteOff->setScale(.8f);

		auto nidButton = CCMenuItemToggler::create(
			buttonSpriteOff,
			buttonSpriteOn,
			this,
			menu_selector(SelectIDFilterPopup::onToggleButton)
		);
		nidButton->setSizeMult(1.2f);
		nidButton->setTag(static_cast<int>(nid));
		nidButton->setID("toggle-button");
		nidButton->toggle(nid == currentNid);
		toggleMenu->addChild(nidButton);

		auto nidLabel = CCLabelBMFont::create(
			fmt::format("{} ({})", ng::utils::getNamedIDIndentifier(nid), NIDManager::getNamedIDs(nid).size()).c_str(),
			"bigFont.fnt"
		);
		nidLabel->limitLabelWidth(100.f, 1.f, .1f);
		toggleMenu->addChild(nidLabel);

		toggleMenu->updateLayout();
		m_toggles_menu->addChild(toggleMenu);

		nidLabel->setPositionY(nidLabel->getPositionY() + 2.f);
	}

	m_toggles_menu->updateLayout();

	return true;
}

void SelectIDFilterPopup::onToggleButton(CCObject* sender)
{
	int tag = sender->getTag();

	for (auto& toggleMenu : CCArrayExt<CCMenu*>(m_toggles_menu->getChildren()))
		static_cast<CCMenuItemToggler*>(toggleMenu->getChildByID("toggle-button"))->toggle(false);

	m_on_changed_callback(static_cast<NID>(tag));
}

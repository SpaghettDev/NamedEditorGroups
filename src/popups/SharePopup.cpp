#include "SharePopup.hpp"

#include <algorithm>

#include <Geode/utils/general.hpp>

#include <NIDManager.hpp>
#include <NIDEnum.hpp>

#include "base64.hpp"
#include "utils.hpp"

using namespace geode::prelude;

SharePopup* SharePopup::create()
{
	auto ret = new SharePopup();

	if (ret && ret->initAnchored(155.f, 145.f))
		ret->autorelease();
	else
	{
		delete ret;
		ret = nullptr;
	}

	return ret;
}

bool SharePopup::setup()
{
	this->setID("SharePopup");

	auto importBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { .0f, .0f, 80.f, 80.f });
	importBg->setColor({ 191, 114, 62 });
	importBg->setPosition({ 78.f, 105.f });
	importBg->setContentSize({ 140.f, 65.f });
	this->m_buttonMenu->addChild(importBg, -2);

	auto importBorder = geode::ListBorders::create();
	importBorder->setPosition({ 78.f, 105.f });
	importBorder->setContentSize({ 140.f, 60.f });
	this->m_buttonMenu->addChild(importBorder, -1);

	auto importLabel = CCLabelBMFont::create("Import", "goldFont.fnt");
	importLabel->setPosition({ 57.f, 117.f });
	importLabel->setScale(.9f);
	this->m_buttonMenu->addChild(importLabel);

	auto importButtonSpr = CCSprite::create("import.png"_spr);
	importButtonSpr->setScale(.85f);
	auto importButton = CCMenuItemSpriteExtra::create(
		importButtonSpr,
		this,
		menu_selector(SharePopup::onImportButton)
	);
	importButton->setPosition({ 122.f, 114.f });
	this->m_buttonMenu->addChild(importButton);

	auto importTooltipLabel = TextArea::create(
		"Importing will try to parse what's\ncurrently in your clipboard",
		"chatFont.fnt", GEODE_DESKTOP(.6f) GEODE_MOBILE(.5f), 125.f, { .45f, .5f }, 10.f, true
	);
	importTooltipLabel->colorAllCharactersTo({ 0, 0, 0, });
	importTooltipLabel->setOpacity(125);
	importTooltipLabel->setPosition({ 73.f, 95.f });
	this->m_buttonMenu->addChild(importTooltipLabel);


	auto exportBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { .0f, .0f, 80.f, 80.f });
	exportBg->setColor({ 191, 114, 62 });
	exportBg->setPosition({ 78.f, 40.f });
	exportBg->setContentSize({ 140.f, 65.f });
	this->m_buttonMenu->addChild(exportBg, -2);

	auto exportBorder = geode::ListBorders::create();
	exportBorder->setPosition({ 78.f, 40.f });
	exportBorder->setContentSize({ 140.f, 60.f });
	this->m_buttonMenu->addChild(exportBorder, -1);

	auto exportLabel = CCLabelBMFont::create("Export", "goldFont.fnt");
	exportLabel->setScale(.9f);
	exportLabel->setPosition({ 57.f, 53.f });
	this->m_buttonMenu->addChild(exportLabel);

	auto exportButtonSpr = CCSprite::create("export.png"_spr);
	exportButtonSpr->setScale(.85f);
	auto exportButton = CCMenuItemSpriteExtra::create(
		exportButtonSpr,
		this,
		menu_selector(SharePopup::onExportButton)
	);
	exportButton->setPosition({ 122.f, 49.f });
	this->m_buttonMenu->addChild(exportButton);

	auto exportTooltipLabel = TextArea::create(
		"Exporting will copy the save\nobject into your clipboard",
		"chatFont.fnt", .6f, 135.f, { .45f, .5f }, 10.f, true
	);
	exportTooltipLabel->colorAllCharactersTo({ 0, 0, 0 });
	exportTooltipLabel->setOpacity(125);
	exportTooltipLabel->setPosition({ 73.f, 30.f });
	this->m_buttonMenu->addChild(exportTooltipLabel);

	return true;
}

void SharePopup::onImportButton(CCObject*)
{
	std::string clipboard = geode::utils::clipboard::read();

	if (!clipboard.empty() && ng::base64::isBase64URLLike(clipboard))
	{
		if (auto b64decoded = ng::base64::base64URLDecode(clipboard); b64decoded.isErr())
		{
			auto errorPopup = FLAlertLayer::create(
				nullptr,
				"Error importing NamedIDs",
				fmt::format(
					"Unable to decode base64 in NamedIDs: <cr>{}</c>",
					b64decoded.unwrapErr()
				),
				"OK",
				nullptr,
				350.f
			);
			errorPopup->m_scene = this;
			return errorPopup->show();
		}
		else
			clipboard = b64decoded.unwrap();
	}

	// quick sanity checks
	if (clipboard.empty() || std::ranges::count(clipboard, '|') < static_cast<std::size_t>(NID::_INTERNAL_TLAST) - 1)
		return ng::utils::cocos::createNotificationToast(this, "Invalid save data string!", .5f, 30.f);

	if (auto res = NIDManager::importNamedIDs(clipboard); res.isErr())
	{
		auto errorPopup = FLAlertLayer::create(
			nullptr,
			"Error importing NamedIDs",
			fmt::format("<cr>{}</c>", res.unwrapErr()),
			"OK",
			nullptr,
			350.f
		);
		errorPopup->m_scene = this;
		return errorPopup->show();
	}

	ng::utils::cocos::createNotificationToast(this, "Successfully imported NamedIDs", 1.f, 60.f);
}

void SharePopup::onExportButton(CCObject*)
{
	geode::utils::clipboard::write(NIDManager::dumpNamedIDs());

	ng::utils::cocos::createNotificationToast(this, "Successfully copied to clipboard", .5f, 60.f);
}

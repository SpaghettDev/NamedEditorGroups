#include "NamedIDCell.hpp"

#include <Geode/binding/LevelEditorLayer.hpp>
#include <Geode/binding/GJColorSetupLayer.hpp>

#include <NIDManager.hpp>
#include <NIDExtrasManager.hpp>

#include "../EditDescriptionPopup.hpp"

#include "utils.hpp"
#include "globals.hpp"

using namespace geode::prelude;

NamedIDCell* NamedIDCell::create(NID idType, short id, std::string&& name, bool advMode, float width)
{
	auto ret = new NamedIDCell();

	if (ret && ret->init(idType, id, std::move(name), advMode, width))
		ret->autorelease();
	else
	{
		delete ret;
		ret = nullptr;
	}

	return ret;
}

bool NamedIDCell::init(NID idType, short id, std::string&& name, bool advMode, float width)
{
	if (!CCNode::init()) return false;

	m_id_type = idType;
	m_id = id;
	m_name = std::move(name);

	m_bg = CCLayerColor::create({ 0, 0, 0, 0 });
	m_bg->setContentSize({ width, 0 });
	m_bg->ignoreAnchorPointForPosition(false);
	m_bg->setAnchorPoint({ .5f, .5f });
	this->addChildAtPosition(m_bg, Anchor::Center);

	m_name_menu = CCMenu::create();
	m_name_menu->setContentWidth(width / 2.f + 25.f);

	m_name_label = ButtonSprite::create(
		fmt::format("{}", id).c_str(),
		30.f, 0, .5f, true,
		"goldFont.fnt", "GJ_button_04.png", 20.f
	);
	m_name_label->setCascadeColorEnabled(true);
	m_name_menu->addChild(m_name_label);

	if (m_id_type == NID::COLOR)
	{
		auto LEL = LevelEditorLayer::get();

		m_color_sprite = ColorChannelSprite::create();
		m_color_sprite->setScale(.6f);

		colorSelectClosed(nullptr);

		m_color_button = CCMenuItemSpriteExtra::create(
			m_color_sprite,
			this,
			menu_selector(NamedIDCell::onSelectColor)
		);
		m_name_menu->addChild(m_color_button);
	}

	if (ng::globals::g_isEditorIDAPILoaded)
	{
		auto previewButtonSpr = CCSprite::create("preview.png"_spr);
		auto previewToggledButtonSpr = CCSprite::create("previewToggled.png"_spr);
		previewButtonSpr->setScale(.3f);
		previewToggledButtonSpr->setScale(.3f);
		previewToggledButtonSpr->setVisible(false);
		previewToggledButtonSpr->setID("toggled-sprite");
		m_preview_button = CCMenuItemSpriteExtra::create(
			previewButtonSpr,
			this,
			menu_selector(NamedIDCell::onPreviewButton)
		);
		m_preview_button->addChild(previewToggledButtonSpr);
		previewToggledButtonSpr->setPosition(previewButtonSpr->getPosition());
		m_preview_button->setVisible(false);
		m_name_menu->addChild(m_preview_button);

		if (!NIDExtrasManager::getIsNamedIDPreviewed(m_id_type, m_id).unwrapOr(true))
		{
			m_preview_toggled = false;
			previewToggledButtonSpr->setVisible(true);
			previewButtonSpr->setVisible(false);

			m_name_label->setColor({ 125, 125, 125});
		}

		auto descriptionButtonSpr = CCSprite::create("description.png"_spr);
		descriptionButtonSpr->setScale(.65f);
		m_description_button = CCMenuItemSpriteExtra::create(
			descriptionButtonSpr,
			this,
			menu_selector(NamedIDCell::onDescriptionButton)
		);
		m_description_button->setVisible(
			NIDExtrasManager::getNamedIDDescription(m_id_type, m_id).isOkAnd(
				[](const std::string& s) { return !s.empty(); }
			)
		);
		m_name_menu->addChild(m_description_button);
	}

	m_name_menu->setLayout(
		RowLayout::create()
			->setAxisAlignment(AxisAlignment::Start)
			->setAutoScale(false)
			->setGap(4.f)
	);
	m_name_menu->getLayout()->ignoreInvisibleChildren(true);
	this->addChildAtPosition(m_name_menu, Anchor::Left, { 10.f, .0 }, { .0f, .5f });

	m_button_menu = CCMenu::create();
	m_button_menu->setContentSize({ width / 2.f + 10.f, 30.f });
	m_button_menu->setLayout(
		RowLayout::create()
			->setGap(10.f)
			->setAutoScale(true)
			->setAxisReverse(true)
	);
	this->addChildAtPosition(m_button_menu, Anchor::Right, { -6.f, .0f }, { 1.f, .5f });

	this->setAnchorPoint({ .5f, .0f });
	this->setContentSize({ width, 30.f });
	m_bg->setContentSize({ width, 30.f });
	this->updateLayout();


	auto editInputButtonSpr = CCSprite::create("edit.png"_spr);
	auto saveButtonSpr = ButtonSprite::create(
		CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"),
		30, 0, 30.5f, 1.f, false, "baseBtn.png"_spr, true
	);
	saveButtonSpr->setID("toggled-sprite");
	saveButtonSpr->setVisible(false);
	m_edit_button = CCMenuItemSpriteExtra::create(
		editInputButtonSpr,
		this,
		menu_selector(NamedIDCell::onEditButton)
	);
	saveButtonSpr->setPosition(m_edit_button->getNormalImage()->getPosition());
	m_edit_button->addChild(saveButtonSpr);
	saveButtonSpr->m_subSprite->setPositionY(saveButtonSpr->getPositionY() + 1.f);
	saveButtonSpr->m_subSprite->setScale(.75f);
	m_button_menu->addChild(m_edit_button);

	m_name_input = geode::TextInput::create(width / 2.f, "");
	m_name_input->setString(m_name.data());
	m_name_input->setEnabled(false);
	m_button_menu->addChild(m_name_input);

	m_cancel_button = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_longBtn07_001.png"),
		this,
		menu_selector(NamedIDCell::onClearButton)
	);
	m_cancel_button->setVisible(false);
	m_button_menu->addChild(m_cancel_button);

	m_name_menu->setContentWidth(this->getContentWidth() - m_button_menu->getContentWidth() - 25.f);
	m_name_menu->updateLayout();
	m_button_menu->updateLayout();

	this->showAdvancedOptions(advMode);

	return true;
}

void NamedIDCell::setDefaultBGColor(const ccColor4B& color)
{
	m_bg_color = color;

	m_bg->setColor(geode::cocos::to3B(m_bg_color));
	m_bg->setOpacity(m_bg_color.a);
}

void NamedIDCell::onEditButton(CCObject* sender)
{
	auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
	m_editing = !m_editing;

	if (m_editing && g_currentEditingItem && g_currentEditingItem != this)
	{
		g_currentEditingItem->m_editing = false;
		g_currentEditingItem->m_edit_button->getChildByID("toggled-sprite")->setVisible(false);
		g_currentEditingItem->m_edit_button->getNormalImage()->setVisible(true);
		g_currentEditingItem->m_cancel_button->setVisible(false);

		g_currentEditingItem->m_name_input->setString(
			NIDManager::getNameForID(m_id_type, g_currentEditingItem->m_id).unwrapOr("")
		);
		g_currentEditingItem->m_name_input->setEnabled(false);
	}

	button->getChildByID("toggled-sprite")->setVisible(m_editing);
	button->getNormalImage()->setVisible(!m_editing);
	m_cancel_button->setVisible(m_editing);

	m_name_input->setEnabled(m_editing);

	g_currentEditingItem = m_editing ? this : nullptr;

	auto nameInputStr = m_name_input->getString();

	if (m_editing) return;

	if (!nameInputStr.empty())
	{
		if (
			auto otherNamedID = NIDManager::getIDForName(m_id_type, nameInputStr);
			otherNamedID.isOkAnd([&](short id) { return id != m_id; })
		) {
			ng::utils::cocos::createNotificationToast(
				reinterpret_cast<CCLayer*>(CCScene::get()),
				fmt::format("ID {} already has this name!", otherNamedID.unwrap()),
				1.f, 45.f
			);

			m_name_input->setString(NIDManager::getNameForID(m_id_type, m_id).unwrapOr(""));

			return;
		}
		else if (auto res = NIDManager::saveNamedID(m_id_type, m_name_input->getString(), m_id); res.isErr())
		{
			ng::utils::cocos::createNotificationToast(
				reinterpret_cast<CCLayer*>(CCScene::get()),
				res.unwrapErr(),
				1.f, 45.f
			);

			m_name_input->setString(NIDManager::getNameForID(m_id_type, m_id).unwrapOr(""));

			return;
		}

		m_name = m_name_input->getString();

		m_name_input->getInputNode()->onClickTrackNode(false);
	}
	else
	{
		(void)NIDManager::removeNamedID(m_id_type, m_id);
		m_name = "";
	}

	ng::utils::editor::refreshObjectLabels();

	if (m_name_input->getString().empty())
	{
		CCNode* parent = this->getParent();
		this->removeFromParent();
		parent->updateLayout();
	}
}

void NamedIDCell::onClearButton(CCObject*)
{
	if (!m_editing) return;

	m_name_input->setString("");
}

void NamedIDCell::onSelectColor(CCObject*)
{
	auto colorAction = LevelEditorLayer::get()->m_levelSettings->m_effectManager->getColorAction(m_id);

	auto popup = ColorSelectPopup::create(colorAction);
	popup->m_delegate = this;
	popup->m_ZOrder = CCScene::get()->getHighestChildZ() + 1;
	popup->show();
}

void NamedIDCell::onPreviewButton(CCObject*)
{
	m_preview_toggled = !m_preview_toggled;

	m_preview_button->getNormalImage()->setVisible(m_preview_toggled);
	m_preview_button->getChildByID("toggled-sprite")->setVisible(!m_preview_toggled);

	(void)NIDExtrasManager::setNamedIDIsPreviewed(
		m_id_type, m_id, m_preview_toggled
	);

	m_name_label->setColor(m_preview_toggled ? ccColor3B{ 255, 255, 255 } : ccColor3B{ 125, 125, 125 });
}

void NamedIDCell::onDescriptionButton(CCObject*)
{
	EditDescriptionPopup::create(m_id_type, m_id)->show();
}

void NamedIDCell::showAdvancedOptions(bool state)
{
	m_preview_button->setVisible(state);
	m_description_button->setVisible(
		state || NIDExtrasManager::getNamedIDDescription(m_id_type, m_id).isOkAnd(
			[](const std::string& s) { return !s.empty(); }
		)
	);

	if (m_color_button)
		m_color_button->setVisible(!state);

	m_name_menu->updateLayout();
}

void NamedIDCell::colorSelectClosed(CCNode*)
{
	auto effectManager = LevelEditorLayer::get()->m_levelSettings->m_effectManager;

	if (!effectManager->colorExists(m_id))
		return m_color_sprite->updateValues(nullptr);

	auto colAction = effectManager->getColorAction(m_id);
	m_color_sprite->updateValues(colAction);

	// this is safe since updateCopyLabel just modifies visual data
	if (!m_color_sprite->m_copyLabel)
		m_color_sprite->updateCopyLabel(1, false);

	if (colAction->m_copyID == 0)
	{
		switch (colAction->m_playerColor)
		{
			case 1:
			case 2:
				m_color_sprite->m_copyLabel->setString(fmt::format("P{}", colAction->m_playerColor).c_str());
				m_color_sprite->m_copyLabel->setVisible(true);
				break;

			default:
				m_color_sprite->m_copyLabel->setVisible(false);
				break;
		}
	}
}

NamedIDCell::~NamedIDCell()
{
	g_currentEditingItem = nullptr;
}

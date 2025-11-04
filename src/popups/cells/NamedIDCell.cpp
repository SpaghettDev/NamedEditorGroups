#include "NamedIDCell.hpp"

#include <Geode/binding/LevelEditorLayer.hpp>
#include <Geode/binding/GJColorSetupLayer.hpp>

#include <NIDManager.hpp>
#include <NIDExtrasManager.hpp>

#include "../EditDescriptionPopup.hpp"

#include "utils.hpp"
#include "globals.hpp"
#include "constants.hpp"
#include "fuzzy_match.hpp"

using namespace geode::prelude;

NamedIDCell<false>* NamedIDCell<false>::create(
	NID idType, short id, std::string&& name, bool advMode, bool readOnly, float width
) {
	auto ret = new NamedIDCell<false>();

	if (ret && ret->init(idType, id, std::move(name), advMode, readOnly, width))
		ret->autorelease();
	else
	{
		delete ret;
		ret = nullptr;
	}

	return ret;
}

bool NamedIDCell<false>::init(
	NID idType, short id, std::string&& name, bool advMode, bool readOnly, float width
) {
	if (!CCNode::init()) return false;

	m_id_type = idType;
	m_id = id;
	m_name = std::move(name);

	m_bg = CCLayerColor::create({ 0, 0, 0, 0 });
	m_bg->setContentSize({ width, 30.f });
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
		if (auto LEL = LevelEditorLayer::get())
		{
			m_color_sprite = ColorChannelSprite::create();
			m_color_sprite->setScale(.6f);

			// fake a call to colorSelectClosed to set visual data
			colorSelectClosed(nullptr);

			m_color_button = CCMenuItemSpriteExtra::create(
				m_color_sprite,
				this,
				menu_selector(NamedIDCell<false>::onSelectColor)
			);
			m_color_button->setEnabled(!readOnly);
			m_name_menu->addChild(m_color_button);
		}
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
			menu_selector(NamedIDCell<false>::onPreviewButton)
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
			menu_selector(NamedIDCell<false>::onDescriptionButton)
		);
		m_description_button->setVisible(
			NIDExtrasManager::getNamedIDDescription(m_id_type, m_id).isOkAnd(
				[](const std::string& s) { return !s.empty(); }
			) && !readOnly
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
	this->addChildAtPosition(m_name_menu, Anchor::Left, { 10.f, .0f }, { .0f, .5f });

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
		menu_selector(NamedIDCell<false>::onEditButton)
	);
	m_edit_button->setEnabled(!readOnly);
	m_edit_button->setColor(readOnly ? ccColor3B{ 125, 125, 125 } : ccColor3B{ 255, 255, 255 });
	saveButtonSpr->setPosition(m_edit_button->getNormalImage()->getPosition());
	m_edit_button->addChild(saveButtonSpr);
	saveButtonSpr->m_subSprite->setPositionY(saveButtonSpr->getPositionY() + 1.f);
	saveButtonSpr->m_subSprite->setScale(.75f);
	m_button_menu->addChild(m_edit_button);

	m_name_input = geode::TextInput::create(width / 2.f, "");
	m_name_input->setFilter(ng::constants::VALID_NAMED_ID_CHARACTERS);
	m_name_input->setString(m_name.data());
	m_name_input->setEnabled(false);
	m_button_menu->addChild(m_name_input);

	m_cancel_button = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_longBtn07_001.png"),
		this,
		menu_selector(NamedIDCell<false>::onClearButton)
	);
	m_cancel_button->setVisible(false);
	m_button_menu->addChild(m_cancel_button);

	m_name_menu->setContentWidth(this->getContentWidth() - m_button_menu->getContentWidth() - 25.f);
	m_name_menu->updateLayout();
	m_button_menu->updateLayout();

	this->showAdvancedOptions(advMode);

	return true;
}

void NamedIDCell<false>::setDefaultBGColor(const ccColor4B& color)
{
	m_bg_color = color;

	m_bg->setColor(geode::cocos::to3B(m_bg_color));
	m_bg->setOpacity(m_bg_color.a);
}

void NamedIDCell<false>::onEditButton(CCObject* sender)
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

void NamedIDCell<false>::onClearButton(CCObject*)
{
	if (!m_editing) return;

	m_name_input->setString("");
}

void NamedIDCell<false>::onSelectColor(CCObject*)
{
	auto colorAction = LevelEditorLayer::get()->m_levelSettings->m_effectManager->getColorAction(m_id);

	auto popup = ColorSelectPopup::create(colorAction);
	popup->m_delegate = this;
	popup->m_ZOrder = CCScene::get()->getHighestChildZ() + 1;
	popup->show();
}

void NamedIDCell<false>::onPreviewButton(CCObject*)
{
	m_preview_toggled = !m_preview_toggled;

	m_preview_button->getNormalImage()->setVisible(m_preview_toggled);
	m_preview_button->getChildByID("toggled-sprite")->setVisible(!m_preview_toggled);

	(void)NIDExtrasManager::setNamedIDIsPreviewed(
		m_id_type, m_id, m_preview_toggled
	);

	m_name_label->setColor(m_preview_toggled ? ccColor3B{ 255, 255, 255 } : ccColor3B{ 125, 125, 125 });
}

void NamedIDCell<false>::onDescriptionButton(CCObject*)
{
	EditDescriptionPopup::create(m_id_type, m_id)->show();
}

void NamedIDCell<false>::showAdvancedOptions(bool state)
{
	if (!ng::globals::g_isEditorIDAPILoaded) return;

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

void NamedIDCell<false>::colorSelectClosed(CCNode*)
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



NamedIDCell<true>* NamedIDCell<true>::create(NID idType, short id, std::string&& name, float width)
{
	auto ret = new NamedIDCell<true>();

	if (ret && ret->init(idType, id, std::move(name), width))
		ret->autorelease();
	else
	{
		delete ret;
		ret = nullptr;
	}

	return ret;
}

bool NamedIDCell<true>::init(NID idType, short id, std::string&& name, float width)
{
	if (!CCNode::init()) return false;

	m_id_type = idType;
	m_id = id;
	m_name = std::move(name);

	this->setContentSize({ width, 30.f });

	m_bg = CCLayerColor::create({ 0, 0, 0, 0 });
	m_bg->setContentSize({ width, 30.f });
	m_bg->ignoreAnchorPointForPosition(false);
	m_bg->setAnchorPoint({ .5f, .5f });
	this->addChildAtPosition(m_bg, Anchor::Center);

	m_name_menu = CCMenu::create();
	m_name_menu->setContentWidth(width / 2.f - 10.f);
	m_name_menu->setLayout(
		RowLayout::create()
			->setAxisAlignment(AxisAlignment::Start)
			->setAutoScale(false)
			->setGap(4.f)
	);
	m_name_menu->getLayout()->ignoreInvisibleChildren(true);
	this->addChildAtPosition(m_name_menu, Anchor::Left, { 15.f, .0f }, { .0f, .5f });

	m_name_label = ButtonSprite::create(
		fmt::format("{}", id).c_str(),
		30.f, 0, .5f, true,
		"goldFont.fnt", "GJ_button_04.png", 20.f
	);
	m_name_label->setCascadeColorEnabled(true);
	m_name_menu->addChild(m_name_label);

	if (m_id_type == NID::COLOR)
	{
		if (auto LEL = LevelEditorLayer::get())
		{
			m_color_sprite = ColorChannelSprite::create();
			m_color_sprite->setScale(.6f);

			auto effectManager = LevelEditorLayer::get()->m_levelSettings->m_effectManager;

			if (!effectManager->colorExists(m_id))
				m_color_sprite->updateValues(nullptr);

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

			m_name_menu->addChild(m_color_sprite);
		}
	}

	m_button_menu = CCMenu::create();
	m_button_menu->setContentWidth(width / 2.f + 5.f);
	m_button_menu->setLayout(
		RowLayout::create()
			->setAxisAlignment(AxisAlignment::End)
			->setAutoScale(true)
			->setAxisReverse(true)
			->setGap(10.f)
	);
	this->addChildAtPosition(m_button_menu, Anchor::Right, { -6.f, .0f }, { 1.f, .5f });

	m_name_input = geode::TextInput::create(width / 2.f, "");
	m_name_input->setFilter(ng::constants::VALID_NAMED_ID_CHARACTERS);
	m_name_input->setString(m_name.data());
	m_name_input->setEnabled(false);
	m_name_input->setContentHeight(20.f);
	m_name_input->getBGSprite()->setScaleX(.425f);
	m_name_input->getBGSprite()->setScaleY(.35f);
	{
		auto textLabel = m_name_input->getInputNode()->m_textLabel;
		textLabel->limitLabelWidth(
			m_name_input->getBGSprite()->getScaledContentWidth() - 5.f,
			.5f,
			.1f
		);
		textLabel->setOpacity(255);
		textLabel->setCascadeOpacityEnabled(false);
	}
	m_name_input->updateLayout();
	m_button_menu->addChild(m_name_input);

	m_name_menu->updateLayout();
	m_button_menu->updateLayout();

	return true;
}

void NamedIDCell<true>::setDefaultBGColor(const ccColor4B& color)
{
	m_bg_color = color;

	m_bg->setColor(geode::cocos::to3B(m_bg_color));
	m_bg->setOpacity(m_bg_color.a);
}

void NamedIDCell<true>::highlightQuery(const std::string_view query)
{
	std::array<std::uint8_t, 256> indices{};
	ng::utils::fuzzy_match::matchesQuery(std::string{ query }, { m_name, m_id }, indices);
	highlightQuery(query, indices);
}

void NamedIDCell<true>::highlightQuery(const std::string_view query, const std::array<std::uint8_t, 256>& indices)
{
	auto textLabel = m_name_input->getInputNode()->m_textLabel;

	for (std::size_t sz = 0; auto idx : indices)
	{
		if (sz == m_name.size())
			break;

		if (query.empty() || query.find_first_not_of("0123465789") == std::string_view::npos)
			textLabel->getChildByType<CCFontSprite*>(sz)->setOpacity(125);
		else
			textLabel->getChildByType<CCFontSprite*>(sz)->setOpacity(
				((idx == 0 && sz == 0) || idx != 0) ? 255 : 125
			);

		sz++;
	}
}

void NamedIDCell<true>::onEnter()
{
	registerWithTouchDispatcher();

	CCNode::onEnter();
}

void NamedIDCell<true>::onEnterTransitionDidFinish()
{
	auto TD = CCTouchDispatcher::get();

	TD->setPriority(TD->findHandler(this)->getPriority() - 2, this);

	CCNode::onEnterTransitionDidFinish();
}

void NamedIDCell<true>::onExit()
{
	unregisterWithTouchDispatcher();

	CCNode::onExit();
}

bool NamedIDCell<true>::ccTouchBegan(CCTouch*, CCEvent*)
{
	return true;
}

void NamedIDCell<true>::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	if (std::abs((touch->getLocation() - touch->getStartLocation()).y) <= 2.5f) return;

	touch->setTag(TOUCH_MOVED_EVENT_TAG);
}

void NamedIDCell<true>::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	if (touch->getTag() == TOUCH_MOVED_EVENT_TAG) return;

	const CCSize& contentSize = this->getContentSize();
	const CCPoint& anchor = this->getAnchorPoint();

	CCPoint origin{ -anchor.x * contentSize.width, -anchor.y * contentSize.height };
	CCPoint dest{ origin.x + contentSize.width, origin.y + contentSize.height };

	if (
		this->isVisible() && 
		CCRect{ origin, dest }.containsPoint(this->convertTouchToNodeSpace(touch))
	) {
		m_on_select_cb(m_id_type, m_id);
	}
}

void NamedIDCell<true>::registerWithTouchDispatcher()
{
	if (!m_registered_touch)
		CCTouchDispatcher::get()->addTargetedDelegate(this, cocos2d::kCCMenuHandlerPriority, false);

	m_registered_touch = true;
}

void NamedIDCell<true>::unregisterWithTouchDispatcher()
{
	if (m_registered_touch)
		CCTouchDispatcher::get()->removeDelegate(this);

	m_registered_touch = false;
}

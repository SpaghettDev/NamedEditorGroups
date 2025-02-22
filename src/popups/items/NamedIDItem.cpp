#include "NamedIDItem.hpp"

#include <Geode/binding/LevelEditorLayer.hpp>

#include <NIDManager.hpp>

#include "utils.hpp"

using namespace geode::prelude;

NamedIDItem* NamedIDItem::create(NID idType, short id, std::string&& name, float width)
{
	auto ret = new NamedIDItem();

	if (ret && ret->init(idType, id, std::move(name), width))
		ret->autorelease();
	else
	{
		delete ret;
		ret = nullptr;
	}

	return ret;
}

bool NamedIDItem::init(NID idType, short id, std::string&& name, float width)
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
	m_name_menu->addChild(m_name_label);

	m_name_menu->setLayout(
		RowLayout::create()
			->setAxisAlignment(AxisAlignment::Start)
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


	auto editButtonSpr = CCSprite::create("edit.png"_spr);
	auto saveButtonSpr = ButtonSprite::create(
		CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"),
		30, 0, 30.5f, 1.f, false, "baseBtn.png"_spr, true
	);
	saveButtonSpr->setID("save-sprite");
	saveButtonSpr->setVisible(false);
	m_edit_button = CCMenuItemSpriteExtra::create(
		editButtonSpr,
		this,
		menu_selector(NamedIDItem::onEditButton)
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
		menu_selector(NamedIDItem::onClearButton)
	);
	m_cancel_button->setVisible(false);
	m_button_menu->addChild(m_cancel_button);

	m_name_menu->setContentWidth(this->getContentWidth() - m_button_menu->getContentWidth() - 25.f);
	m_name_menu->updateLayout();
	m_button_menu->updateLayout();

	return true;
}

void NamedIDItem::setDefaultBGColor(const ccColor4B& color)
{
	m_bg_color = color;

	m_bg->setColor(geode::cocos::to3B(m_bg_color));
	m_bg->setOpacity(m_bg_color.a);
}

void NamedIDItem::onEditButton(CCObject* sender)
{
	auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
	m_editing = !m_editing;

	if (m_editing && g_currentEditingItem && g_currentEditingItem != this)
	{
		g_currentEditingItem->m_editing = false;
		g_currentEditingItem->m_edit_button->getChildByID("save-sprite")->setVisible(false);
		g_currentEditingItem->m_edit_button->getNormalImage()->setVisible(true);
		g_currentEditingItem->m_cancel_button->setVisible(false);

		g_currentEditingItem->m_name_input->setString(
			NIDManager::getNameForID(m_id_type, g_currentEditingItem->m_id).unwrapOr("")
		);
		g_currentEditingItem->m_name_input->setEnabled(false);
	}

	button->getChildByID("save-sprite")->setVisible(m_editing);
	button->getNormalImage()->setVisible(!m_editing);
	m_cancel_button->setVisible(m_editing);

	m_name_input->setEnabled(m_editing);

	auto nameInputStr = m_name_input->getString();

	if (!m_editing)
	{
		if (nameInputStr.empty())
		{
			static_cast<void>(NIDManager::removeNamedID(m_id_type, m_id));
			m_name = "";
		}
		else
		{
			if (
				auto otherNamedID = NIDManager::getIDForName(m_id_type, nameInputStr);
				otherNamedID.isOkAnd([&](short id) { return id != m_id; })
			) {
				ng::utils::cocos::createNotificationToast(
					reinterpret_cast<CCLayer*>(CCScene::get()),
					fmt::format("ID {} already has this name!", otherNamedID.unwrap()),
					1.f, 85.f
				);

				m_name_input->setString(NIDManager::getNameForID(m_id_type, m_id).unwrapOr(""));

				return;
			}
			else if (auto res = NIDManager::saveNamedID(m_id_type, m_name_input->getString(), m_id); res.isErr())
			{
				ng::utils::cocos::createNotificationToast(
					reinterpret_cast<CCLayer*>(CCScene::get()),
					res.unwrapErr(),
					1.f, 85.f
				);

				m_name_input->setString(NIDManager::getNameForID(m_id_type, m_id).unwrapOr(""));

				return;
			}

			m_name = m_name_input->getString();

			m_name_input->getInputNode()->onClickTrackNode(false);
		}

		ng::utils::editor::refreshObjectLabels();

		if (m_name_input->getString().empty())
		{
			CCNode* parent = this->getParent();
			this->removeFromParent();
			parent->updateLayout();
		}
	}

	g_currentEditingItem = m_editing ? this : nullptr;
}

void NamedIDItem::onClearButton(CCObject*)
{
	if (!m_editing) return;

	m_name_input->setString("");
}

NamedIDItem::~NamedIDItem()
{
	g_currentEditingItem = nullptr;
}

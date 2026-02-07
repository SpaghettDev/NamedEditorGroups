#include "EditDescriptionPopup.hpp"

#include <string>

#include <NIDExtrasManager.hpp>

#include "utils.hpp"
#include "constants.hpp"

using namespace geode::prelude;

EditDescriptionPopup* EditDescriptionPopup::create(NID nid, short id)
{
	auto ret = new EditDescriptionPopup();

	if (ret && ret->init(nid, id))
		ret->autorelease();
	else
	{
		delete ret;
		ret = nullptr;
	}

	return ret;
}

bool EditDescriptionPopup::init(NID nid, short id)
{
	if (!Popup::init(285.f, 155.f))
		return false;

	m_id_type = nid;
	m_id = id;

	std::string description = NIDExtrasManager::getNamedIDDescription(nid, id).unwrapOr("");

	this->setID("EditDescriptionPopup");
	this->setTitle("Named ID Description");

	m_input = CCTextInputNode::create(220.f, 60.f, "Insert description", 5, nullptr);
	auto textArea = TextArea::create("Insert description", "chatFont.fnt", 1.f, 220.f, { .5f, .5f }, 20.f, true);
	m_input->addTextArea(textArea);
	m_input->setLabelPlaceholderColor({ 165, 165, 165 });
	m_input->m_delegate = this;
	this->m_buttonMenu->addChildAtPosition(m_input, Anchor::Center);

	auto background = cocos2d::extension::CCScale9Sprite::create("square02b_001.png", { .0f, .0f, 80.f, 80.f });
	background->setColor({ 0, 0, 0});
	background->setContentSize({ 250.f, 80.f });
	background->setOpacity(90);
	background->setZOrder(-1);
	this->m_buttonMenu->addChildAtPosition(background, Anchor::Center);

	auto deleteButtonSpr = CCSprite::createWithSpriteFrameName("GJ_longBtn07_001.png");
	deleteButtonSpr->setScale(.7f);
	m_delete_button = CCMenuItemSpriteExtra::create(
		deleteButtonSpr,
		this,
		menu_selector(EditDescriptionPopup::onDeleteButton)
	);
	this->m_buttonMenu->addChildAtPosition(m_delete_button, Anchor::Bottom, { -30.f, 20.f });

	auto saveButtonSpr = ButtonSprite::create(
		CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"),
		30, 0, 30.5f, 1.f, false, "baseBtn.png"_spr, true
	);
	saveButtonSpr->setScale(.75f);
	m_save_button = CCMenuItemSpriteExtra::create(
		saveButtonSpr,
		this,
		menu_selector(EditDescriptionPopup::onSaveButton)
	);
	this->m_buttonMenu->addChildAtPosition(m_save_button, Anchor::Bottom, { 30.f, 20.f });
	saveButtonSpr->m_subSprite->setPositionY(saveButtonSpr->m_subBGSprite->getPositionY() + 1.f);
	saveButtonSpr->m_subSprite->setScale(.75f);

	m_char_count = CCLabelBMFont::create(
		fmt::format("{}", ng::constants::MAX_DESCRIPTION_LENGTH - description.size()).c_str(),
		"chatFont.fnt"
	);
	m_char_count->setScale(.9f);
	m_char_count->setColor({ 0, 0, 0 });
	m_char_count->setOpacity(125);
	this->m_mainLayer->addChildAtPosition(m_char_count, Anchor::TopRight, { -19.f, -14.f });


	if (!description.empty())
		m_input->setString(description);

	return true;
}

void EditDescriptionPopup::textChanged(CCTextInputNode* input)
{
	std::string str = input->getString();

	std::int64_t size = ng::constants::MAX_DESCRIPTION_LENGTH - str.size();
	m_char_count->setString(
		fmt::format("{}", size).c_str()
	);

	if (str.size() > ng::constants::MAX_DESCRIPTION_LENGTH)
	{
		m_char_count->setColor({ 255, 0, 0 });
		m_char_count->setOpacity(255);
	}
	else
	{
		m_char_count->setColor({ 0, 0, 0 });
		m_char_count->setOpacity(125);
	}
}

void EditDescriptionPopup::onDeleteButton(CCObject*)
{
	m_input->setString("");
}

void EditDescriptionPopup::onSaveButton(CCObject*)
{
	if (m_input->getString().size() > ng::constants::MAX_DESCRIPTION_LENGTH)
		return ng::utils::cocos::createNotificationToast(this, "Description is too long!", .5f, 60.f);

	(void)NIDExtrasManager::setNamedIDDescription(
		m_id_type, m_id, std::move(m_input->getString())
	);

	this->removeFromParent();
}

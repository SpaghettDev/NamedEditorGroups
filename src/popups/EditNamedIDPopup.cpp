#include "EditNamedIDPopup.hpp"

#include <Geode/binding/LevelEditorLayer.hpp>

#include <NIDManager.hpp>

#include "utils.hpp"
#include "constants.hpp"

using namespace geode::prelude;

template <NID nid>
EditNamedIDPopup<nid>* EditNamedIDPopup<nid>::create(short id, std::function<void(short)>&& changedIDCallback, std::function<void()>&& savedCallback)
{
	auto ret = new EditNamedIDPopup<nid>();

	if (ret && ret->initAnchored(240.f, 150.f, id, std::move(changedIDCallback), std::move(savedCallback)))
		ret->autorelease();
	else
	{
		delete ret;
		ret = nullptr;
	}

	return ret;
}

template <NID nid>
bool EditNamedIDPopup<nid>::setup(short id, std::function<void(short)>&& changedIDCallback, std::function<void()>&& savedCallback)
{
	m_changed_id_callback = std::move(changedIDCallback);
	m_saved_callback = std::move(savedCallback);

	this->setID("EditNamedIDPopup");
	this->setTitle(fmt::format("Edit {} Name", ng::utils::getNamedIDIndentifier<nid>()));

	auto infoButton = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
		this,
		menu_selector(EditNamedIDPopup::onInfoIcon)
	);
	infoButton->setPosition({ 224.f, 133.f });
	this->m_buttonMenu->addChild(infoButton);

	m_left_arrow_button = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
		this,
		menu_selector(EditNamedIDPopup::onLeftArrow)
	);
	m_left_arrow_button->getNormalImage()->setScale(.775f);
	m_left_arrow_button->setPosition({ 70.f, 90.f });
	this->m_buttonMenu->addChild(m_left_arrow_button);

	m_right_arrow_button = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
		this,
		menu_selector(EditNamedIDPopup::onRightArrow)
	);
	static_cast<CCSprite*>(m_right_arrow_button->getNormalImage())->setFlipX(true);
	m_right_arrow_button->getNormalImage()->setScale(.775f);
	m_right_arrow_button->setPosition({ 170.f, 90.f });
	this->m_buttonMenu->addChild(m_right_arrow_button);

	m_id_input = geode::TextInput::create(65.f, "Num");
	m_id_input->setString(fmt::format("{}", id));
	m_id_input->setCommonFilter(CommonFilter::Uint);
	m_id_input->setMaxCharCount(4);
	m_id_input->setPosition({ 120.f, 90.f });
	m_id_input->setCallback([&](const std::string& str) { EditNamedIDPopup::onEditIDInput(this, std::move(str)); });
	this->m_buttonMenu->addChild(m_id_input);

	m_named_id_input = geode::TextInput::create(100.f, "Unnamed");
	m_named_id_input->setFilter(ng::constants::VALID_NAMED_ID_CHARACTERS);
	m_named_id_input->setPosition({ 120.f, 40.f });
	m_named_id_input->setCallback([&](const std::string& str) { EditNamedIDPopup::onEditIDNameInput(this, std::move(str)); });
	this->m_buttonMenu->addChild(m_named_id_input);

	auto clearNamedIDButton = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_longBtn07_001.png"),
		this,
		menu_selector(EditNamedIDPopup::onClearIDNameButton)
	);
	clearNamedIDButton->setPosition({ 40.f, 40.f });
	this->m_buttonMenu->addChild(clearNamedIDButton);

	auto saveNamedIDSprite = ButtonSprite::create(
		CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"),
		30, 0, 30.5f, 1.f, false, "baseBtn.png"_spr, true
	);
	saveNamedIDSprite->m_subSprite->setScale(.75f);
	auto saveNamedIDButton = CCMenuItemSpriteExtra::create(
		saveNamedIDSprite,
		this,
		menu_selector(EditNamedIDPopup::onSaveButton)
	);
	saveNamedIDSprite->m_subSprite->setPositionY(saveNamedIDSprite->getPositionY() + 1.f);
	saveNamedIDButton->setPosition({ 200.f, 40.f });
	this->m_buttonMenu->addChild(saveNamedIDButton);


	if (auto namedID = NIDManager::getNameForID<nid>(id); namedID.isOk())
		m_named_id_input->setString(fmt::format("{}", namedID.unwrap()));

	return true;
}

template <NID nid>
void EditNamedIDPopup<nid>::onInfoIcon(CCObject*)
{
	FLAlertLayer::create(
		nullptr,
		"Help",
		fmt::format(
			"You can abstract <ca>IDs</c> away by giving them a <cg>name</c>.\n"
			"The ID's name <cr>cannot</c> exceed <cy>20 characters</c> "
			"<cg>(it's recommended to keep them shorter than 14 characters)</c>, "
			"<cr>cannot</c> contain <cy>only numbers</c> or <cy>only symbols</c>, "
			"and each ID <cr>must</c> have its own <cy>unique name</c>.\n"
			"Valid character are: <cg>{}</c>",
			ng::constants::VALID_NAMED_ID_CHARACTERS
		),
		"Ok", nullptr,
		400.f
	)->show();
}

template <NID nid>
void EditNamedIDPopup<nid>::onLeftArrow(CCObject*)
{
	if (auto parsedNum = geode::utils::numFromString<short>(m_id_input->getString()); parsedNum.isOk())
	{
		short namedID = parsedNum.unwrap();
		namedID--;

		m_id_input->setString(fmt::format("{}", namedID));

		if (namedID <= 0)
		{
			m_id_input->setString("1");
			namedID = 1;
		}
		else if (auto idName = NIDManager::getNameForID<nid>(namedID); idName.isOk())
			m_named_id_input->setString(idName.unwrap());
		else
			m_named_id_input->setString("");

		if (m_changed_id_callback)
			m_changed_id_callback(namedID);
	}
	else
	{
		m_id_input->setString("1");

		if (m_changed_id_callback)
			m_changed_id_callback(1);
	}
}

template <NID nid>
void EditNamedIDPopup<nid>::onRightArrow(CCObject*)
{
	if (auto parsedNum = geode::utils::numFromString<short>(m_id_input->getString()); parsedNum.isOk())
	{
		short namedID = parsedNum.unwrap();
		namedID++;

		m_id_input->setString(fmt::format("{}", namedID));

		if (namedID > 9999)
		{
			m_id_input->setString("9999");
			namedID = 9999;
		}
		else if (auto idName = NIDManager::getNameForID<nid>(namedID); idName.isOk())
			m_named_id_input->setString(idName.unwrap());
		else
			m_named_id_input->setString("");

		if (m_changed_id_callback)
			m_changed_id_callback(namedID);
	}
	else
	{
		m_id_input->setString("9999");

		if (m_changed_id_callback)
			m_changed_id_callback(9999);
	}
}

template <NID nid>
void EditNamedIDPopup<nid>::onSaveButton(CCObject*)
{
	auto&& namedIDStr = m_named_id_input->getString();
	auto idStr = geode::utils::numFromString<short>(m_id_input->getString());

	if (idStr.isErr())
		return ng::utils::cocos::createNotificationToast(this, "Invalid ID!", 1.f, 85.f);

	if (
		auto otherNamedID = NIDManager::getIDForName<nid>(namedIDStr);
		otherNamedID.isOkAnd([&](short id) { return id != idStr.unwrap(); })
	)
		return ng::utils::cocos::createNotificationToast(
			this,
			fmt::format("{} {} already has this name!", ng::utils::getNamedIDIndentifier<nid>(), otherNamedID.unwrap()),
			1.f, 85.f
		);

	if (namedIDStr.empty())
		static_cast<void>(NIDManager::removeNamedID<nid>(idStr.unwrap()));
	else if (auto res = NIDManager::saveNamedID<nid>(std::move(namedIDStr), idStr.unwrap()); res.isErr())
		return ng::utils::cocos::createNotificationToast(this, res.unwrapErr(), 1.f, 85.f);

	ng::utils::editor::refreshObjectLabels();

	if (m_saved_callback)
		m_saved_callback();

	this->removeFromParent();
}

template <NID nid>
void EditNamedIDPopup<nid>::onClearIDNameButton(CCObject*)
{
	m_named_id_input->setString("");
}


template <NID nid>
void EditNamedIDPopup<nid>::onEditIDInput(EditNamedIDPopup<nid>* self, const std::string& str)
{
	if (auto parsedNum = geode::utils::numFromString<short>(self->m_id_input->getString()); parsedNum.isOk())
	{
		short nameID = parsedNum.unwrap();

		// if (auto idName = NIDManager::getNameForID<nid>(nameID); idName.isOk())
		// 	self->m_named_id_input->setString(idName.unwrap());
		// else
		// 	self->m_named_id_input->setString("");

		self->m_changed_id_callback(nameID);
	}
}

template <NID nid>
void EditNamedIDPopup<nid>::onEditIDNameInput(EditNamedIDPopup<nid>* self, const std::string& str)
{
	if (auto id = NIDManager::getIDForName<nid>(str); id.isOk())
	{
		self->m_id_input->setString(fmt::format("{}", id.unwrap()));
		self->m_changed_id_callback(id.unwrap());
	}
}

template class EditNamedIDPopup<NID::GROUP>;
template class EditNamedIDPopup<NID::COLLISION>;
template class EditNamedIDPopup<NID::COUNTER>;

#include "AddNamedIDPopup.hpp"

#include <NIDManager.hpp>

#include "utils.hpp"

using namespace geode::prelude;

AddNamedIDPopup* AddNamedIDPopup::create(NID nid, std::function<void(const std::string&, short)>&& savedCallback)
{
	auto ret = new AddNamedIDPopup();

	if (ret)
	{
		ret->m_nid = nid;
		ret->m_saved_callback = std::move(savedCallback);
	}

	if (ret && ret->init(1, nullptr, nullptr))
		ret->autorelease();
	else
	{
		delete ret;
		ret = nullptr;
	}

	return ret;
}

bool AddNamedIDPopup::init(short, std::function<void(short)>&&, std::function<void()>&&)
{
	if (!EditNamedIDPopup<NID::GROUP>::init(1, nullptr, nullptr)) return false;

	this->setID("AddNamedIDPopup");
	this->setTitle(fmt::format("New {} Name", ng::utils::getNamedIDIndentifier(m_nid)));

	this->m_left_arrow_button->setVisible(false);
	this->m_right_arrow_button->setVisible(false);

	this->m_id_input->setString("");
	this->m_named_id_input->setString("");

	this->m_id_input->setCallback([](const std::string&) {});
	this->m_named_id_input->setCallback([](const std::string&) {});

	return true;
}

void AddNamedIDPopup::onSaveButton(CCObject*)
{
	if (this->m_id_input->getString().empty() || this->m_named_id_input->getString().empty())
		return;

	auto&& namedIDStr = this->m_named_id_input->getString();
	auto namedID = geode::utils::numFromString<short>(this->m_id_input->getString());

	if (namedID.isErr())
		return;

	if (
		auto otherNamedID = NIDManager::getIDForName(m_nid, namedIDStr);
		otherNamedID.isOkAnd([&](short id) { return id != namedID.unwrap(); })
	)
		return ng::utils::cocos::createNotificationToast(
			this,
			fmt::format("{} {} already has this name!", ng::utils::getNamedIDIndentifier(m_nid), otherNamedID.unwrap()),
			1.f, 45.f
		);

	if (auto res = NIDManager::saveNamedID(m_nid, std::move(namedIDStr), namedID.unwrap()); res.isErr())
		return ng::utils::cocos::createNotificationToast(this, res.unwrapErr(), 1.f, 45.f);

	ng::utils::editor::refreshObjectLabels();

	if (m_saved_callback)
		m_saved_callback(namedIDStr, namedID.unwrap());

	this->removeFromParent();
}

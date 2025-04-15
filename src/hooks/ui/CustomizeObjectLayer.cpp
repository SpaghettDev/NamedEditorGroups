#include <Geode/modify/CustomizeObjectLayer.hpp>

#include <NIDManager.hpp>

#include "constants.hpp"
#include "globals.hpp"

using namespace geode::prelude;

struct NIDCustomizeObjectLayer : geode::Modify<NIDCustomizeObjectLayer, CustomizeObjectLayer>
{
	struct Fields
	{
		geode::TextInput* m_input;
	};

	bool init(GameObject* p0, CCArray* p1)
	{
		if (!CustomizeObjectLayer::init(p0, p1)) return false;

		auto channelMenu = this->m_mainLayer->getChildByID("selected-channel-menu");
		auto colorEditBtn = this->m_mainLayer->querySelector("ok-menu > live-color-select-button");
		auto idLabel = channelMenu->getChildByID("selected-channel-label");
		const CCPoint& idLabelPos = idLabel->getPosition();

		static_cast<CCMenuItemSpriteExtra*>(
			channelMenu->getChildByID("select-channel-button")
		)->getNormalImage()->setScale(.8f);
		colorEditBtn->setPositionY(colorEditBtn->getPositionY() + (ng::globals::g_isBetterEditLoaded ? -2.f : 12.f));
		for (auto child : CCArrayExt<CCNode*>(channelMenu->getChildren()))
			child->setPositionY(child->getPositionY() + (ng::globals::g_isBetterEditLoaded ? 11.f : 13.f));

		auto colorNameInput = geode::TextInput::create(110.f, "Search...");
		colorNameInput->setContentHeight(20.f);
		colorNameInput->setFilter(ng::constants::VALID_NAMED_ID_CHARACTERS);
		colorNameInput->setCallback([&](const std::string& str) {
			this->onEditInput(str);
		});
		colorNameInput->setPosition({ idLabelPos.x + 10.f, idLabelPos.y - 29.f });
		colorNameInput->setID("color-name-input"_spr);
		colorNameInput->getBGSprite()->setContentSize({ 220.f, 55.f });
		colorNameInput->setScale(.6f);
		channelMenu->addChild(colorNameInput);
		m_fields->m_input = colorNameInput;

		colorNameInput->setString(
			NIDManager::getNameForID<NID::COLOR>(getColorID()).unwrapOr("")
		);

		return true;
	}

	void colorSetupClosed(int p0)
	{
		CustomizeObjectLayer::colorSetupClosed(p0);

		m_fields->m_input->setString(
			NIDManager::getNameForID<NID::COLOR>(getColorID()).unwrapOr("").c_str()
		);
	}

	void updateColorSprite()
	{
		CustomizeObjectLayer::updateColorSprite();

		if (!m_fields->m_input) return;

		m_fields->m_input->setString(
			NIDManager::getNameForID<NID::COLOR>(getColorID()).unwrapOr("").c_str()
		);
	}


	short getColorID()
	{
		return LevelEditorLayer::get()->m_levelSettings->m_effectManager->getColorAction(
			this->getActiveMode(true)
		)->m_colorID;
	}

	void onEditInput(const std::string& str)
	{
		if (auto idRes = NIDManager::getIDForName<NID::COLOR>(str))
		{
			short id = idRes.unwrap();

			this->colorSetupClosed(id);
			this->m_customColorInput->setString(fmt::format("{}", id));

			// this->updateSelected(id);
			// // this->highlightSelected(button);
			// this->updateColorSprite();
			// this->updateChannelLabel(this->getActiveMode(true));
		}
	}
};

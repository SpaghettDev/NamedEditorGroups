#include <Geode/modify/LevelSettingsLayer.hpp>

#include "../popups/NamedIDsPopup.hpp"

using namespace geode::prelude;

struct NIDLevelSettingsLayer : geode::Modify<NIDLevelSettingsLayer, LevelSettingsLayer>
{
	bool init(LevelSettingsObject* p0, LevelEditorLayer* p1)
	{
		if (!LevelSettingsLayer::init(p0, p1)) return false;

		auto nidSettingsSpr = CCSprite::createWithSpriteFrameName("GJ_menuBtn_001.png");
		nidSettingsSpr->setScale(.65f);
		auto nidSettingsButton = CCMenuItemSpriteExtra::create(
			nidSettingsSpr,
			this,
			menu_selector(NIDLevelSettingsLayer::onNIDSettingsButton)
		);
		nidSettingsButton->setPosition({ 185.f, -115.f });
		nidSettingsButton->setID("settings-button"_spr);
		this->m_buttonMenu->addChild(nidSettingsButton);

		return true;
	}


	void onNIDSettingsButton(CCObject*)
	{
		NamedIDsPopup::create(false)->show();
	}
};

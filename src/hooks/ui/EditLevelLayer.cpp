#include <Geode/modify/EditLevelLayer.hpp>

#include <NIDManager.hpp>

#include "../LevelEditorLayerData.hpp"

#include "../popups/NamedIDsPopup.hpp"

struct NIDEditLevelLayer : geode::Modify<NIDEditLevelLayer, EditLevelLayer>
{
	bool init(GJGameLevel* p0)
	{
		if (!EditLevelLayer::init(p0)) return false;

		NIDManager::reset();

#ifdef GEODE_IS_ANDROID
		auto decompressedString = std::string{
			cocos2d::ZipUtils::decompressString(this->m_level->m_levelString, false, 11)
		};
#else
		auto decompressedString = cocos2d::ZipUtils::decompressString(
			this->m_level->m_levelString, false, 11
		);
#endif

		if (auto res = LevelEditorLayerData::parseDataString(decompressedString); res.isErr())
		{
			const auto& [err, saveObjStr] = res.unwrapErr();

			auto errorPopup = FLAlertLayer::create(
				nullptr,
				"Error parsing save object",
				fmt::format(
					"{}\n"
					"Save string has been copied to clipboard "
					"and the save object has been deleted.",
					err
				),
				"OK",
				nullptr,
				350.f
			);
			errorPopup->m_scene = this;
			errorPopup->show();

			geode::utils::clipboard::write(saveObjStr);
		}

		if (geode::Mod::get()->getSettingValue<bool>("show-edit-level-preview-button") && !NIDManager::isEmpty())
		{
			auto nidSettingsSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_menuBtn_001.png");
			nidSettingsSpr->setScale(.85f);
			auto nidSettingsButton = CCMenuItemSpriteExtra::create(
				nidSettingsSpr,
				this,
				menu_selector(NIDEditLevelLayer::onNIDSettingsButton)
			);
			nidSettingsButton->setID("settings-button"_spr);
			this->getChildByID("level-actions-menu")->addChild(nidSettingsButton);
			this->getChildByID("level-actions-menu")->updateLayout(false);
		}

		return true;
	}


	void onNIDSettingsButton(CCObject*)
	{
		NamedIDsPopup::create(true)->show();
	}
};

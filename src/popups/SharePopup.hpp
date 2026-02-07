#pragma once

#include <functional>

#include <Geode/ui/Popup.hpp>

class SharePopup : public geode::Popup
{
public:
	static SharePopup* create(std::function<void(bool)>&&, std::function<void(bool)>&&);

protected:
	bool init(std::function<void(bool)>&&, std::function<void(bool)>&&);

public:
	void onImportButton(cocos2d::CCObject*);
	void onExportButton(cocos2d::CCObject*);

protected:
	CCMenuItemSpriteExtra* m_import_button;
	CCMenuItemSpriteExtra* m_export_button;

	cocos2d::CCLabelBMFont* m_char_count;

	std::function<void(bool)> m_on_imported_callback;
	std::function<void(bool)> m_on_exported_callback;
};

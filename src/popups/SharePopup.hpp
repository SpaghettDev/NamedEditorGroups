#pragma once

#include <Geode/ui/Popup.hpp>

class SharePopup : public geode::Popup<>
{
public:
	static SharePopup* create();

protected:
	bool setup() override;

public:
	void onImportButton(cocos2d::CCObject*);
	void onExportButton(cocos2d::CCObject*);

protected:
	CCMenuItemSpriteExtra* m_import_button;
	CCMenuItemSpriteExtra* m_export_button;

	cocos2d::CCLabelBMFont* m_char_count;
};

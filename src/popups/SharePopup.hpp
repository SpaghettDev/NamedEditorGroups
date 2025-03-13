#pragma once

#include <Geode/ui/Popup.hpp>

class SharePopup : public geode::Popup<>
{
public:
	static SharePopup* create();

protected:
	bool setup() override;

public:

protected:
	CCTextInputNode* m_input;
	CCMenuItemSpriteExtra* m_delete_button;
	CCMenuItemSpriteExtra* m_save_button;

	cocos2d::CCLabelBMFont* m_char_count;
};

#pragma once

#include <string>

#include <Geode/cocos/base_nodes/CCNode.h>

#include <NIDEnum.hpp>

class NamedIDItem : public cocos2d::CCNode
{
public:
	static NamedIDItem* create(NID, short, std::string&&, float);

protected:
	bool init(NID, short, std::string&&, float);

public:
	void setDefaultBGColor(const cocos2d::ccColor4B& color);

	void onEditButton(cocos2d::CCObject*);
	void onClearButton(cocos2d::CCObject*);

	short getID() const { return m_id; }
	const std::string& getName() const& { return m_name; }

private:
	inline static NamedIDItem* g_currentEditingItem = nullptr;

	NID m_id_type;
	short m_id;
	std::string m_name;

	cocos2d::CCLayerColor* m_bg;
	cocos2d::CCMenu* m_name_menu;
	cocos2d::CCMenu* m_button_menu;
	ButtonSprite* m_name_label;
	geode::TextInput* m_name_input;
	CCMenuItemSpriteExtra* m_edit_button;
	CCMenuItemSpriteExtra* m_cancel_button;
	cocos2d::ccColor4B m_bg_color = { 0, 0, 0, 0 };

	bool m_editing = false;
};

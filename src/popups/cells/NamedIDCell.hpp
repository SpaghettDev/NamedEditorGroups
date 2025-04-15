#pragma once

#include <string>

#include <Geode/cocos/base_nodes/CCNode.h>

#include <Geode/binding/ColorChannelSprite.hpp>
#include <Geode/binding/ColorSelectDelegate.hpp>

#include <NIDEnum.hpp>

class NamedIDCell : public cocos2d::CCNode, public ColorSelectDelegate
{
public:
	static NamedIDCell* create(NID, short, std::string&&, bool, float);

protected:
	bool init(NID, short, std::string&&, bool, float);

	virtual ~NamedIDCell();

public:
	void setDefaultBGColor(const cocos2d::ccColor4B& color);

	void onEditButton(cocos2d::CCObject*);
	void onClearButton(cocos2d::CCObject*);
	void onSelectColor(cocos2d::CCObject*);
	void onPreviewButton(cocos2d::CCObject*);
	void onDescriptionButton(cocos2d::CCObject*);

	void showAdvancedOptions(bool);

	virtual void colorSelectClosed(cocos2d::CCNode*) override;

	short getID() const { return m_id; }
	const std::string& getName() const& { return m_name; }

private:
	inline static NamedIDCell* g_currentEditingItem = nullptr;

	NID m_id_type;
	short m_id;
	std::string m_name;

	bool m_preview_toggled = true;

	cocos2d::CCLayerColor* m_bg;
	cocos2d::CCMenu* m_name_menu;
	cocos2d::CCMenu* m_button_menu;
	ButtonSprite* m_name_label;
	CCMenuItemSpriteExtra* m_color_button = nullptr;
	ColorChannelSprite* m_color_sprite = nullptr;
	CCMenuItemSpriteExtra* m_preview_button;
	CCMenuItemSpriteExtra* m_description_button;
	geode::TextInput* m_name_input;
	CCMenuItemSpriteExtra* m_edit_button;
	CCMenuItemSpriteExtra* m_cancel_button;
	cocos2d::ccColor4B m_bg_color = { 0, 0, 0, 0 };

	bool m_editing = false;
};

#pragma once

#include <string>
#include <string_view>
#include <array>

#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/cocos/touch_dispatcher/CCTouchDelegateProtocol.h>

#include <Geode/binding/ColorChannelSprite.hpp>
#include <Geode/binding/ColorSelectDelegate.hpp>

#include <NIDEnum.hpp>

template <bool SHORT>
class NamedIDCell;

template <>
class NamedIDCell<false> : public cocos2d::CCNode, public ColorSelectDelegate
{
public:
	static NamedIDCell<false>* create(NID, short, std::string&&, bool, bool, float);

protected:
	bool init(NID, short, std::string&&, bool, bool, float);

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
	inline static NamedIDCell<false>* g_currentEditingItem = nullptr;

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


template <>
class NamedIDCell<true> : public cocos2d::CCNode, public cocos2d::CCTouchDelegate
{
public:
	static NamedIDCell<true>* create(NID, short, std::string&&, float);

protected:
	bool init(NID, short, std::string&&, float);

public:
	void setDefaultBGColor(const cocos2d::ccColor4B& color);
	void highlightQuery(const std::string_view);
	void highlightQuery(const std::string_view, const std::array<std::uint8_t, 256>&);

	virtual void onEnter() override;
	virtual void onEnterTransitionDidFinish() override;
	virtual void onExit() override;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) override;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) override;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) override;
	void registerWithTouchDispatcher();
	void unregisterWithTouchDispatcher();

	void setSelectCallback(std::function<void(NID, short)>&& cb) { m_on_select_cb = std::move(cb); }

	short getID() const { return m_id; }
	const std::string& getName() const& { return m_name; }

private:
	static constexpr std::uint32_t TOUCH_MOVED_EVENT_TAG = 0xB00B1E5;

	NID m_id_type;
	short m_id;
	std::string m_name;

	bool m_registered_touch = false;

	std::function<void(NID, short)> m_on_select_cb = [](...) {};

	cocos2d::CCLayerColor* m_bg;
	cocos2d::CCMenu* m_name_menu;
	cocos2d::CCMenu* m_button_menu;
	ButtonSprite* m_name_label;
	ColorChannelSprite* m_color_sprite = nullptr;
	geode::TextInput* m_name_input;
	cocos2d::ccColor4B m_bg_color = { 0, 0, 0, 0 };
};

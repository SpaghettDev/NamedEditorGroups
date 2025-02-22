#pragma once

#include <Geode/ui/Popup.hpp>

#include <NIDEnum.hpp>

class NamedIDsPopup : public geode::Popup<>
{
public:
	static NamedIDsPopup* create();

protected:
	bool setup() override;

public:
	void onClearSearchButton(cocos2d::CCObject*);
	void onFilterButton(cocos2d::CCObject*);
	void onAddButton(cocos2d::CCObject*);

	void updateList(NID);
	void updateState();

private:
	static constexpr cocos2d::CCSize SCROLL_LAYER_SIZE{ 260.f, 215.f };

	NID m_ids_type = NID::GROUP;

	cocos2d::CCLayerColor* m_layer_bg;
	cocos2d::CCMenu* m_search_container;
	geode::TextInput* m_search_input;
	geode::ScrollLayer* m_list;
};

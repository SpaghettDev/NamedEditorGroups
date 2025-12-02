#pragma once

#include "cells/NamedIDCell.hpp"
#include <string_view>

#include <NIDEnum.hpp>

#include <Geode/ui/TextInput.hpp>

#include <Geode/cocos/layers_scenes_transitions_nodes/CCLayer.h>
#include <Geode/cocos/extensions/GUI/CCControlExtension/CCScale9Sprite.h>

class AutofillNamedIDsPreview : public cocos2d::CCLayer
{
public:
	static AutofillNamedIDsPreview* create(NID, const std::string_view);

protected:
	bool init(NID, const std::string_view);

public:
	void attachToInput(geode::TextInput*);
	void show();

	void updateList(NID, const std::string_view);
	void updateList(const std::string_view);

	virtual void keyBackClicked() override;
	virtual void onEnterTransitionDidFinish() override;
	virtual void onExit() override;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) override;
	virtual void registerWithTouchDispatcher() override;

	void setSelectCallback(std::function<void(NID, short)>&& cb) { m_select_callback = std::move(cb); };

private:
	void selectCallback(NID, short);

private:
	static constexpr cocos2d::CCSize PREVIEW_SIZE{ 190.f, 110.f };
	static constexpr cocos2d::CCSize SCROLL_LAYER_SIZE{ 168.f, 92.f };

	NID m_ids_type;
	std::string m_query;


	std::function<void(NID, short)> m_select_callback;
	std::unordered_map<short, geode::Ref<NamedIDCell<true>>> m_cells;


	cocos2d::extension::CCScale9Sprite* m_bg_sprite;
	geode::Ref<cocos2d::CCLayerColor> m_layer_bg;
	geode::Ref<geode::ScrollLayer> m_list;
	geode::Ref<geode::Scrollbar> m_scroll_bar;
};

#pragma once

#include <functional>

#include <Geode/ui/Popup.hpp>

#include <NIDEnum.hpp>

class SelectIDFilterPopup : public geode::Popup<NID, std::function<void(NID)>&&>
{
public:
	static SelectIDFilterPopup* create(NID, std::function<void(NID)>&&);

protected:
	bool setup(NID, std::function<void(NID)>&&) override;

public:
	void onToggleButton(cocos2d::CCObject*);

private:
	std::function<void(NID)> m_on_changed_callback;

	cocos2d::CCMenu* m_toggles_menu;
};

#pragma once

#include <functional>

#include <Geode/ui/Popup.hpp>

#include <NIDEnum.hpp>

template <NID nid>
class EditNamedIDPopup : public geode::Popup<short, std::function<void(short)>&&, std::function<void()>&&>
{
public:
	static EditNamedIDPopup* create(short, std::function<void(short)>&&, std::function<void()>&&);

protected:
	bool setup(short, std::function<void(short)>&&, std::function<void()>&&) override;
	virtual void keyBackClicked() override {};
	virtual void keyDown(cocos2d::enumKeyCodes) override {};

public:
	void onInfoIcon(cocos2d::CCObject*);

	void onLeftArrow(cocos2d::CCObject*);
	void onRightArrow(cocos2d::CCObject*);
	void onClearIDNameButton(cocos2d::CCObject*);
	virtual void onSaveButton(cocos2d::CCObject*);

	static void onEditIDInput(EditNamedIDPopup*, const std::string&);
	static void onEditIDNameInput(EditNamedIDPopup*, const std::string&);

protected:
	geode::TextInput* m_id_input;
	geode::TextInput* m_named_id_input;
	CCMenuItemSpriteExtra* m_left_arrow_button;
	CCMenuItemSpriteExtra* m_right_arrow_button;

	std::function<void(short)> m_changed_id_callback;
	std::function<void()> m_saved_callback;
};

static inline void ShowEditNamedIDPopup(NID nid, short id, std::function<void(short)>&& changedIDCallback, std::function<void()>&& savedCallback)
{
	if (nid == NID::GROUP)
		EditNamedIDPopup<NID::GROUP>::create(id, std::move(changedIDCallback), std::move(savedCallback))->show();
	else if (nid == NID::COLLISION)
		EditNamedIDPopup<NID::COLLISION>::create(id, std::move(changedIDCallback), std::move(savedCallback))->show();
	else if (nid == NID::COUNTER)
		EditNamedIDPopup<NID::COUNTER>::create(id, std::move(changedIDCallback), std::move(savedCallback))->show();
}

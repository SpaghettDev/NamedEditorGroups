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
	virtual void keyDown(cocos2d::enumKeyCodes) override;
	virtual void onExit() override;

public:
	void onInfoIcon(cocos2d::CCObject*);

	void onLeftArrow(cocos2d::CCObject*);
	void onRightArrow(cocos2d::CCObject*);
	void onClearIDNameButton(cocos2d::CCObject*);
	virtual void onSaveButton(cocos2d::CCObject*);

	virtual void registerWithTouchDispatcher() override;

	geode::TextInput* getIDInput() { return m_id_input; }
	geode::TextInput* getNamedIDInput() { return m_named_id_input; }
	CCMenuItemSpriteExtra* getLeftArrowButton() { return m_left_arrow_button; }
	CCMenuItemSpriteExtra* getRightArrowButton() { return m_right_arrow_button; }

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

static inline void ShowEditNamedIDPopup(const NID nid, short id, std::function<void(short)>&& changedIDCallback, std::function<void()>&& savedCallback)
{
	switch (nid)
	{
		case NID::GROUP:
			EditNamedIDPopup<NID::GROUP>::create(id, std::move(changedIDCallback), std::move(savedCallback))->show(); break;
		case NID::COLLISION:
			EditNamedIDPopup<NID::COLLISION>::create(id, std::move(changedIDCallback), std::move(savedCallback))->show(); break;
		case NID::COUNTER:
			EditNamedIDPopup<NID::COUNTER>::create(id, std::move(changedIDCallback), std::move(savedCallback))->show(); break;
		case NID::TIMER:
			EditNamedIDPopup<NID::TIMER>::create(id, std::move(changedIDCallback), std::move(savedCallback))->show(); break;
		case NID::EFFECT:
			EditNamedIDPopup<NID::EFFECT>::create(id, std::move(changedIDCallback), std::move(savedCallback))->show(); break;
		case NID::COLOR:
			EditNamedIDPopup<NID::COLOR>::create(id, std::move(changedIDCallback), std::move(savedCallback))->show(); break;

		default:
			throw "Invalid NID enum value";
	}
}

#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/binding/TextInputDelegate.hpp>

#include <NIDEnum.hpp>

class EditDescriptionPopup : public geode::Popup<NID, short>, TextInputDelegate
{
public:
	static EditDescriptionPopup* create(NID, short);

protected:
	bool setup(NID, short) override;

public:
	virtual void textChanged(CCTextInputNode*) override;

	void onDeleteButton(cocos2d::CCObject*);
	void onSaveButton(cocos2d::CCObject*);

protected:
	NID m_id_type;
	short m_id;

	CCTextInputNode* m_input;
	CCMenuItemSpriteExtra* m_delete_button;
	CCMenuItemSpriteExtra* m_save_button;

	cocos2d::CCLabelBMFont* m_char_count;
};

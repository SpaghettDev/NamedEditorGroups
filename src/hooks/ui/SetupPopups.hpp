#pragma once

#include <functional>
#include <array>
#include <vector>
#include <unordered_map>

#include <Geode/modify/SetupMoveCommandPopup.hpp>
#include <Geode/modify/SetupTriggerPopup.hpp>

#include <NIDEnum.hpp>

struct NIDSetupTriggerPopup : geode::Modify<NIDSetupTriggerPopup, SetupTriggerPopup>
{
	struct IDInputInfo
	{
		geode::TextInput* namedIDInput;
		CCMenuItemSpriteExtra* editInputButton;
		NID idType;

		CCTextInputNode* idInput;
		std::array<CCMenuItemSpriteExtra*, 2> arrowButtons{ nullptr, nullptr };

		operator bool() { return static_cast<bool>(idInput); }
	};

	struct Fields
	{
		std::unordered_map<std::uint16_t, IDInputInfo> m_id_inputs;
		short m_objectID;
	};

	cocos2d::CCArray* createValueControlAdvanced(int, gd::string, cocos2d::CCPoint, float, bool, InputValueType, int, bool, float, float, int, int, GJInputStyle, int, bool);

	void triggerArrowLeft(cocos2d::CCObject*);
	void triggerArrowRight(cocos2d::CCObject*);
	void onToggleTriggerValue(cocos2d::CCObject*);
	virtual void textChanged(CCTextInputNode*) override;

	void valueChanged(int, float);
	void updateValue(int, float);

	
	void triggerArrowWasClicked(int, bool);
	void textWasChanged(CCTextInputNode*);
	void onEditIDNameButton(CCObject*);

	void handleSpecialCases(int, std::uint16_t, cocos2d::CCArray*);

	static NID evaluateDynamicType(SetupTriggerPopup*, NID, short);
	static IDInputInfo commonInputSetup(cocos2d::CCLayer*, NID, std::uint16_t, std::vector<CCNode*>&&, CCNode*, CCNode*, std::function<void(std::vector<CCNode*>&&)>&& = nullptr);

	static void onEditInput(NIDSetupTriggerPopup*, std::uint16_t, const std::string&);
};

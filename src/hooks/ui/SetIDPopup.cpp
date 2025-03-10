// #include <Geode/modify/SetIDPopup.hpp>

// #include "SetupPopups.hpp"

// #include <NIDManager.hpp>

// #include "utils.hpp"

// using namespace geode::prelude;

// NID nidForPopup(SetIDPopup* popup)
// {
// 	if (typeinfo_cast<FindObjectPopup*>(popup))
// 		return NID::GROUP;
// 	else if (typeinfo_cast<SetIDPopup*>(popup))
// 		return NID::GROUP;
// 	else if (typeinfo_cast<SetColorIDPopup*>(popup))
// 		return NID::COLOR;

// 	throw "Invalid SetIDPopup type!";
// }

// struct NIDSetIDPopup : geode::Modify<NIDSetIDPopup, SetIDPopup>
// {
// 	struct Fields
// 	{

// 	};

// 	bool init(int current, int begin, int end, gd::string title, gd::string button, bool hasResetButton, int defaultValue, float yOffset, bool isNumberInput, bool isVertical)
// 	{
// 		if (!SetIDPopup::init(current, begin, end, title, button, hasResetButton, defaultValue, yOffset, isNumberInput, isVertical)) return false;

// 		if (
// 			typeinfo_cast<FindObjectPopup*>(this) ||
// 			// SetIDPopup is used in LevelBrowserLayer
// 			(typeinfo_cast<SetIDPopup*>(this) && isNumberInput) ||
// 			typeinfo_cast<SetColorIDPopup*>(this)
// 		) {
// 			std::vector<CCNode*> nodes;
// 			nodes.reserve(4);

// 			nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
// 				this->m_mainLayer,
// 				[](CCNode* c) {
// 					return typeinfo_cast<CCTextInputNode*>(c) != nullptr;
// 				}
// 			));
// 			nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
// 				this->m_mainLayer,
// 				[inputNodePos = nodes[0]->getPosition()](CCNode* c) {
// 					auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
// 					return castedNode && castedNode->getPosition() == inputNodePos;
// 				}
// 			));
// 			nodes.emplace_back(this->m_buttonMenu->getChildByTag(1));
// 			nodes.emplace_back(this->m_buttonMenu->getChildByTag(2));

// 			auto inputInfo = NIDSetupTriggerPopup::commonInputSetup(
// 				this,
// 				NID,
// 				0,
// 				std::move(nodes),
// 				this->m_mainLayer,
// 				this->m_buttonMenu
// 			);
// 		}

// 		return true;
// 	}
// };

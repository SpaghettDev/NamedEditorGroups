#include <Geode/modify/SetIDPopup.hpp>

#include "SetupPopups.hpp"
#include "../popups/EditNamedIDPopup.hpp"

#include <NIDManager.hpp>

#include "utils.hpp"

using namespace geode::prelude;

NID nidForPopup(SetIDPopup* popup)
{
	// order is important here
	if (typeinfo_cast<SetColorIDPopup*>(popup))
		return NID::COLOR;
	else if (typeinfo_cast<FindObjectPopup*>(popup))
		return NID::GROUP;
	else if (typeinfo_cast<SetIDPopup*>(popup))
		return NID::GROUP;

	throw "Invalid SetIDPopup type!";
}

struct NIDSetIDPopup : geode::Modify<NIDSetIDPopup, SetIDPopup>
{
	struct Fields
	{
		NIDSetupTriggerPopup::IDInputInfo m_id_input{
			{}, nullptr, NID::GROUP, nullptr, { nullptr, nullptr }
		};
	};

	bool init(int current, int begin, int end, gd::string title, gd::string button, bool hasResetButton, int defaultValue, float yOffset, bool isNumberInput, bool isVerticalArrows)
	{
		if (!SetIDPopup::init(current, begin, end, title, button, hasResetButton, defaultValue, yOffset, isNumberInput, isVerticalArrows)) return false;

		if (
			typeinfo_cast<FindObjectPopup*>(this) ||
			// SetIDPopup is used in LevelBrowserLayer
			(typeinfo_cast<SetIDPopup*>(this) && isNumberInput) ||
			typeinfo_cast<SetColorIDPopup*>(this)
		) {
			std::vector<CCNode*> nodes;
			nodes.reserve(4);

			nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
				this->m_mainLayer,
				[](CCNode* c) {
					return typeinfo_cast<CCTextInputNode*>(c) != nullptr;
				}
			));
			nodes.emplace_back(ng::utils::cocos::getChildByPredicate(
				this->m_mainLayer,
				[inputNodePos = nodes[0]->getPosition()](CCNode* c) {
					auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
					return castedNode && castedNode->getPosition() == inputNodePos;
				}
			));
			nodes.emplace_back(this->m_buttonMenu->getChildByTag(1));
			nodes.emplace_back(this->m_buttonMenu->getChildByTag(2));

			auto inputInfo = NIDSetupTriggerPopup::commonInputSetup(
				this,
				nidForPopup(this),
				0,
				std::move(nodes),
				this->m_mainLayer,
				this->m_buttonMenu
			);
			inputInfo.namedIDInput.setEditInputCallback([&](const std::string& str) {
				NIDSetIDPopup::onEditInput(this, std::move(str));
			});
			inputInfo.editInputButton->m_pfnSelector = menu_selector(NIDSetIDPopup::onEditIDNameButton);

			m_fields->m_id_input = std::move(inputInfo);
		}

		return true;
	}

	void onItemIDArrow(CCObject* sender)
	{
		SetIDPopup::onItemIDArrow(sender);

		auto& idInputInfo = m_fields->m_id_input;
		if (!idInputInfo) return;

		auto parsedIdInputValue = geode::utils::numFromString<short>(idInputInfo.idInput->getString());
		if (parsedIdInputValue.isErr()) return;
		short idInputValue = parsedIdInputValue.unwrap();

		idInputInfo.namedIDInput->getInputNode()->onClickTrackNode(false);
		idInputInfo.namedIDInput->setString(
			NIDManager::getNameForID(idInputInfo.idType, idInputValue).unwrapOr("")
		);
	}

	void textChanged(CCTextInputNode* input)
	{
		SetIDPopup::textChanged(input);

		auto& idInputInfo = m_fields->m_id_input;
		if (!idInputInfo || !idInputInfo.idInput) return;

		if (auto parsedNum = numFromString<short>(idInputInfo.idInput->getString()); parsedNum.isOk())
			idInputInfo.namedIDInput->setString(
				NIDManager::getNameForID(idInputInfo.idType, parsedNum.unwrap()).unwrapOr("")
			);
	}


	static void onEditInput(NIDSetIDPopup* self, const std::string& str)
	{
		auto& idInputInfo = self->m_fields->m_id_input;

		if (auto name = NIDManager::getIDForName(idInputInfo.idType, str); name.isOk())
			idInputInfo.idInput->setString(fmt::format("{}", name.unwrap()));
	}

	void onEditIDNameButton(CCObject*)
	{
		auto& idInputInfo = m_fields->m_id_input;

		ShowEditNamedIDPopup(
			idInputInfo.idType,
			geode::utils::numFromString<short>(idInputInfo.idInput->getString()).unwrapOr(0),
			[&](short id) {
				idInputInfo.idInput->setString(fmt::format("{}", id));
			},
			[&] {
				this->textChanged(idInputInfo.idInput);
			}
		);
	}
};

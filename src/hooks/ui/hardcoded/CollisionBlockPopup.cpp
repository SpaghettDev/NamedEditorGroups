#include <Geode/modify/CollisionBlockPopup.hpp>

#include <NIDManager.hpp>

#include "../SetupPopups.hpp"

#include "../popups/EditNamedIDPopup.hpp"

#include "utils.hpp"

using namespace geode::prelude;

struct NIDCollisionBlockPopup : geode::Modify<NIDCollisionBlockPopup, CollisionBlockPopup>
{
	static constexpr std::uint16_t BLOCK_ID_PROPERTY = 0;

	// this popup is weird, it doesn't inherit SetupTriggerPopup???
	struct Fields
	{
		std::unordered_map<std::uint16_t, NIDSetupTriggerPopup::IDInputInfo> m_id_inputs;
	};

	bool init(EffectGameObject* p0, CCArray* p1)
	{
		if (!CollisionBlockPopup::init(p0, p1)) return false;

		std::vector<CCNode*> blockIDNodes;
		blockIDNodes.reserve(5);


		blockIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[](CCNode* c) {
				auto castedNode = typeinfo_cast<CCLabelBMFont*>(c);
				return castedNode && std::string_view(castedNode->getString()) == "Block ID";
			}
		));
		blockIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[labelPosX = blockIDNodes[0]->getPositionX()](CCNode* c) {
				auto castedNode = typeinfo_cast<CCTextInputNode*>(c);
				return castedNode && castedNode->getPositionX() == labelPosX;
			}
		));
		blockIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_mainLayer,
			[inputNodePos = blockIDNodes[1]->getPosition()](CCNode* c) {
				auto castedNode = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(c);
				return castedNode && (castedNode->getPosition() == inputNodePos);
			}
		));
		blockIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(blockIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 1 && c->getPositionY() == inputPosY;
			}
		));
		blockIDNodes.emplace_back(ng::utils::cocos::getChildByPredicate(
			this->m_buttonMenu,
			[
					inputPosY = this->m_buttonMenu->convertToNodeSpace(
						this->m_mainLayer->convertToWorldSpace(blockIDNodes[1]->getPosition())
					).y
			](CCNode* c) {
				return c->getTag() == 2 && c->getPositionY() == inputPosY;
			}
		));


		// thank you robert
		blockIDNodes[1]->setTag(BLOCK_ID_PROPERTY);


		auto blockIDInputInfo = NIDSetupTriggerPopup::commonInputSetup(
			this,
			NID::COLLISION,
			BLOCK_ID_PROPERTY,
			std::move(blockIDNodes),
			this->m_mainLayer,
			this->m_buttonMenu
		);
		blockIDInputInfo.namedIDInput->setCallback([&](const std::string& str) {
			NIDCollisionBlockPopup::onEditInput(this, BLOCK_ID_PROPERTY, str);
		});
		blockIDInputInfo.editInputButton->m_pfnSelector = menu_selector(NIDCollisionBlockPopup::onEditIDNameButton);
		m_fields->m_id_inputs[BLOCK_ID_PROPERTY] = std::move(blockIDInputInfo);

		return true;
	}

	void onItemIDArrow(CCObject* sender)
	{
		CollisionBlockPopup::onItemIDArrow(sender);

		auto inputNode = static_cast<CCTextInputNode*>(this->m_mainLayer->getChildByTag(BLOCK_ID_PROPERTY));

		auto parsedIdInputValue = geode::utils::numFromString<short>(inputNode->getString());
		if (parsedIdInputValue.isErr()) return;
		short idInputValue = parsedIdInputValue.unwrap();

		auto& idInputInfo = m_fields->m_id_inputs.at(BLOCK_ID_PROPERTY);

		idInputInfo.namedIDInput->getInputNode()->onClickTrackNode(false);
		idInputInfo.namedIDInput->setString(
			NIDManager::getNameForID<NID::COLLISION>(idInputValue).unwrapOr("")
		);
	}

	void textChanged(CCTextInputNode* input)
	{
		CollisionBlockPopup::textChanged(input);

		if (!m_fields->m_id_inputs.contains(input->getTag())) return;

		auto& idInputInfo = m_fields->m_id_inputs.at(input->getTag());

		if (auto parsedNum = numFromString<short>(idInputInfo.idInput->getString()); parsedNum.isOk())
			idInputInfo.namedIDInput->setString(
				NIDManager::getNameForID<NID::COLLISION>(parsedNum.unwrap()).unwrapOr("")
			);
	}


	static void onEditInput(NIDCollisionBlockPopup* self, std::uint8_t property, const std::string& str)
	{
		auto& idInputInfo = self->m_fields->m_id_inputs.at(property);

		if (auto name = NIDManager::getIDForName<NID::COLLISION>(str); name.isOk())
			idInputInfo.idInput->setString(fmt::format("{}", name.unwrap()));
	}

	void onEditIDNameButton(CCObject*)
	{
		auto& idInputInfo = m_fields->m_id_inputs.at(BLOCK_ID_PROPERTY);

		EditNamedIDPopup<NID::COLLISION>::create(
			geode::utils::numFromString<short>(idInputInfo.idInput->getString()).unwrapOr(0),
			[&](short id) {
				idInputInfo.idInput->setString(fmt::format("{}", id));
			},
			[&] {
				this->textChanged(idInputInfo.idInput);
			}
		)->show();
	}
};

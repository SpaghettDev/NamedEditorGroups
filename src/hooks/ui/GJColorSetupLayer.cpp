#include <Geode/modify/GJColorSetupLayer.hpp>

#include <NIDManager.hpp>

#include "utils.hpp"
#include "constants.hpp"

using namespace geode::prelude;

struct NIDGJColorSetupLayer : geode::Modify<NIDGJColorSetupLayer, GJColorSetupLayer>
{
	struct Fields
	{
		std::array<geode::TextInput*, 24> m_inputs;

		Fields()
		{
			m_inputs.fill(nullptr);
		}
	};

	bool init(LevelSettingsObject* p0)
	{
		if (!GJColorSetupLayer::init(p0)) return false;

		ng::utils::cocos::fixTouchPriority(this);

		auto& inputs = m_fields->m_inputs;

		for (auto colorLabel : CCArrayExt<CCLabelBMFont*>(this->m_colorLabels))
		{
			colorLabel->setPositionY(colorLabel->getPositionY() + 11.f);
			colorLabel->setScale(.225f);
		}

		for (std::size_t i = 0; auto colorSpr : CCArrayExt<ColorChannelSprite*>(this->m_colorSprites))
		{
			auto colorSprBtn = static_cast<CCMenuItemSpriteExtra*>(colorSpr->getParent());
			short id = colorSprBtn->getTag() + this->m_page * this->m_colorsPerPage;

			auto colorNameInput = geode::TextInput::create(60.f, fmt::format("Col {}", id));
			colorNameInput->setContentHeight(10.f);
			colorNameInput->setFilter(ng::constants::VALID_NAMED_ID_CHARACTERS);
			colorNameInput->setCallback([&, this, colorNameInput, id](const std::string& str) {
				NIDGJColorSetupLayer::onEditInput(this, colorNameInput, id, str);
			});
			colorNameInput->setPosition({ colorSprBtn->getPositionX(), colorSprBtn->getPositionY() + 15.f });
			colorNameInput->setID(fmt::format("color-name-input-{}"_spr, id));
			colorNameInput->getBGSprite()->setScaleX(.61f);
			colorNameInput->getBGSprite()->setScaleY(.325f);
			colorNameInput->setScale(.7f);
			colorNameInput->setTag(colorSprBtn->getTag() + 1000);
			m_buttonMenu->addChild(colorNameInput);
			inputs[i] = colorNameInput;

			i++;
		}

		this->updateSpriteColors();

		return true;
	}

	void updateSpriteColors()
	{
		GJColorSetupLayer::updateSpriteColors();

		// this method is called in original init()
		if (!m_fields->m_inputs[0]) return;

		for (auto input : m_fields->m_inputs)
		{
			short id = (input->getTag() - 1000) + this->m_page * this->m_colorsPerPage;
			auto name = NIDManager::getNameForID<NID::COLOR>(id);

			input->setPlaceholder(fmt::format("Col {}", id));
			input->setVisible(id < 1000);
			input->setString(
				name.unwrapOr(""),
				false
			);
			static_cast<CCLabelBMFont*>(
				this->m_colorLabels->objectAtIndex(input->getTag() - 1000 - 1)
			)->setVisible(name.isOk());
		}
	}


	static void onEditInput(NIDGJColorSetupLayer* self, geode::TextInput* input, short id, const std::string& str)
	{
		short btnID = id - self->m_page * self->m_colorsPerPage - 1;

		if (
			auto otherNamedID = NIDManager::getIDForName<NID::COLOR>(str);
			otherNamedID.isOkAnd([&](short otherID) { return otherID != id; })
		) {
			(void)NIDManager::removeNamedID<NID::COLOR>(id);
			static_cast<CCLabelBMFont*>(self->m_colorLabels->objectAtIndex(btnID))->setVisible(false);

			return ng::utils::cocos::createNotificationToast(
				self,
				fmt::format("{} {} already has this name!", ng::utils::getNamedIDIndentifier<NID::COLOR>(), otherNamedID.unwrap()),
				1.f, 45.f
			);
		}

		if (str.empty())
			(void)NIDManager::removeNamedID<NID::COLOR>(id);
		else if (auto res = NIDManager::saveNamedID<NID::COLOR>(std::string{ str }, id); res.isErr())
		{
			input->setString("");
			input->getInputNode()->onClickTrackNode(false);
			ng::utils::cocos::createNotificationToast(self, res.unwrapErr(), 1.f, 45.f);
		}

		static_cast<CCLabelBMFont*>(self->m_colorLabels->objectAtIndex(btnID))->setVisible(!str.empty());
	}
};

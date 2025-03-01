#include "utils.hpp"

#include <Geode/utils/cocos.hpp>

#include <Geode/binding/TextAlertPopup.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>

#include "constants.hpp"

geode::Result<> ng::utils::sanitizeName(const std::string_view name)
{
	if (name.length() > ng::constants::MAX_NAMED_ID_CHARACTERS)
		return geode::Err("Name is too long!");

	for (char c : name)
		if (ng::constants::VALID_NAMED_ID_CHARACTERS_VIEW.find(c) == std::string_view::npos)
			return geode::Err("Name contains invalid character '{}'", c);

	return geode::Ok();
}


void ng::utils::cocos::createNotificationToast(cocos2d::CCLayer* layer, const std::string& str, float time, float yPosition)
{
	auto tap = TextAlertPopup::create(str, time, .6f, 0x96, "bigFont.fnt");
	tap->setPositionY(yPosition);

	layer->addChild(tap, 420);
}

cocos2d::CCNode* ng::utils::cocos::getChildByPredicate(cocos2d::CCNode* parent, std::function<bool(cocos2d::CCNode*)>&& predicate)
{
	for (auto child : geode::cocos::CCArrayExt<cocos2d::CCNode*>(parent->getChildren()))
		if (child && predicate(child))
			return child;

	return nullptr;
}

void ng::utils::cocos::fixTouchPriority(cocos2d::CCTouchDelegate* delegate)
{
	cocos2d::CCTouchDispatcher::get()->addTargetedDelegate(
		delegate, cocos2d::kCCMenuHandlerPriority, true
	);
}


void ng::utils::editor::refreshObjectLabels(bool forced)
{
	if (const auto lel = LevelEditorLayer::get())
	{
		if (forced)
		{
			for (auto& obj : geode::cocos::CCArrayExt<GameObject*>(lel->m_objects))
				lel->updateObjectLabel(obj);

			return;
		}

		// thank you peter ida
		int count = (lel->m_sections.size() == 0) ? -1 : lel->m_sections.size();
		for (int i = lel->m_leftSectionIndex; i <= lel->m_rightSectionIndex && i < count; ++i) {
			auto leftSection = lel->m_sections[i];
			if (!leftSection) continue;

			auto leftSectionSize = leftSection->size();
			for (int j = lel->m_bottomSectionIndex; j <= lel->m_topSectionIndex && j < leftSectionSize; ++j) {
				auto section = leftSection->at(j);
				if (!section) continue;

				auto sectionSize = section->size();
				for (int k = 0; k < sectionSize; ++k) {
					auto obj = section->at(k);
					if (!obj) continue;

					LevelEditorLayer::updateObjectLabel(obj);
				}
			}
		}
	}
}

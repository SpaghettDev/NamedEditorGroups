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


void ng::utils::editor::refreshObjectLabels()
{
	if (const auto lel = LevelEditorLayer::get())
	{
		for (auto trigger : lel->m_unk3278)
			lel->updateObjectLabel(trigger);
	}
}

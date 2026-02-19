#include "utils.hpp"

#include <ranges>
#include <cctype>

#include <Geode/utils/cocos.hpp>

#include <Geode/binding/TextAlertPopup.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>

// #include <alphalaneous.improved_group_view/api/GroupViewUpdateEvent.hpp>

#include "../hooks/LevelEditorLayerData.hpp"

// #include "globals.hpp"
#include "constants.hpp"

geode::Result<> ng::utils::sanitizeName(const std::string_view name)
{
	if (name.size() > ng::constants::MAX_NAMED_ID_LENGTH)
		return geode::Err("Name is too long!");

	if (auto invalidCharIdx = name.find_first_not_of(ng::constants::VALID_NAMED_ID_CHARACTERS_VIEW); invalidCharIdx != std::string_view::npos)
		return geode::Err("Name contains invalid character '{}'", name[invalidCharIdx]);

	return geode::Ok();
}

bool ng::utils::startsWithNumbers(const std::string_view str)
{
	if (str.empty()) return false;

	for (const char& c : str)
		if (!std::isdigit(c))
			return &c != &str.front();

	return true;
}

bool ng::utils::endsWithNumbers(const std::string_view str)
{
	if (str.empty()) return false;

	for (const char& c : std::ranges::views::reverse(str))
		if (!std::isdigit(c))
			return &c != &str.back();

	return true;
}

geode::Result<int> ng::utils::numberFromStart(const std::string_view str)
{
	std::size_t pos = 0;
	while (pos < str.size() && std::isdigit(str[pos]))
		pos++;

	return geode::utils::numFromString<int>(str.substr(0, pos));
}

geode::Result<int> ng::utils::numberFromEnd(const std::string_view str)
{
	std::size_t pos = str.size() - 1;
	while (pos > 0 && std::isdigit(str[pos - 1]))
		pos--;

	return geode::utils::numFromString<int>(str.substr(pos));
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


void ng::utils::editor::refreshObjectLabels()
{
	if (const auto lel = LevelEditorLayer::get())
	{
		for (auto trigger : lel->m_activeObjects)
		{
			// trigger can be nullptr for some fucking reason
			// 1816 is player object, which has a hidden object label
			if (!trigger || trigger->m_objectID == 1816u) continue;
			lel->updateObjectLabel(trigger);
		}
	}
}

void ng::utils::editor::postIGVUpdateEvent()
{
	// if (!ng::globals::g_isImprovedGroupViewLoaded)
		return;

	// igv::GroupViewUpdateEvent().post();
}

void ng::utils::editor::save()
{
	if (auto lel = LevelEditorLayer::get())
	{
		// little dirty hack :p
		static char eplBuff[sizeof(NIDEditorPauseLayerSave)];
		static auto epl = new (eplBuff) NIDEditorPauseLayerSave();
		epl->m_editorLayer = lel;
	
		epl->saveLevel();
	}
}

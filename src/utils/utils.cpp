#include "utils.hpp"

#include <ranges>
#include <cctype>

#include <Geode/utils/cocos.hpp>

#include <Geode/binding/TextAlertPopup.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>

#include "constants.hpp"

geode::Result<std::pair<std::string, std::string>> ng::utils::parseNamedIDString(
	const std::string_view format,
	const std::string_view str
) {
	std::size_t namePos = format.find("{name}");
	std::size_t idPos = format.find("{id}");
	
	if (namePos == std::string::npos || idPos == std::string::npos)
		return geode::Err("Format doesn't contain required formatters {name} or {id}!");

	std::string separator;
	if (namePos < idPos)
		separator = format.substr(namePos + 6, idPos - (namePos + 6));
	else
		separator = format.substr((idPos + 3) + 1, namePos - ((idPos + 3) + 1));

	if (separator.empty())
	{
		if (idPos == 0)
		{
			std::string name;
			name = str.substr(str.find_first_not_of("0123465789"));

			return geode::Ok(std::pair{
				name,
				fmt::format("{}", ng::utils::numberFromStart(str).unwrap())
			});
		}
		else if (idPos == format.length() - 4)
		{
			std::string name;
			name = str.substr(0, str.find_first_of("0123465789"));

			return geode::Ok(std::pair{
				name,
				fmt::format("{}", ng::utils::numberFromEnd(str).unwrap())
			});
		}

		return geode::Err("Couldn't extract name or ID from string!");
	}

	std::size_t sepPos = str.find(separator);
	if (sepPos == std::string::npos)
		return geode::Err("String does not match format!");

	if (namePos < idPos)
		return geode::Ok(std::pair{
			str.substr(0, sepPos), str.substr(sepPos + separator.length()) }
		);
	else
		return geode::Ok(std::pair{
			str.substr(sepPos + separator.length()), str.substr(0, sepPos) }
		);
}

geode::Result<> ng::utils::sanitizeName(const std::string_view name)
{
	if (name.length() > ng::constants::MAX_NAMED_ID_LENGTH)
		return geode::Err("Name is too long!");

	for (char c : name)
		if (ng::constants::VALID_NAMED_ID_CHARACTERS_VIEW.find(c) == std::string_view::npos)
			return geode::Err("Name contains invalid character '{}'", c);

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
		{
			// ???
			if (!trigger) continue;
			lel->updateObjectLabel(trigger);
		}
	}
}

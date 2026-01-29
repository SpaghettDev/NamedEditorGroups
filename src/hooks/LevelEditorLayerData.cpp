#include "LevelEditorLayerData.hpp"

#include <Geode/utils/general.hpp>
#include <Geode/utils/base64.hpp>

#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditorUI.hpp>

#include <Geode/binding/GameObject.hpp>
#include <Geode/binding/TextGameObject.hpp>

#include <NIDManager.hpp>
#include <NIDExtrasManager.hpp>

#include <cvolton.level-id-api/include/EditorIDs.hpp>

#include "globals.hpp"
#include "constants.hpp"
#include "benchmark.hpp"

using namespace geode::prelude;

// get the save data very early
void NIDLevelEditorLayerData::createObjectsFromSetup(gd::string& levelString)
{
#ifdef GEODE_IS_ANDROID
	auto levelStr = std::string{ levelString };
#else
	auto& levelStr = levelString;
#endif

	if (ng::globals::g_isEditorIDAPILoaded)
	{
		int id;
		EditorIDs::Event::GetLevelID2("cvolton.level-id-api/v1/get-level-id-2", &id, this->m_level, true).post();
		NIDExtrasManager::init(id);
	}

	NIDManager::reset();

	if (std::strstr(levelStr.c_str(), ng::constants::old::SAVE_OBJECT_STRING_START) != nullptr)
		updateSaveObject(levelString);

	{
		NID_PROFILER("Parse string");

		m_fields->m_parse_result = parseDataString(levelStr);
	}

	LevelEditorLayer::createObjectsFromSetup(levelString);
}


geode::Result<void, std::pair<std::string, std::string>> NIDLevelEditorLayerData::parseDataString(
#ifdef GEODE_IS_ANDROID
	const std::string& str
#else
	const gd::string& str
#endif
) {
	std::string_view lvlStr{ str };

	// const std::size_t saveObjStrOffset = lvlStr.find(ng::constants::SAVE_OBJECT_STRING_START);
	const auto saveObjStrOffset = std::strstr(lvlStr.data(), ng::constants::SAVE_OBJECT_STRING_START) - lvlStr.data();
	if (saveObjStrOffset <= 0)
		return geode::Ok();

	// lvlStr.remove_prefix(saveObjStrOffset + ng::constants::SAVE_OBJECT_STRING_START.size());
	lvlStr.remove_prefix(saveObjStrOffset + ng::constants::SAVE_OBJECT_STRING_START_VIEW.size());

	// const auto separatorOffset = lvlStr.find(ng::constants::TEXT_OBJECT_STRING_SEPARATOR);
	const auto separatorOffset = std::strstr(lvlStr.data(), ng::constants::TEXT_OBJECT_STRING_SEPARATOR) - lvlStr.data();
	if (separatorOffset <= 0)
		return geode::Ok();

	// lvlStr.remove_prefix(separatorOffset + ng::constants::TEXT_OBJECT_STRING_SEPARATOR.size());
	lvlStr.remove_prefix(separatorOffset + ng::constants::TEXT_OBJECT_STRING_SEPARATOR_VIEW.size());

	std::string_view saveObjStr = lvlStr.substr(0, lvlStr.find(';'));

	if (auto data = geode::utils::base64::decodeString(saveObjStr, geode::utils::base64::Base64Variant::UrlWithPad))
	{
		if (auto importRes = NIDManager::importNamedIDs(data.unwrap()); importRes.isErr())
		{
			NIDManager::reset();

			return geode::Err(std::pair{
				fmt::format("<cr>{}</c>", importRes.unwrapErr()),
				std::string{ saveObjStr }
			});
		}
	}
	else
	{
		NIDManager::reset();

		return geode::Err(std::pair{
			fmt::format("Unable to decode base64 in NamedIDS: <cr>{}</c>", data.unwrapErr()),
			std::string{ saveObjStr }
		});
	}

	return geode::Ok();
}

void NIDLevelEditorLayerData::updateSaveObject(gd::string& levelString)
{
#ifdef GEODE_IS_ANDROID
	auto lvlStr = std::string{ levelString };

	lvlStr.replace(
		lvlStr.find(ng::constants::old::SAVE_OBJECT_STRING_START_VIEW),
		ng::constants::old::SAVE_OBJECT_STRING_START_VIEW.size(),
		ng::constants::SAVE_OBJECT_STRING_START
	);
#else
	levelString.replace(
		levelString.find(ng::constants::old::SAVE_OBJECT_STRING_START_VIEW),
		ng::constants::old::SAVE_OBJECT_STRING_START_VIEW.size(),
		ng::constants::SAVE_OBJECT_STRING_START
	);
#endif

#ifdef GEODE_IS_ANDROID
	levelString = lvlStr;
#endif
}

TextGameObject* NIDLevelEditorLayerData::getSaveObject()
{
	short currentLayer = this->m_currentLayer;
	this->m_currentLayer = -1;
	auto object = static_cast<TextGameObject*>(
		this->objectAtPosition(ng::constants::SAVE_DATA_OBJECT_POS)
	);
	this->m_currentLayer = currentLayer;

	if (m_fields->m_parse_result.isErr()) [[unlikely]]
	{
		this->removeObject(object, true);
		object = nullptr;

		if (!s_shouldDeleteSaveObject)
		{
			s_shouldDeleteSaveObject = false;

			const auto& [err, saveObjStr] = m_fields->m_parse_result.unwrapErr();
	
			auto errorPopup = FLAlertLayer::create(
				nullptr,
				"Error parsing save object",
				fmt::format(
					"{}\n"
					"Save string has been copied to clipboard "
					"and the save object has been deleted.",
					err
				),
				"OK",
				nullptr,
				350.f
			);
			errorPopup->m_scene = this;
			errorPopup->show();
	
			geode::utils::clipboard::write(saveObjStr);
		}

		m_fields->m_parse_result = geode::Ok();
	}

	return object;
}

void NIDLevelEditorLayerData::createSaveObject()
{
	if (getSaveObject()) return;

	short currentLayer = this->m_currentLayer;
	this->m_currentLayer = -1;
	auto saveObject = static_cast<TextGameObject*>(
		this->createObject(914, { .0f, .0f }, true)
	);
	this->m_currentLayer = currentLayer;
	saveObject->updateTextObject("", false);
	this->removeObjectFromSection(saveObject);
	saveObject->setPosition(ng::constants::SAVE_DATA_OBJECT_POS);
	saveObject->setScale(.01f);
	this->addToSection(saveObject);
}


void NIDEditorPauseLayerSave::saveLevel()
{
	if (NIDManager::isDirty())
	{
		auto lel = static_cast<NIDLevelEditorLayerData*>(LevelEditorLayer::get());
		auto saveObject = lel->getSaveObject();

		if (!saveObject)
		{
			lel->createSaveObject();
			saveObject = lel->getSaveObject();
		}

		saveObject->m_text = NIDManager::dumpNamedIDs();
	}

	if (NIDExtrasManager::isDirty())
		NIDExtrasManager::save();

	EditorPauseLayer::saveLevel();
}

void NIDEditorPauseLayerSave::onExitEditor(CCObject* sender)
{
	NIDManager::reset();
	NIDExtrasManager::reset();

	EditorPauseLayer::onExitEditor(sender);
}

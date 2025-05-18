#include "LevelEditorLayerData.hpp"

#include <Geode/utils/general.hpp>

#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditorUI.hpp>

#include <Geode/binding/GameObject.hpp>
#include <Geode/binding/TextGameObject.hpp>

#include <NIDManager.hpp>
#include <NIDExtrasManager.hpp>

#include <cvolton.level-id-api/include/EditorIDs.hpp>

#include "base64.hpp"
#include "globals.hpp"
#include "constants.hpp"

using namespace geode::prelude;

// get the save data very early
void LevelEditorLayerData::createObjectsFromSetup(gd::string& levelString)
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

	if (levelStr.find(ng::constants::old::SAVE_OBJECT_STRING_START) != std::string_view::npos)
		updateSaveObject(levelString);

	m_fields->m_parse_result = parseDataString(levelStr);

	LevelEditorLayer::createObjectsFromSetup(levelString);
}


#ifdef GEODE_IS_ANDROID
geode::Result<void, std::pair<std::string, std::string>> LevelEditorLayerData::parseDataString(const std::string& str)
#else
geode::Result<void, std::pair<std::string, std::string>> LevelEditorLayerData::parseDataString(const gd::string& str)
#endif
{
	std::string_view lvlStr = str;

	if (
		std::size_t saveObjStrOffset = lvlStr.find(ng::constants::SAVE_OBJECT_STRING_START);
		saveObjStrOffset != std::string_view::npos
	) {
		std::string_view intermediateStr = lvlStr.substr(
			saveObjStrOffset + ng::constants::SAVE_OBJECT_STRING_START.size()
		);
		intermediateStr = intermediateStr.substr(
			intermediateStr.find(ng::constants::TEXT_OBJECT_STRING_SEPARATOR) +
			ng::constants::TEXT_OBJECT_STRING_SEPARATOR.size()
		);
		std::string_view saveObjStr = intermediateStr.substr(0, intermediateStr.find(';'));

		if (auto data = ng::base64::base64URLDecode(saveObjStr))
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
	}

	return geode::Ok();
}

void LevelEditorLayerData::updateSaveObject(gd::string& levelString)
{
#ifdef GEODE_IS_ANDROID
	auto lvlStr = std::string{ levelString };

	lvlStr.replace(
		lvlStr.find(ng::constants::old::SAVE_OBJECT_STRING_START),
		ng::constants::old::SAVE_OBJECT_STRING_START.size(),
		ng::constants::SAVE_OBJECT_STRING_START
	);
#else
	levelString.replace(
		levelString.find(ng::constants::old::SAVE_OBJECT_STRING_START),
		ng::constants::old::SAVE_OBJECT_STRING_START.size(),
		ng::constants::SAVE_OBJECT_STRING_START
	);
#endif

#ifdef GEODE_IS_ANDROID
	levelString = lvlStr;
#endif
}

TextGameObject* LevelEditorLayerData::getSaveObject()
{
	short currentLayer = this->m_currentLayer;
	this->m_currentLayer = -1;
	auto object = static_cast<TextGameObject*>(this->objectAtPosition(ng::constants::SAVE_DATA_OBJECT_POS));
	this->m_currentLayer = currentLayer;

	if (m_fields->m_parse_result.isErr())
	{
		this->removeObject(object, true);
		object = nullptr;

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

		m_fields->m_parse_result = geode::Ok();
	}

	return object;
}

void LevelEditorLayerData::createSaveObject()
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


void EditorPauseLayerSave::saveLevel()
{
	if (NIDManager::isDirty())
	{
		auto lel = static_cast<LevelEditorLayerData*>(LevelEditorLayer::get());
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

void EditorPauseLayerSave::onExitEditor(CCObject* sender)
{
	NIDManager::reset();
	NIDExtrasManager::reset();

	EditorPauseLayer::onExitEditor(sender);
}

#include "LevelEditorLayerData.hpp"

#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditorUI.hpp>

#include <Geode/binding/GameObject.hpp>
#include <Geode/binding/TextGameObject.hpp>

#include <NIDManager.hpp>

#include "../base64/base64.hpp"
#include "constants.hpp"

using namespace geode::prelude;

// get the save data very early
void LevelEditorLayerData::createObjectsFromSetup(gd::string& levelString)
{
	std::string_view lvlStr = levelString;

	if (
		std::size_t saveObjStrOffset = lvlStr.find(ng::constants::SAVE_OBJECT_STRING_START);
		saveObjStrOffset != std::string_view::npos
	) {
		std::string_view intermediateStr = lvlStr.substr(
			saveObjStrOffset + ng::constants::SAVE_OBJECT_STRING_START.length()
		);
		intermediateStr = intermediateStr.substr(
			intermediateStr.find(ng::constants::TEXT_OBJECT_STRING_SEPARATOR) +
			ng::constants::TEXT_OBJECT_STRING_SEPARATOR.length()
		);
		std::string_view saveObjStr = intermediateStr.substr(0, intermediateStr.find(';'));

		// TODO: maybe do something here idk
		if (auto data = ng::base64::base64URLDecode(saveObjStr))
			NIDManager::importNamedIDs(data.unwrap());
		else
			log::warn("Unable to import Named IDs! Error: {}", data.unwrapErr());
	}

	LevelEditorLayer::createObjectsFromSetup(levelString);
}

TextGameObject* LevelEditorLayerData::getSaveObject()
{
	short currentLayer = this->m_currentLayer;
	this->m_currentLayer = -1;
	auto object = static_cast<TextGameObject*>(this->objectAtPosition(ng::constants::SAVE_DATA_OBJECT_POS));
	this->m_currentLayer = currentLayer;

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
	saveObject->setScale(.0f);
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

	EditorPauseLayer::saveLevel();
}

void EditorPauseLayerSave::onExitEditor(CCObject* sender)
{
	NIDManager::reset();

	EditorPauseLayer::onExitEditor(sender);
}

#include "LevelEditorLayerData.hpp"

#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditorUI.hpp>

#include <Geode/binding/GameObject.hpp>
#include <Geode/binding/TextGameObject.hpp>

#include <NIDManager.hpp>

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

		auto data = cocos2d::ZipUtils::base64URLDecode(gd::string{ saveObjStr.data() });
		NIDManager::importNamedIDs(data);
	}

	LevelEditorLayer::createObjectsFromSetup(levelString);
}

TextGameObject* LevelEditorLayerData::getSaveObject()
{
	return static_cast<TextGameObject*>(this->objectAtPosition(ng::constants::SAVE_DATA_OBJECT_POS));
}

void LevelEditorLayerData::createSaveObject()
{
	if (getSaveObject()) return;

	auto saveObject = static_cast<TextGameObject*>(
		this->createObject(914, { .0f, .0f }, true)
	);
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

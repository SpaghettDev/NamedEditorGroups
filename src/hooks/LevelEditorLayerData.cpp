#include "LevelEditorLayerData.hpp"

#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditorUI.hpp>

#include <Geode/binding/GameObject.hpp>
#include <Geode/binding/TextGameObject.hpp>

#include <NIDManager.hpp>

#include "utils.hpp"
#include "constants.hpp"

using namespace geode::prelude;

bool LevelEditorLayerData::init(GJGameLevel* p0, bool p1)
{
	if (!LevelEditorLayer::init(p0, p1)) return false;

	auto eui = EditorUI::get();

	if (auto saveObject = getSaveObject())
	{
		NIDManager::importNamedIDs(saveObject->m_text);

		// force refresh after loading the group names
		ng::utils::editor::refreshObjectLabels();
	}
	else
	{
		saveObject = static_cast<TextGameObject*>(
			this->createObject(914, { .0f, .0f }, true)
		);
		saveObject->updateTextObject("", false);
		this->removeObjectFromSection(saveObject);
		saveObject->setPosition(ng::constants::SAVE_DATA_OBJECT_POS);
		saveObject->setScale(.0f);
		this->addToSection(saveObject);
	}

	return true;
}

TextGameObject* LevelEditorLayerData::getSaveObject()
{
	return static_cast<TextGameObject*>(this->objectAtPosition(ng::constants::SAVE_DATA_OBJECT_POS));
}


void EditorPauseLayerSave::saveLevel()
{
	if (auto saveObject = static_cast<LevelEditorLayerData*>(LevelEditorLayer::get())->getSaveObject())
		saveObject->m_text = NIDManager::dumpNamedIDs();

	EditorPauseLayer::saveLevel();
}

void EditorPauseLayerSave::onExitEditor(CCObject* sender)
{
	NIDManager::reset();

	EditorPauseLayer::onExitEditor(sender);
}

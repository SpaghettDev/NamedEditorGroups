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
		geode::log::debug("loaded saveObject: {}", saveObject->m_text);
		NIDManager::importNamedIDs(saveObject->m_text);

		// force refresh after loading the group names
		ng::utils::editor::refreshObjectLabels();
	}
	else
	{
		eui->m_clickAtPosition = cocos2d::CCPoint{ .0f, .0f };
		eui->onCreateObject(914);
		static_cast<TextGameObject*>(eui->m_selectedObject)->updateTextObject("", false);
		this->removeObjectFromSection(eui->m_selectedObject);
		eui->m_selectedObject->setPosition(ng::constants::SAVE_DATA_OBJECT_POS);
		eui->m_selectedObject->setScale(.0f);
		this->addToSection(eui->m_selectedObject);
		eui->deselectObject(eui->m_selectedObject);
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
	{
		saveObject->updateTextObject(NIDManager::dumpNamedIDs(), false);
		geode::log::debug("text is now {}", saveObject->m_text);
	}

	EditorPauseLayer::saveLevel();
}

void EditorPauseLayerSave::onExitEditor(CCObject* sender)
{
	NIDManager::reset();

	EditorPauseLayer::onExitEditor(sender);
}

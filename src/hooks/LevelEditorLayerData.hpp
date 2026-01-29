#pragma once

#include <string>

#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

#include <Geode/binding/TextGameObject.hpp>

#include <Geode/Result.hpp>

struct NIDLevelEditorLayerData : geode::Modify<NIDLevelEditorLayerData, LevelEditorLayer>
{
	inline static bool s_shouldDeleteSaveObject = false;

	struct Fields
	{
		geode::Result<void, std::pair<std::string, std::string>> m_parse_result = geode::Ok();
	};

	void createObjectsFromSetup(gd::string&);


#ifdef GEODE_IS_ANDROID
	static geode::Result<void, std::pair<std::string, std::string>> parseDataString(const std::string&);
#else
	static geode::Result<void, std::pair<std::string, std::string>> parseDataString(const gd::string&);
#endif

	// transition
	static void updateSaveObject(gd::string&);

	TextGameObject* getSaveObject();
	void createSaveObject();
};

struct NIDEditorPauseLayerSave : geode::Modify<NIDEditorPauseLayerSave, EditorPauseLayer>
{
	void saveLevel();
	void onExitEditor(CCObject*);
};

#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

#include <Geode/binding/TextGameObject.hpp>

using namespace geode::prelude;

struct LevelEditorLayerData : geode::Modify<LevelEditorLayerData, LevelEditorLayer>
{
	struct Fields
	{
		bool m_had_error;
	};

	void createObjectsFromSetup(gd::string&);


	// transition
	void updateSaveObject(gd::string&);

	TextGameObject* getSaveObject();
	void createSaveObject();
};

struct EditorPauseLayerSave : geode::Modify<EditorPauseLayerSave, EditorPauseLayer>
{
	void saveLevel();
	void onExitEditor(CCObject*);
};

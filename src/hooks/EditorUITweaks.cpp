#include <Geode/modify/EditorUI.hpp>

#include <NIDManager.hpp>

#include "GameObjectData.hpp"

#include "utils.hpp"
#include "constants.hpp"

using namespace geode::prelude;

auto objInArray = [](GameObject* object, const auto& container) {
	return ng::utils::getIndexOf(container, object->m_objectID) != -1;
};

struct NIDEditorUITweaks : geode::Modify<NIDEditorUITweaks, EditorUI>
{
	void dynamicGroupUpdate(bool isRegroup)
	{
		const std::string nameFormatRaw = getNameFormatRaw();
		const std::string nameFormat = getNameFormat();
		std::vector<ng::types::GameObjectData> origObjects;
		origObjects.reserve(this->m_selectedObjects->count());
		auto selectedObjects = CCArray::create();

		if (this->m_selectedObject)
			origObjects.push_back(this->m_selectedObject);
		else
			for (auto obj : CCArrayExt<GameObject*>(this->m_selectedObjects))
				origObjects.push_back(obj);

		EditorUI::dynamicGroupUpdate(isRegroup);

		if (!this->m_selectedObject && !this->m_selectedObjects->count()) return;

		if (this->m_selectedObject)
			selectedObjects->addObject(this->m_selectedObject);
		else
			selectedObjects->addObjectsFromArray(this->m_selectedObjects);

		for (auto idx = 0; auto object : origObjects)
		{
			auto newObj = static_cast<GameObject*>(selectedObjects->objectAtIndex(idx));

			if (object.m_groupCount > 0)
			{
				for (auto idx = 0; short id : object.m_groups)
				{
					short oldID = object.m_groups.at(idx);
					short newID = newObj->m_groups->at(idx);

					if (newID != oldID)
						if (auto newName = autoNameObjectID(NID::GROUP, oldID, nameFormatRaw, nameFormat))
							static_cast<void>(NIDManager::saveNamedID<NID::GROUP>(std::move(newName.unwrap()), newID));

					idx++;
				}
			}

			if (object.m_classType == GameObjectClassType::Effect)
			{
				auto newEffectObj = static_cast<EffectGameObject*>(newObj);

				for (const auto& [nid, getters] : ng::constants::EFFECTGAMEOBJECT_TO_ID_TYPE.at(newEffectObj->m_objectID))
				{
					NID realNID = nid;
					if (realNID == NID::_UNKNOWN) break;

					const auto& [dataGetter, objGetter] = getters;
					if (objGetter(newEffectObj) == dataGetter(object)) continue;

					// Edit Area triggers ID can either be Group ID or Effect ID
					// getTriggerValue doesn't use any members in SetupTriggerPopup
					if (
						newEffectObj->m_objectID >= 3011 && newEffectObj->m_objectID <= 3015 &&
						static_cast<SetupTriggerPopup*>(nullptr)->getTriggerValue(355, newEffectObj) != .0f
					)
						realNID = NID::EFFECT;

					if (realNID == NID::DYNAMIC_COUNTER_TIMER)
					{
						auto& toggleMap = ng::constants::DYNAMIC_PROPERTIES_TOGGLES.at(newEffectObj->m_objectID);

						for (const auto& [property, toggleInfo] : toggleMap)
						{
							auto propVal = static_cast<SetupTriggerPopup*>(nullptr)->getTriggerValue(
								toggleInfo.togglePropID,
								newEffectObj
							);
	
							if (propVal == toggleInfo.counterState)
							{
								realNID = NID::COUNTER;
								break;
							}
							else
								realNID = NID::TIMER;
						}
					}

					if (auto newName = autoNameObjectID(realNID, dataGetter(object), nameFormatRaw, nameFormat))
						static_cast<void>(NIDManager::saveNamedID(realNID, std::move(newName.unwrap()), objGetter(newEffectObj)));
				}
			}

			idx++;
		}
		
		ng::utils::editor::refreshObjectLabels();
	}


	std::string getNameFormatRaw()
	{
		return Mod::get()->getSettingValue<std::string>("auto-name-format");
	}

	std::string getNameFormat()
	{
		std::string nameFormatRaw = Mod::get()->getSettingValue<std::string>("auto-name-format");

		std::string nameFormat;
		nameFormat = nameFormatRaw.replace(nameFormatRaw.find("{name}"), 6, "{}");
		nameFormat = nameFormat.replace(nameFormat.find("{id}"), 4, "{}");

		return nameFormat;
	}

	geode::Result<std::string> autoNameObjectID(NID nid, short id, const std::string& nameFormatRaw, const std::string& nameFormat)
	{
		auto nameRes = NIDManager::getNameForID(nid, id);
		if (nameRes.isErr()) return geode::Err("");
		auto name = nameRes.unwrap();

		std::string newName{};

		// id is last
		if (nameFormatRaw.find("{id}") == nameFormatRaw.length() - 4 && ng::utils::endsWithNumbers(name))
		{
			auto numRes = ng::utils::numberFromEnd(name);
			if (numRes.isErr()) return geode::Err("");
			auto num = numRes.unwrap();

			auto parsedRes = ng::utils::parseNamedIDString(nameFormatRaw, name);
			if (parsedRes.isErr()) return geode::Err("");
			auto [rawName, idStr] = parsedRes.unwrap();

			while (NIDManager::getIDForName(nid, fmt::format(fmt::runtime(nameFormat), rawName, num)).isOk())
				num++;

			newName = fmt::format(fmt::runtime(nameFormat), rawName, num);
		}
		// id is first
		else if (nameFormatRaw.find("{id}") == 0 && ng::utils::startsWithNumbers(name))
		{
			auto numRes = ng::utils::numberFromStart(name);
			if (numRes.isErr()) return geode::Err("");
			auto num = numRes.unwrap();

			auto parsedRes = ng::utils::parseNamedIDString(nameFormatRaw, name);
			if (parsedRes.isErr()) {log::debug("ERR {}", parsedRes.unwrapErr()); return geode::Err("");}
			auto [rawName, idStr] = parsedRes.unwrap();

			while (NIDManager::getIDForName(nid, fmt::format(fmt::runtime(nameFormat), num, rawName)).isOk())
				num++;

			newName = fmt::format(fmt::runtime(nameFormat), num, rawName);
		}
		else if (name.length() <= ng::constants::MAX_NAMED_ID_LENGTH - 2)
		{
			if (nameFormatRaw.find("{id}") == 0)
				newName = fmt::format(fmt::runtime(nameFormat), 2, name);
			else
				newName = fmt::format(fmt::runtime(nameFormat), name, 2);
		}

		if (newName.length() > ng::constants::MAX_NAMED_ID_LENGTH)
			return geode::Err("");

		return geode::Ok(newName);
	}
};

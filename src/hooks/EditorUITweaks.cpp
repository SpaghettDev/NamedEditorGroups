#include <Geode/modify/EditorUI.hpp>

#include <NIDManager.hpp>

#include "IDFormatParser.hpp"

#include "GameObjectData.hpp"

#include "utils.hpp"
#include "globals.hpp"
#include "constants.hpp"

using namespace geode::prelude;

struct NIDEditorUITweaks : geode::Modify<NIDEditorUITweaks, EditorUI>
{
	void dynamicGroupUpdate(bool isRegroup)
	{
		std::vector<short> errorredIDs;

		std::vector<ng::types::GameObjectData> origObjects;
		origObjects.reserve(this->m_selectedObjects->count());
		auto selectedObjects = CCArray::create();

		if (this->m_selectedObject)
			origObjects.emplace_back(this->m_selectedObject);
		else
			for (auto obj : CCArrayExt<GameObject*>(this->m_selectedObjects))
				origObjects.emplace_back(obj);

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
					{
						if (auto newName = autoNameObjectID(NID::GROUP, oldID))
							(void)NIDManager::saveNamedID<NID::GROUP>(std::move(newName.unwrap()), newID);
						else if (!newName.unwrapErr().empty())
							errorredIDs.push_back(newID);
					}

					idx++;
				}
			}

			if (object.m_classType == GameObjectClassType::Effect)
			{
				auto newEffectObj = static_cast<EffectGameObject*>(newObj);

				// idk
				if (!newEffectObj) continue;

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

					if (auto newName = autoNameObjectID(realNID, dataGetter(object)))
						(void)NIDManager::saveNamedID(realNID, std::move(newName.unwrap()), objGetter(newEffectObj));
					else if (!newName.unwrapErr().empty())
						errorredIDs.push_back(objGetter(newEffectObj));
				}

				this->m_editorLayer->updateObjectLabel(newEffectObj);
			}

			idx++;
		}

		if (!errorredIDs.empty())
			ng::utils::cocos::createNotificationToast(
				this,
				fmt::format("Couldn't auto-name IDs: {}", fmt::join(errorredIDs, ", ")),
				1.f, 110.f
			);
	}


	geode::Result<std::string> autoNameObjectID(NID nid, short id)
	{
		auto nameRes = NIDManager::getNameForID(nid, id);
		if (nameRes.isErr()) return geode::Err("");
		auto name = nameRes.unwrap();

		auto tokensRes = ng::parser::parseFormat(ng::globals::g_buildHelperRawNameFormat);
		if (tokensRes.isErr()) return geode::Err(tokensRes.unwrapErr());
		auto& tokens = tokensRes.unwrap();

		auto fmtPairRes = ng::parser::extract(name, tokens);
		if (fmtPairRes.isErr())
		{
			fmtPairRes = ng::parser::extract(
				ng::parser::format(tokens, { name, "1" }).unwrap(),
				tokens
			);
		}
		auto& fmtPair = fmtPairRes.unwrap();

		auto numRes = geode::utils::numFromString<int>(fmtPair.id);
		if (numRes.isErr()) return geode::Err("Invalid number in name");
		fmtPair.id = fmt::format("{}", numRes.unwrap() + 1);

		auto fmtRes = ng::parser::format(tokens, fmtPair);
		if (fmtRes.isErr()) return geode::Err(fmtRes.unwrapErr());
		auto& fmt = fmtRes.unwrap();

		if (fmt.size() > ng::constants::MAX_NAMED_ID_LENGTH)
			return geode::Err("Auto-named ID is too long ({})", fmt.size());

		return geode::Ok(fmt);
	}
};

#include <Geode/modify/EffectGameObject.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

#include <NIDManager.hpp>
#include <NIDExtrasManager.hpp>

#include "utils.hpp"
#include "constants.hpp"
#include "globals.hpp"

using namespace geode::prelude;

auto objInArray = [](GameObject* object, const auto& container) {
	return ng::utils::getIndexOf(container, object->m_objectID) != -1;
};

struct NIDEffectGameObject : geode::Modify<NIDEffectGameObject, EffectGameObject>
{
	struct Fields
	{
		Ref<CCLabelBMFont> m_id_name_label = CCLabelBMFont::create("", "bigFont.fnt");
		bool m_has_id_name_label = false;
	};

	void customSetup()
	{
		EffectGameObject::customSetup();

		bool isTrigger = objInArray(this, ng::constants::TRIGGER_OBJECT_IDS_WITH_LABEL);
		bool isCollision = objInArray(this, ng::constants::COLLISION_OBJECT_IDS_WITH_LABEL);
		bool isCounter = objInArray(this, ng::constants::COUNTER_OBJECT_IDS_WITH_LABEL);
		bool isTimer = objInArray(this, ng::constants::TIMER_OBJECT_IDS_WITH_LABEL);

		if (!(isTrigger || isCollision || isCounter || isTimer))
			return;

		// lol why does the game not do this
		this->setCascadeOpacityEnabled(true);

		// 28.5f is content width of move trigger, which works well for all other triggers
		m_fields->m_id_name_label->limitLabelWidth(28.5f + 10.f, .6f, .1f);
		// can't add the label here
	}
};

struct NIDLevelEditorLayer : geode::Modify<NIDLevelEditorLayer, LevelEditorLayer>
{
	static void updateObjectLabel(GameObject* object)
	{
		LevelEditorLayer::updateObjectLabel(object);

		bool isTrigger = objInArray(object, ng::constants::TRIGGER_OBJECT_IDS_WITH_LABEL);
		bool isCollision = objInArray(object, ng::constants::COLLISION_OBJECT_IDS_WITH_LABEL);
		bool isCounter = objInArray(object, ng::constants::COUNTER_OBJECT_IDS_WITH_LABEL);
		bool isTimer = objInArray(object, ng::constants::TIMER_OBJECT_IDS_WITH_LABEL);

		if (!(isTrigger || isCollision || isCounter || isTimer))
			return;

		auto effectGameObj = static_cast<NIDEffectGameObject*>(object);

		bool& hasIDNameLabel = effectGameObj->m_fields->m_has_id_name_label;
		CCLabelBMFont* idNameLabel = effectGameObj->m_fields->m_id_name_label;
		std::string idNameStr = "";
		CCPoint idLabelPos;

		switch (object->m_objectID)
		{
			// toggle block (doesn't have an ID label)
			case 3643u:
				return;

			// counter object
			case 1615u:
				idLabelPos = CCPoint{ 21.75f, 7.75f };

				if (auto label = effectGameObj->getChildByID("counter-label"); !label)
				{
					if (auto idLabel = effectGameObj->getChildByType<CCLabelBMFont*>(0))
					{
						idLabel->setID("counter-label");
						idLabelPos = idLabel->getPosition();
					}
				}
				break;

			// Link Visible Trigger
			case 3662u:
				idLabelPos = CCPoint{ 18.f, 22.f };
				break;

			default:
				if (auto label = effectGameObj->getChildByID("target-id-label"))
				{
					idLabelPos = label->getPosition();
				}
				else if (auto idLabel = effectGameObj->getChildByType<CCLabelBMFont*>(0); idLabel)
				{
					idLabel->setID("target-id-label");
					idLabelPos = idLabel->getPosition();
				}
				break;
		}

		// Counter Trigger
		if (object->m_objectID == 1615u)
		{
			auto labelNode = static_cast<LabelGameObject*>(object);

			// Disable if counter should show MainTime/Points/Attempts
			if (labelNode->m_shownSpecial != 0);
			else if (labelNode->m_isTimeCounter)
				idNameStr = NIDManager::getNameForID<NID::TIMER>(
					effectGameObj->m_itemID
				).unwrapOr("");
			else
				idNameStr = NIDManager::getNameForID<NID::COUNTER>(
					effectGameObj->m_itemID
				).unwrapOr("");
		}
		// Pulse Trigger
		else if (object->m_objectID == 1006u)
		{
			auto pulseTrigger = static_cast<EffectGameObject*>(object);

			if (pulseTrigger->m_pulseTargetType == 1)
				idNameStr = NIDManager::getNameForID<NID::GROUP>(
					effectGameObj->m_targetGroupID
				).unwrapOr("");
			else
				idNameStr = NIDManager::getNameForID<NID::COLOR>(
					effectGameObj->m_targetGroupID
				).unwrapOr("");

		}
		// Random Trigger
		else if (effectGameObj->m_objectID == 1912u)
		{
			auto id1 = NIDManager::getNameForID<NID::GROUP>(
				effectGameObj->m_targetGroupID
			).unwrapOr("");
			auto id2 = NIDManager::getNameForID<NID::GROUP>(
				effectGameObj->m_centerGroupID
			).unwrapOr("");

			if (!id1.empty() && !id2.empty())
				idNameStr = fmt::format("{}/\n{}", id1, id2);

			idLabelPos = CCPoint{ idLabelPos.x + .75f, idLabelPos.y - 5.5f };
		}
		else if (isTrigger)
			idNameStr = NIDManager::getNameForID<NID::GROUP>(
				effectGameObj->m_targetGroupID
			).unwrapOr("");
		else if (isCollision)
			idNameStr = NIDManager::getNameForID<NID::COLLISION>(
				effectGameObj->m_itemID
			).unwrapOr("");
		else if (isCounter)
			idNameStr = NIDManager::getNameForID<NID::COUNTER>(
				effectGameObj->m_itemID
			).unwrapOr("");
		else if (isTimer)
			idNameStr = NIDManager::getNameForID<NID::TIMER>(
				effectGameObj->m_itemID
			).unwrapOr("");

		if (ng::globals::g_isEditorIDAPILoaded)
		{
			NID nid;
			short id;

			if (isTrigger)
				nid = NID::GROUP;
			else if (isCollision)
				nid = NID::COLLISION;
			else if (isCounter)
				nid = NID::COUNTER;
			else if (isTimer)
				nid = NID::TIMER;

			if (isTrigger)
			{
				idNameLabel->setVisible(
					NIDExtrasManager::getIsNamedIDPreviewed(nid, effectGameObj->m_targetGroupID).unwrapOr(true) &&
					NIDExtrasManager::getIsNamedIDPreviewed(nid, effectGameObj->m_centerGroupID).unwrapOr(true)
				);
			}
			else
			{
				idNameLabel->setVisible(
					NIDExtrasManager::getIsNamedIDPreviewed(nid, effectGameObj->m_itemID).unwrapOr(true)
				);
			}
		}

		idNameLabel->setString(idNameStr.c_str());
		// 28.5f is content width of move trigger, which works well for all other triggers
		idNameLabel->limitLabelWidth(28.5f + 10.f, .5f, .1f);
		idNameLabel->setPosition({ idLabelPos.x, idLabelPos.y - 9.f });

		if (!hasIDNameLabel)
		{
			hasIDNameLabel = true;

			idNameLabel->setID("id-name-label"_spr);
			object->addChild(idNameLabel);
		}
	}
};

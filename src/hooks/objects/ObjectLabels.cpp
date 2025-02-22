#include <Geode/modify/EffectGameObject.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

#include <NIDManager.hpp>

#include "utils.hpp"
#include "constants.hpp"

using namespace geode::prelude;

struct NIDEffectGameObject : geode::Modify<NIDEffectGameObject, EffectGameObject>
{
	struct Fields
	{
		CCLabelBMFont* m_id_name_label;
		bool m_has_id_name_label = false;
	};

	void customSetup()
	{
		EffectGameObject::customSetup();

		auto idNameLabel = CCLabelBMFont::create("", "bigFont.fnt");
		// 28.5f is content width of move trigger, which works well for all other triggers
		idNameLabel->limitLabelWidth(28.5f + 10.f, .6f, .1f);
		m_fields->m_id_name_label = idNameLabel;
		// can't add the label here
	}
};

struct NIDLevelEditorLayer : geode::Modify<NIDLevelEditorLayer, LevelEditorLayer>
{
	static void updateObjectLabel(GameObject* object)
	{
		auto objInArray = [&](const auto& container) {
			return ng::utils::getIndexOf(container, object->m_objectID) != -1;
		};
		bool isTrigger = objInArray(ng::constants::TRIGGER_OBJECT_IDS_WITH_LABEL);
		bool isCollision = objInArray(ng::constants::COLLISION_OBJECT_IDS_WITH_LABEL);
		bool isCounter = objInArray(ng::constants::COUNTER_OBJECT_IDS_WITH_LABEL);

		LevelEditorLayer::updateObjectLabel(object);

		if (!(isTrigger || isCollision || isCounter))
			return;

		auto effectGameObj = static_cast<NIDEffectGameObject*>(object);

		bool& hasIDNameLabel = effectGameObj->m_fields->m_has_id_name_label;
		CCLabelBMFont* idNameLabel = effectGameObj->m_fields->m_id_name_label;
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

		std::string idNameStr = "";

		if (isTrigger)
		{
			// random trigger
			if (effectGameObj->m_objectID == 1912u)
			{
				idNameStr = "";

				auto id1 = NIDManager::getNameForID<NID::GROUP>(
					effectGameObj->m_targetGroupID
				).unwrapOr(fmt::format("{}", effectGameObj->m_targetGroupID));
				auto id2 = NIDManager::getNameForID<NID::GROUP>(
					effectGameObj->m_centerGroupID
				).unwrapOr(fmt::format("{}", effectGameObj->m_centerGroupID));

				idNameStr = fmt::format("{}/\n{}", id1, id2);

				idLabelPos = CCPoint{ idLabelPos.x + .75f, idLabelPos.y - 5.5f };
			}
			else
				idNameStr = NIDManager::getNameForID<NID::GROUP>(
					effectGameObj->m_targetGroupID
				).unwrapOr("");
		}
		else if (isCollision)
			idNameStr = NIDManager::getNameForID<NID::COLLISION>(
				effectGameObj->m_itemID
			).unwrapOr("");
		else if (isCounter)
			idNameStr = NIDManager::getNameForID<NID::COUNTER>(
				effectGameObj->m_itemID
			).unwrapOr("");

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

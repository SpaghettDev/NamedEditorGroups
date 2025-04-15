#pragma once

#include <array>

#include <Geode/cocos/cocoa/CCGeometry.h>

#include <NIDEnum.hpp>

#include "DynamicPropertyTypes.hpp"
#include "GameObjectData.hpp"
#include "FastMap.hpp"

namespace ng::constants
{
	// transition
	namespace old
	{
		inline constexpr cocos2d::CCPoint SAVE_DATA_OBJECT_POS{ -7000.f, -5000.f };
		inline constexpr std::string_view SAVE_OBJECT_STRING_START = ";1,914,2,-7000,3,-5090,";
	}

	inline constexpr cocos2d::CCPoint SAVE_DATA_OBJECT_POS{ -10000.f, -10000.f };
	inline constexpr std::string_view SAVE_OBJECT_STRING_START = ";1,914,2,-10000,3,-10090,";
	inline constexpr std::string_view TEXT_OBJECT_STRING_SEPARATOR = "31,";

	inline constexpr std::uint8_t MAX_NAMED_ID_LENGTH = 24;
	inline constexpr const char* VALID_NAMED_ID_CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz@_-,.!$^&*()+=/<>?\\01234567890";
	inline constexpr std::string_view VALID_NAMED_ID_CHARACTERS_VIEW = VALID_NAMED_ID_CHARACTERS;

	inline constexpr std::uint16_t MAX_DESCRIPTION_LENGTH = 100;

	inline constexpr std::array TRIGGER_OBJECT_IDS_WITH_LABEL{
		901u, 1616u, 1007u, 1049u,
		1268u, 1346u, 2067u, 1347u,
		3033u, 1814u, 3016u, 3660u,
		1595u, 1912u, 2062u, 1815u,
		1812u, 1611u, 2062u, 3609u,
		1585u, 1006u, 3662u
	};

	inline constexpr std::array COLLISION_OBJECT_IDS_WITH_LABEL{
		1816u, 3643u
	};

	inline constexpr std::array COUNTER_OBJECT_IDS_WITH_LABEL{
		1811u, 1817u, 1615u
	};

	inline constexpr std::array TIMER_OBJECT_IDS_WITH_LABEL{
		1615u
	};

	using namespace ng::types;
	// relies on the fact that fmap<K, V, N> will fill with default values (in this case 0 (NID::_UNKNOWN))
	// if the array is not large enough
	inline constexpr const auto OBJECT_ID_TO_PROPERTIES_INFO = fmap<std::uint16_t, fast_map<fm::element<std::uint16_t, NID>, 7>>({
		// LevelOptionsLayer
		{ -1, fmap<std::uint16_t, NID, 7>({
			// Spawn Group
			{ 13, NID::GROUP }
		}) },
		// Color Trigger
		{ 899, fmap<std::uint16_t, NID, 7>({
			// Color ID
			{ 3, NID::COLOR }
		}) },
		// Move Trigger
		{ 901, fmap<std::uint16_t, NID, 7>({
			// Group ID
			{ 51, NID::GROUP },
			// Center Group ID
			{ 395, NID::GROUP },
			// TargetPos Group ID
			{ 71, NID::GROUP }
		}) },
		// Stop Trigger
		{ 1616, fmap<std::uint16_t, NID, 7>({
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Opacity Trigger
		{ 1007, fmap<std::uint16_t, NID, 7>({
			{ 3, NID::GROUP }
		}) },
		// Toggle Trigger
		{ 1049, fmap<std::uint16_t, NID, 7>({
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Spawn Trigger
		{ 1268, fmap<std::uint16_t, NID, 7>({
			// Group ID
			{ 51, NID::GROUP },
			// Original ID
			// { ?, NID::GROUP },
			// New ID
			// { -2, NID::GROUP },
		}) },
		// Rotate Trigger
		{ 1346, fmap<std::uint16_t, NID, 7>({
			// Rot Target ID
			{ 401, NID::GROUP },
			// Target Group ID
			{ 51, NID::GROUP },
			// Center Group ID
			{ 71, NID::GROUP },
			// MinX ID
			{ 516, NID::GROUP },
			// MaxX ID
			{ 518, NID::GROUP },
			// MinY ID
			{ 517, NID::GROUP },
			// MaxY ID
			{ 519, NID::GROUP },
		}) },
		// Scale Trigger
		{ 2067, fmap<std::uint16_t, NID, 7>({
			// Center Group ID
			{ 71, NID::GROUP },
			// Target Group ID
			{ 51, NID::GROUP }
		}) },
		// Follow Trigger
		{ 1347, fmap<std::uint16_t, NID, 7>({
			// Target Group ID
			{ 2, NID::GROUP },
			// Follow Group ID
			{ 4, NID::GROUP }
		}) },
		// Keyframe Trigger
		{ 3032, fmap<std::uint16_t, NID, 7>({
			// { 76, NID::ANIMATION },
			// Target Group ID
			{ 51, NID::GROUP },
			// Parent ID
			{ 71, NID::GROUP }
		}) },
		// Follow Player Y Trigger
		{ 1814, fmap<std::uint16_t, NID, 7>({
			// Target Group ID
			{ 2, NID::GROUP }
		}) },
		// Advanced Follow Trigger
		{ 3016, fmap<std::uint16_t, NID, 7>({
			// Target Group ID
			{ 51, NID::GROUP },
			// Follow Group ID
			{ 71, NID::GROUP }
		}) },
		// Edit Advanced Follow Trigger
		{ 3660, fmap<std::uint16_t, NID, 7>({
			// Target Group ID
			{ 51, NID::GROUP }
		}) },
		// Re-Target Advanced Follow Trigger
		{ 3661, fmap<std::uint16_t, NID, 7>({
			// Target Group ID
			{ 51, NID::GROUP },
			// Follow Group ID
			{ 71, NID::GROUP }
		}) },
		// Keyframe Trigger
		{ 3033, fmap<std::uint16_t, NID, 7>({
			// Target ID
			{ 51, NID::GROUP },
			// Parent ID
			{ 71, NID::GROUP }
		}) },
		// Area Move Trigger
		{ 3006, fmap<std::uint16_t, NID, 7>({
			// Effect ID
			{ 225, NID::EFFECT },
			// Target Group ID
			{ 51, NID::GROUP },
			// Center Group ID
			{ 71, NID::GROUP }
		}) },
		// Area Rotate Trigger
		{ 3007, fmap<std::uint16_t, NID, 7>({
			// Effect ID
			{ 225, NID::EFFECT },
			// Target Group ID
			{ 51, NID::GROUP },
			// Center Group ID
			{ 71, NID::GROUP }
		}) },
		// Area Scale Trigger
		{ 3008, fmap<std::uint16_t, NID, 7>({
			// Effect ID
			{ 225, NID::EFFECT },
			// Target Group ID
			{ 51, NID::GROUP },
			// Center Group ID
			{ 71, NID::GROUP }
		}) },
		// Area Fade Trigger
		{ 3009, fmap<std::uint16_t, NID, 7>({
			// Effect ID
			{ 225, NID::EFFECT },
			// Target Group ID
			{ 51, NID::GROUP },
			// Center Group ID
			{ 71, NID::GROUP }
		}) },
		// Area Tint Trigger
		{ 3010, fmap<std::uint16_t, NID, 7>({
			// Effect ID
			{ 225, NID::EFFECT },
			// Target Group ID
			{ 51, NID::GROUP },
			// Center Group ID
			{ 71, NID::GROUP },
			// Color Channel
			{ 260, NID::COLOR }
		}) },
		// Edit Area Move Trigger
		{ 3011, fmap<std::uint16_t, NID, 7>({
			// Target Group ID
			{ 51, NID::GROUP }
		}) },
		// Edit Area Rotate Trigger
		{ 3012, fmap<std::uint16_t, NID, 7>({
			// Target Group ID
			{ 51, NID::GROUP }
		}) },
		// Edit Area Scale Trigger
		{ 3013, fmap<std::uint16_t, NID, 7>({
			// Target Group ID
			{ 51, NID::GROUP }
		}) },
		// Edit Area Fade Trigger
		{ 3014, fmap<std::uint16_t, NID, 7>({
			// Target Group ID
			{ 51, NID::GROUP }
		}) },
		// Edit Area Tint Trigger
		{ 3015, fmap<std::uint16_t, NID, 7>({
			// Target Group ID
			{ 51, NID::GROUP }
		}) },
		// Area Stop Trigger
		{ 3024, fmap<std::uint16_t, NID, 7>({
			// Effect ID
			{ 51, NID::EFFECT }
		}) },
		// Touch Toggle
		{ 1595, fmap<std::uint16_t, NID, 7>({
			// Target Group ID
			{ 3, NID::GROUP }
		}) },
		// Timer Trigger
		{ 3614, fmap<std::uint16_t, NID, 7>({
			// Item ID
			{ 80, NID::TIMER },
			// Target ID
			{ 51, NID::GROUP }
		}) },
		// Timer Event Trigger
		{ 3615, fmap<std::uint16_t, NID, 7>({
			// Item ID
			{ 80, NID::TIMER },
			// Target ID
			{ 51, NID::GROUP }
		}) },
		// Timer Control Trigger
		{ 3617, fmap<std::uint16_t, NID, 7>({
			// Item ID
			{ 80, NID::TIMER }
		}) },
		// Item Edit Trigger
		{ 3619, fmap<std::uint16_t, NID, 7>({
			// Item ID 1
			{ 80, NID::DYNAMIC_COUNTER_TIMER },
			// 476 = 1 if item, 2 if timer
			// Item ID 2
			{ 95, NID::DYNAMIC_COUNTER_TIMER },
			// 477 = 1 if item, 2 if timer
			// Target Item ID
			{ 51, NID::DYNAMIC_COUNTER_TIMER }
			// 478 = 1 if item, 2 if timer
		}) },
		// Item Compare Trigger
		{ 3620, fmap<std::uint16_t, NID, 7>({
			// Item ID 1
			{ 80, NID::DYNAMIC_COUNTER_TIMER },
			// 476 = 1 if item, 2 if timer
			// Item ID 2
			{ 95, NID::DYNAMIC_COUNTER_TIMER },
			// 477 = 1 if item, 2 if timer
			// True ID
			{ 51, NID::GROUP },
			// False ID
			{ 71, NID::GROUP }
		}) },
		// Persistent Item Trigger
		{ 3641, fmap<std::uint16_t, NID, 7>({
			{ 80, NID::DYNAMIC_COUNTER_TIMER }
			// 494: 1 if counter
		}) },
		// Random Trigger
		{ 1912, fmap<std::uint16_t, NID, 7>({
			{ 3, NID::GROUP },
			{ 5, NID::GROUP }
		}) },
		// Advanced Random Trigger
		{ 2068, fmap<std::uint16_t, NID, 7>({
			// Group ID
			{ 1, NID::GROUP }
		}) },
		// Sequence Trigger
		// { 3607, fmap<std::uint16_t, NID, 7>({
		// 	// no tags, thanks robert
		// }) },
		// Spawn Particle Trigger
		{ 3608, fmap<std::uint16_t, NID, 7>({
			// Particle Group
			{ 51, NID::GROUP },
			// Position Group
			{ 71, NID::GROUP }
		}) },
		// Reset Trigger
		{ 3618, fmap<std::uint16_t, NID, 7>({
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Static Camera Trigger
		{ 1914, fmap<std::uint16_t, NID, 7>({
			// Target Group ID
			{ 71, NID::GROUP }
		}) },
		// Edge Camera Trigger
		// { 2062, fmap<std::uint16_t, NID, 7>({
		// 	// Target ID doesn't have a tag
		// }) },
		// Edit Song Trigger
		{ 3605, fmap<std::uint16_t, NID, 7>({
			// Group ID 1
			{ 51, NID::GROUP },
			// Group ID 2
			{ 71, NID::GROUP }
		}) },
		// SFX Trigger
		{ 3602, fmap<std::uint16_t, NID, 7>({
			// Group ID 1
			{ 51, NID::GROUP },
			// Group ID 2
			{ 71, NID::GROUP }
		}) },
		// Edit SFX Trigger
		{ 3603, fmap<std::uint16_t, NID, 7>({
			// Group ID
			{ 457, NID::GROUP },
			// Group ID 1
			{ 51, NID::GROUP },
			// Group ID 2
			{ 71, NID::GROUP }
		}) },
		// Event Trigger
		{ 3604, fmap<std::uint16_t, NID, 7>({
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// UI Object Trigger
		{ 3613, fmap<std::uint16_t, NID, 7>({
			// Group ID
			{ 51, NID::GROUP },
			// UI Target
			{ 71, NID::GROUP }
		}) },
		// Visibility Link Trigger
		{ 3662, fmap<std::uint16_t, NID, 7>({
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// On Death Trigger
		{ 1812, fmap<std::uint16_t, NID, 7>({
			{ 51, NID::GROUP }
		}) },
		// End Trigger
		{ 3600, fmap<std::uint16_t, NID, 7>({
			// Spawn ID
			{ 51, NID::GROUP },
			// Target Pos
			{ 71, NID::GROUP }
		}) },
		// Teleport Trigger
		{ 3022, fmap<std::uint16_t, NID, 7>({
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Shockwave Trigger
		{ 2905, fmap<std::uint16_t, NID, 7>({
			// Center ID
			{ 51, NID::GROUP }
		}) },
		// Shockline Trigger
		{ 2907, fmap<std::uint16_t, NID, 7>({
			// Center ID
			{ 51, NID::GROUP }
		}) },
		// Lens Circle Trigger
		{ 2913, fmap<std::uint16_t, NID, 7>({
			// Center ID
			{ 51, NID::GROUP },
			// Tint Channel
			{ 71, NID::COLOR }
		}) },
		// Radial Blur Trigger
		{ 2914, fmap<std::uint16_t, NID, 7>({
			// Center ID
			{ 51, NID::GROUP },
			// Ref Channel
			{ 71, NID::COLOR }
		}) },
		// Motion Blur Trigger
		{ 2915, fmap<std::uint16_t, NID, 7>({
			// Center ID
			{ 51, NID::GROUP },
			// Ref Channel
			{ 71, NID::COLOR }
		}) },
		// Grayscale Trigger
		{ 2919, fmap<std::uint16_t, NID, 7>({
			// Target
			{ 51, NID::GROUP }
		}) },
		// Toggle Block Trigger
		{ 3643, fmap<std::uint16_t, NID, 7>({
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Enter Move Trigger
		{ 3017, fmap<std::uint16_t, NID, 7>({
			// EffectID
			{ 225, NID::EFFECT }
		}) },
		// Enter Rotate Trigger
		{ 3018, fmap<std::uint16_t, NID, 7>({
			// EffectID
			{ 225, NID::EFFECT }
		}) },
		// Enter Scale Trigger
		{ 3019, fmap<std::uint16_t, NID, 7>({
			// EffectID
			{ 225, NID::EFFECT }
		}) },
		// Enter Fade Trigger
		{ 3020, fmap<std::uint16_t, NID, 7>({
			// EffectID
			{ 225, NID::EFFECT }
		}) },
		// Enter Tint Trigger
		{ 3021, fmap<std::uint16_t, NID, 7>({
			// EffectID
			{ 225, NID::EFFECT },
			// Color Channel
			{ 260, NID::COLOR }
		}) },
		// Enter Stop Trigger
		{ 3023, fmap<std::uint16_t, NID, 7>({
			// EffectID
			{ 225, NID::EFFECT }
		}) },

		// Counter Label
		{ 1615, fmap<std::uint16_t, NID, 7>({
			{ 80, NID::DYNAMIC_COUNTER_TIMER }
			// 466: 1 if counter, unspecified if timer
		}) },
		// Checkpoint Object
		{ 2063, fmap<std::uint16_t, NID, 7>({
			// Spawn ID
			{ 51, NID::GROUP },
			// Target Pos
			{ 71, NID::GROUP },
			// Respawn ID
			{ 448, NID::GROUP }
		}) },
		// Teleport Orb
		{ 3027, fmap<std::uint16_t, NID, 7>({
			// Target GroupID
			{ 51, NID::GROUP }
		}) },
		// Toggle Orb
		{ 1594, fmap<std::uint16_t, NID, 7>({
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Teleport Portal
		{ 747, fmap<std::uint16_t, NID, 7>({
			// Target GroupID
			{ 51, NID::GROUP }
		}) },
		// Blue Teleport Portal
		{ 2902, fmap<std::uint16_t, NID, 7>({
			// Target GroupID
			{ 51, NID::GROUP }
		}) },


		// --- Collision Triggers ---

		// Collision Trigger
		{ 1815, fmap<std::uint16_t, NID, 7>({
			// BlockA ID
			{ 0, NID::COLLISION },
			// BlockB ID
			{ 1, NID::COLLISION },
			// Target Group ID
			{ 2, NID::GROUP }
		}) },
		// Instant Collision Trigger
		{ 3609, fmap<std::uint16_t, NID, 7>({
			// BlockA ID
			{ 80, NID::COLLISION },
			// BlockB ID
			{ 95, NID::COLLISION },
			// True Group ID
			{ 51, NID::GROUP },
			// False Group ID
			{ 71, NID::GROUP }
		}) },
		// Collision State Trigger
		{ 3640, fmap<std::uint16_t, NID, 7>({
			// State On Group ID
			{ 51, NID::GROUP },
			// State Off Group ID
			{ 71, NID::GROUP }
		}) },
		// Collision Block Setup Trigger
		// { 1816, fmap<std::uint16_t, NID, 7>({
		// 	// Block ID doesn't have a tag
		// }) },


		// --- Counter Triggers ---

		// Count Trigger
		{ 1611, fmap<std::uint16_t, NID, 7>({
			// Item ID
			{ 0, NID::COUNTER },
			// Target ID
			{ 2, NID::GROUP }
		}) },
		// Instant Count Trigger
		{ 1811, fmap<std::uint16_t, NID, 7>({
			// Item ID
			{ 0, NID::COUNTER },
			// Target ID
			{ 2, NID::GROUP }
		}) },
		// Pickup Trigger
		{ 1817, fmap<std::uint16_t, NID, 7>({
			// Item ID
			{ 80, NID::COUNTER }
		}) },


		// --- Collectable Objects ---

		// Key 1
		{ 1275, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Heart 1
		{ 1587, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Potion 1
		{ 1589, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Skull 1
		{ 1598, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Small Coin
		{ 1614, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Clock
		{ 3601, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Potion 2
		{ 4401, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Potion 3
		{ 4402, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Potion 4
		{ 4403, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Key 2
		{ 4404, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Key 3
		{ 4405, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Key 4
		{ 4406, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4407, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4408, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4409, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Candy
		{ 4410, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Mushroom
		{ 4411, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Bone
		{ 4412, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Ball
		{ 4413, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4414, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4415, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4416, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4417, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Gemstone 1
		{ 4418, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Gemstone 2
		{ 4419, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Gemstone 3
		{ 4420, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Fish 🐟
		{ 4421, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4422, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4423, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4424, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Heart 2
		{ 4425, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Heart 3
		{ 4426, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Page
		{ 4427, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Book 1
		{ 4428, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4429, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Book 2
		{ 4430, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Skull 2
		{ 4432, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Chat Bubble
		{ 4433, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Tail
		{ 4434, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4435, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4436, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Eye of Cthulu
		{ 4437, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Up Arrow
		{ 4438, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Down Arrow
		{ 4439, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Zap
		{ 4440, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Barrier Block
		{ 4441, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Cog
		{ 4442, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Plus 1
		{ 4443, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Plus 2
		{ 4444, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4445, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Gift Box
		{ 4446, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Chest
		{ 4447, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Book 3
		{ 4448, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4449, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Pouch
		{ 4450, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4451, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4452, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Ring
		{ 4453, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Necklace 1
		{ 4454, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Necklace 2
		{ 4455, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4456, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4457, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4458, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4459, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4460, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4461, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4462, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4463, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4464, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4465, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4466, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4467, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4468, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4469, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4470, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4471, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4472, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4473, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4474, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4475, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4476, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4477, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4478, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Shield 1
		{ 4479, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Shield 2
		{ 4480, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Shield 3
		{ 4481, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4482, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4483, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Shield 2
		{ 4484, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4485, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Shield 2
		{ 4486, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Sword
		{ 4487, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Bow 1
		{ 4488, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Bow 2
		{ 4538, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Axe
		{ 4489, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Staff 1
		{ 4490, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Staff 2
		{ 4491, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Shovel
		{ 4492, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Pickaxe
		{ 4493, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Sledgehammer
		{ 4494, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Scythe
		{ 4495, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Staff 3
		{ 4496, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Staff 4
		{ 4497, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Fishing Rod
		{ 4537, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Exclamation Point
		{ 4498, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Question Point
		{ 4499, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Addition
		{ 4500, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Subtraction
		{ 4501, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Equals
		{ 4502, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Multiplication
		{ 4503, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Backslash
		{ 4504, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },

		// 0
		{ 4505, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// 1
		{ 4506, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// 2
		{ 4507, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// 3
		{ 4508, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// 4
		{ 4509, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// 5
		{ 4510, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// 6
		{ 4511, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// 7
		{ 4512, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// 8
		{ 4513, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// 9
		{ 4514, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4515, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4516, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4517, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Key 5
		{ 4518, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4519, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4520, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Gemstone 4
		{ 4521, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4522, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Scroll
		{ 4523, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4524, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4525, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4526, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Arrow 1
		{ 4527, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Arrow 2
		{ 4528, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Cheese
		{ 4529, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Apple
		{ 4530, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Carrot
		{ 4531, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Steak
		{ 4532, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4533, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4534, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4535, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// ?
		{ 4536, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
		// Floppy Disk
		{ 4539, fmap<std::uint16_t, NID, 7>({
			// ItemID
			{ 80, NID::COUNTER },
			// Group ID
			{ 51, NID::GROUP }
		}) },
	});
	static_assert(OBJECT_ID_TO_PROPERTIES_INFO.unique());

	inline constexpr const auto DYNAMIC_PROPERTIES_TOGGLES = fmap<std::uint16_t, fast_map<fm::element<std::uint16_t, ng::types::DynamicPropertyToggleType>, 3>>({
		// Item Edit Trigger
		{ 3619, fmap<std::uint16_t, ng::types::DynamicPropertyToggleType, 3>({
			// Item ID 1
			{ 80, { 476, 1 } },
			// Item ID 2
			{ 95, { 477, 1 } },
			// Target Item ID
			{ 51, { 478, 1 } },
		}) },
		// Item Compare Trigger
		{ 3620, fmap<std::uint16_t, ng::types::DynamicPropertyToggleType, 3>({
			// Item ID 1
			{ 80, { 476, 1 } },
			// Item ID 2
			{ 95, { 477, 1 } },
		}) },
		// Persistent Item Trigger
		{ 3641, fmap<std::uint16_t, ng::types::DynamicPropertyToggleType, 3>({
			// Item ID
			{ 80, { 494, 0 } },
		}) },
		// Counter Label
		{ 1615, fmap<std::uint16_t, ng::types::DynamicPropertyToggleType, 3>({
			// Item ID
			{ 80, { 466, 0 } },
		}) },
	});
	static_assert(DYNAMIC_PROPERTIES_TOGGLES.unique());

	inline constexpr const auto DYNAMIC_PROPERTIES_CHOICES = fmap<std::uint16_t, fast_map<fm::element<std::uint16_t, ng::types::DynamicPropertyChoiceType>, 3>>({
		// Item Edit Trigger
		{ 3619, fmap<std::uint16_t, ng::types::DynamicPropertyChoiceType, 3>({
			// Item ID 1
			{ 476, { 80, 1, 2 } },
			// Item ID 2
			{ 477, { 95, 1, 2 } },
			// Target Item ID
			{ 478, { 51, 1, 2 } },
		}) },
		// Item Compare Trigger
		{ 3620, fmap<std::uint16_t, ng::types::DynamicPropertyChoiceType, 3>({
			// Item ID 1
			{ 476, { 80, 1, 2 } },
			// Item ID 2
			{ 477, { 95, 1, 2 } },
		}) },
	});
	static_assert(DYNAMIC_PROPERTIES_CHOICES.unique());

	// lord help me this game is unbearable
#define MAKE_GETTER_PAIR(member) { +[](GameObjectData& o) -> int& { return o.member; }, +[](EffectGameObject* o) -> int& { return o->member; } }
#define FMAP_GETTER_PAIR fmap<NID, std::pair<int&(*)(ng::types::GameObjectData&), int&(*)(EffectGameObject*)>, 6>
	inline constexpr const auto EFFECTGAMEOBJECT_TO_ID_TYPE = fmap<std::uint16_t, fast_map<fm::element<NID, std::pair<int&(*)(ng::types::GameObjectData&), int&(*)(EffectGameObject*)>>, static_cast<std::size_t>(NID::_INTERNAL_TLAST)>>({
		// Move Trigger
		{ 901, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetModCenterID) }
		}) },
		// Stop Trigger
		{ 1616, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// Pulse Trigger
		{ 1006, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::COLOR, MAKE_GETTER_PAIR(m_copyColorID) }
		}) },
		// Alpha Trigger
		{ 1007, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// Stop Trigger
		{ 1049, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// Spawn Trigger
		{ 1268, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// Rotate Trigger
		{ 1346, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_rotationTargetID) }
		}) },
		// Scale Trigger
		{ 2067, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Follow Trigger
		{ 1347, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Animate Trigger
		{ 1585, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// Keyframe Trigger
		{ 3033, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Follow Player Y Trigger
		{ 1814, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// Advanced Follow Trigger
		{ 3016, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Edit Advanced Follow Trigger
		{ 3660, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// Re-Target Advanced Follow Trigger
		{ 3661, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Keyframe Trigger
		{ 3032, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Area Move Trigger
		{ 3006, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Area Rotate Trigger
		{ 3007, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Area Scale Trigger
		{ 3008, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Area Fade Trigger
		{ 3009, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Area Tint Trigger
		{ 3010, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Edit Area Move Trigger
		{ 3011, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// Edit Area Rotate Trigger
		{ 3012, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// Edit Area Scale Trigger
		{ 3013, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// Edit Area Fade Trigger
		{ 3014, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// Edit Area Tint Trigger
		{ 3015, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// Touch Trigger
		{ 1595, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// Count Trigger
		{ 1611, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::COUNTER, MAKE_GETTER_PAIR(m_itemID) }
		}) },
		// Instant Count Trigger
		{ 1811, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::COUNTER, MAKE_GETTER_PAIR(m_itemID) }
		}) },
		// Instant Count Trigger
		{ 1817, FMAP_GETTER_PAIR({
			{ NID::COUNTER, MAKE_GETTER_PAIR(m_itemID) }
		}) },
		// Timer Trigger
		{ 3614, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::TIMER, MAKE_GETTER_PAIR(m_itemID) }
		}) },
		// Timer Event Trigger
		{ 3615, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::TIMER, MAKE_GETTER_PAIR(m_itemID) }
		}) },
		// Timer Control Trigger
		{ 3617, FMAP_GETTER_PAIR({
			{ NID::TIMER, MAKE_GETTER_PAIR(m_itemID) }
		}) },
		// Item Edit Trigger
		{ 3619, FMAP_GETTER_PAIR({
			{ NID::DYNAMIC_COUNTER_TIMER, MAKE_GETTER_PAIR(m_itemID) },
			{ NID::DYNAMIC_COUNTER_TIMER, MAKE_GETTER_PAIR(m_itemID2) },
			{ NID::DYNAMIC_COUNTER_TIMER, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// Item Compare Trigger
		{ 3620, FMAP_GETTER_PAIR({
			{ NID::DYNAMIC_COUNTER_TIMER, MAKE_GETTER_PAIR(m_itemID) },
			{ NID::DYNAMIC_COUNTER_TIMER, MAKE_GETTER_PAIR(m_itemID2) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Persistent Item Trigger
		{ 3641, FMAP_GETTER_PAIR({
			{ NID::DYNAMIC_COUNTER_TIMER, MAKE_GETTER_PAIR(m_itemID) }
		}) },
		// Random Trigger
		{ 1912, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Spawn Particle Trigger
		{ 3608, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Reset Trigger
		{ 3618, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// Static Camera Trigger
		{ 1914, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Edge Camera Trigger
		{ 2062, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// Edit Song Trigger
		{ 3605, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// SFX Trigger
		{ 3602, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Edit SFX Trigger
		{ 3603, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Event Trigger
		{ 3604, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// UI Object Trigger
		{ 3613, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Visibility Link Trigger
		{ 3662, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// Collision Trigger
		{ 1815, FMAP_GETTER_PAIR({
			{ NID::COLLISION, MAKE_GETTER_PAIR(m_itemID) },
			{ NID::COLLISION, MAKE_GETTER_PAIR(m_itemID2) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Instant Collision Trigger
		{ 3609, FMAP_GETTER_PAIR({
			{ NID::COLLISION, MAKE_GETTER_PAIR(m_itemID) },
			{ NID::COLLISION, MAKE_GETTER_PAIR(m_itemID2) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Collision State Trigger
		{ 3640, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Collision Block
		{ 1816, FMAP_GETTER_PAIR({
			{ NID::COLLISION, MAKE_GETTER_PAIR(m_itemID) }
		}) },
		// On Death Trigger
		{ 1812, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) }
		}) },
		// End Trigger
		{ 3600, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
			{ NID::GROUP, MAKE_GETTER_PAIR(m_centerGroupID) }
		}) },
		// Teleport Trigger
		{ 3622, FMAP_GETTER_PAIR({
			{ NID::GROUP, MAKE_GETTER_PAIR(m_targetGroupID) },
		}) }
	});
	static_assert(EFFECTGAMEOBJECT_TO_ID_TYPE.unique());
#undef FMAP_PAIR
#undef MAKE_GETTER_PAIR
}

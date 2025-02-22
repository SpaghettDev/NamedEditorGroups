#pragma once

#include <array>

#include <Geode/cocos/cocoa/CCGeometry.h>

#include <NIDEnum.hpp>

namespace ng
{
	namespace constants
	{
		constexpr cocos2d::CCPoint SAVE_DATA_OBJECT_POS{ -7000.f, -5000.f };
		// constexpr cocos2d::CCPoint SAVE_DATA_OBJECT_POS{ .0f, .0f };

		constexpr const char* VALID_CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-,.01234567890";

		constexpr std::array TRIGGER_OBJECT_IDS_WITH_LABEL{
			901u, 1616u, 1007u, 1049u,
			1268u, 1346u, 2067u, 1347u,
			3033u, 1814u, 3016u, 3660u,
			1595u, 1912u, 2062u, 1815u,
			1812u, 1611u, 2062u, 3609u
		};
		
		constexpr std::array COLLISION_OBJECT_IDS_WITH_LABEL{
			1816u, 3643u
		};
		
		constexpr std::array COUNTER_OBJECT_IDS_WITH_LABEL{
			1811u, 1817u, 1615u
		};

		const std::unordered_map<std::uint16_t, const std::unordered_map<std::uint16_t, NID>> OBJECT_ID_TO_PROPERTIES_INFO{
			// Move Trigger
			{ 901, { 
				// Group ID
				{ 51, NID::GROUP }
			} },
			// Stop Trigger
			{ 1616, {
				// Group ID
				{ 51, NID::GROUP }
			} },
			// Opacity Trigger
			{ 1007, {
				{ 3, NID::GROUP }
			} },
			// Toggle Trigger
			{ 1049, {
				// Group ID
				{ 51, NID::GROUP }
			} },
			// Spawn Trigger
			{ 1268, {
				// Group ID
				{ 51, NID::GROUP },
				// Original ID
				// { ?, NID::GROUP },
				// New ID
				// { -2, NID::GROUP },
			} },
			// Rotate Trigger
			{ 1346, {
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
			} },
			// Scale Trigger
			{ 2067, {
				// Center Group ID
				{ 71, NID::GROUP },
				// Target Group ID
				{ 51, NID::GROUP }
			} },
			// Follow Trigger
			{ 1347, {
				// Target Group ID
				{ 2, NID::GROUP },
				// Follow Group ID
				{ 4, NID::GROUP }
			} },
			// Keyframe Trigger
			{ 3032, {
				// { 76, NID::ANIMATION },
				// Target Group ID
				{ 51, NID::GROUP },
				// Parent ID
				{ 71, NID::GROUP }
			} },
			// Area Move Trigger
			{ 3006, {
				// Target Group ID
				{ 51, NID::GROUP },
				// Center Group ID
				{ 71, NID::GROUP }
			} },
			// Area Rotate Trigger
			{ 3007, {
				// Target Group ID
				{ 51, NID::GROUP },
				// Center Group ID
				{ 71, NID::GROUP }
			} },
			// Area Scale Trigger
			{ 3008, {
				// Target Group ID
				{ 51, NID::GROUP },
				// Center Group ID
				{ 71, NID::GROUP }
			} },
			// Area Fade Trigger
			{ 3009, {
				// Target Group ID
				{ 51, NID::GROUP },
				// Center Group ID
				{ 71, NID::GROUP }
			} },
			// Area Tint Trigger
			{ 3010, {
				// Target Group ID
				{ 51, NID::GROUP },
				// Center Group ID
				{ 71, NID::GROUP }
			} },
			// Edit Area Move Trigger
			{ 3011, {
				// Target Group ID
				{ 51, NID::GROUP }
			} },
			// Edit Area Rotate Trigger
			{ 3012, {
				// Target Group ID
				{ 51, NID::GROUP }
			} },
			// Edit Area Scale Trigger
			{ 3013, {
				// Target Group ID
				{ 51, NID::GROUP }
			} },
			// Edit Area Fade Trigger
			{ 3014, {
				// Target Group ID
				{ 51, NID::GROUP }
			} },
			// Edit Area Tint Trigger
			{ 3015, {
				// Target Group ID
				{ 51, NID::GROUP }
			} },
			// Keyframe Trigger
			{ 3033, {
				// Target ID
				{ 51, NID::GROUP },
				// Parent ID
				{ 71, NID::GROUP }
			} },
			// Follow Player Y Trigger
			{ 1814, {
				// Target Group ID
				{ 2, NID::GROUP }
			} },
			// Advanced Follow Trigger
			{ 3016, {
				// Target Group ID
				{ 51, NID::GROUP },
				// Follow Group ID
				{ 71, NID::GROUP }
			} },
			// Edit Advanced Follow Trigger
			{ 3660, {
				// Target Group ID
				{ 51, NID::GROUP }
			} },
			// Re-Target Advanced Follow Trigger
			{ 3661, {
				// Target Group ID
				{ 51, NID::GROUP },
				// Follow Group ID
				{ 71, NID::GROUP }
			} },
			// Touch Toggle
			{ 1595, {
				// Target Group ID
				{ 3, NID::GROUP }
			} },
			// Random Trigger
			{ 1912, {
				{ 3, NID::GROUP },
				{ 5, NID::GROUP }
			} },
			// Advanced Random Trigger
			{ 2068, {
				// Group ID
				{ 1, NID::GROUP }
			} },
			// Sequence Trigger
			{ 3607, {
				// no tags, thanks robert
			} },
			// Reset Trigger
			{ 3618, {
				// Group ID
				{ 51, NID::GROUP }
			} },
			// Static Camera Trigger
			{ 1914, {
				// Target Group ID
				{ 71, NID::GROUP }
			} },
			// On Death Trigger
			{ 1812, {
				{ 51, NID::GROUP }
			} },
			// End Trigger
			{ 3600, {
				// Spawn ID
				{ 51, NID::GROUP },
				// Target Pos
				{ 71, NID::GROUP }
			} },
			// Teleport Trigger
			{ 3022, {
				{ 51, NID::GROUP }
			} },
			// Edge Trigger
			{ 2062, {
				// Target ID doesn't have a tag
			} },
			// Toggle Block Trigger
			{ 3643, {
				// Group ID
				{ 51, NID::GROUP }
			} },
			// Edit Song Trigger
			{ 3605, { 
				// Group ID 1
				{ 51, NID::GROUP },
				// Group ID 2
				{ 71, NID::GROUP }
			} },
			// SFX Trigger
			{ 3602, {
				// Group ID 1
				{ 51, NID::GROUP },
				// Group ID 2
				{ 71, NID::GROUP }
			} },
			// Edit SFX Trigger
			{ 3603, {
				// Group ID
				{ 457, NID::GROUP },
				// Group ID 1
				{ 51, NID::GROUP },
				// Group ID 2
				{ 71, NID::GROUP }
			} },
			// Event Trigger
			{ 3604, {
				// Group ID
				{ 51, NID::GROUP }
			} },
			// UI Object Trigger
			{ 3613, {
				// Group ID
				{ 51, NID::GROUP },
				// UI Target
				{ 71, NID::GROUP }
			} },
			// Timer Trigger
			{ 3614, {
				// Item ID
				{ 80, NID::COUNTER },
				// Target ID
				{ 51, NID::GROUP }
			} },
			// Timer Event Trigger
			{ 3615, {
				// Item ID
				{ 80, NID::COUNTER },
				// Target ID
				{ 51, NID::GROUP }
			} },
			// Timer Control Trigger
			{ 3617, {
				// Item ID
				{ 80, NID::COUNTER }
			} },
			// Item Edit Trigger
			{ 3619, {
				// Item ID 1
				{ 80, NID::COUNTER },
				// Item ID 2
				{ 95, NID::COUNTER },
				// Target Item ID
				{ 51, NID::COUNTER }
			} },
			// Item Compare Trigger
			{ 3620, {
				// Item ID 1
				{ 80, NID::COUNTER },
				// Item ID 2
				{ 95, NID::COUNTER },
				// True ID
				{ 51, NID::COUNTER },
				// False ID
				{ 71, NID::COUNTER }
			} },
			// Persistent Item Trigger
			{ 3641, {
				{ 80, NID::COUNTER }
			} },
			// Visibility Link Trigger
			{ 3662, {
				// Group ID
				{ 51, NID::GROUP }
			} },
			// Counter Label
			{ 1615, {
				{ 80, NID::COUNTER }
			} },


			// --- Collision Triggers ---
		
			// Collision Trigger
			{ 1815, {
				// BlockA ID
				{ 0, NID::COLLISION },
				// BlockB ID
				{ 1, NID::COLLISION },
				// Target Group ID
				{ 2, NID::GROUP }
			} },
			// Instant Collision Trigger
			{ 3609, {
				// BlockA ID
				{ 80, NID::COLLISION },
				// BlockB ID
				{ 95, NID::COLLISION },
				// True Group ID
				{ 51, NID::GROUP },
				// False Group ID
				{ 71, NID::GROUP }
			} },
			// Collision State Trigger
			{ 3640, {
				// State On Group ID
				{ 51, NID::GROUP },
				// State Off Group ID
				{ 71, NID::GROUP }
			} },
			// Collision Block Setup Trigger
			{ 1816, {
				// Block ID doesn't have a tag
			} },
		
		
			// --- Counter Triggers ---
		
			// Count Trigger
			{ 1611, {
				// Item ID
				{ 0, NID::COUNTER },
				// Target ID
				{ 2, NID::GROUP }
			} },
			// Instant Count Trigger
			{ 1811, {
				// Item ID
				{ 0, NID::COUNTER },
				// Target ID
				{ 2, NID::GROUP }
			} },
			// Pickup Trigger
			{ 1817, {
				// Item ID
				{ 80, NID::COUNTER }
			} }
		};
	}
}

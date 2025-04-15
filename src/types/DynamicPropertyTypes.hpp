#pragma once

#include <cstdint>

namespace ng::types
{
	struct DynamicPropertyToggleType
	{
		std::uint16_t togglePropID;
		std::uint8_t counterState; // timer otherwise
	};

	struct DynamicPropertyChoiceType
	{
		std::uint16_t targetPropID;
		std::uint8_t counterState;
		std::uint8_t timerState;
	};
}

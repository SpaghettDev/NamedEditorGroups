#pragma once

#include <cstdint>

namespace ng::types
{
	struct DynamicPropertyToggleType
	{
		std::uint16_t togglePropID;
		union
		{
			std::uint8_t counterState;
			std::uint8_t timerState;
		};
	};

	struct DynamicPropertyChoiceType
	{
		std::uint16_t targetPropID;
		std::uint8_t counterState;
		std::uint8_t timerState;
	};
}

#pragma once

enum class NID
{
	GROUP,
	COLLISION,
	COUNTER,
	TIMER,
	DYNAMIC_COUNTER_TIMER,

#ifdef SPAGHETTDEV_NAMED_EDITOR_GROUPS_EXPORTING
	_INTERNAL_LAST
#endif
};

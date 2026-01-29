#pragma once

#ifdef NID_DEBUG_BUILD
	#define NID_DEBUG(...) __VA_ARGS__
#else
	#define NID_DEBUG(...)
#endif

// ts pmo...

namespace ng
{
	namespace utils
	{
		namespace cocos {}
		namespace editor {}
		namespace ranges {}
		namespace fuzzy_match {}
	}

	namespace constants {}
	namespace globals {}
	namespace types {}
	namespace base64 {}
	namespace parser {}
	namespace debug {}
}

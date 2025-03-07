#pragma once

#include <functional>
#include <string>
#include <string_view>

#include <Geode/cocos/base_nodes/CCNode.h>

#include <NIDEnum.hpp>

namespace ng
{
	namespace utils
	{
		template <NID nid>
		static inline consteval std::string_view getNamedIDIndentifier()
		{
			if constexpr (nid == NID::GROUP)
				return "Group ID";
			else if constexpr (nid == NID::COLLISION)
				return "Collision ID";
			else if constexpr (nid == NID::COUNTER)
				return "Counter ID";
			else if constexpr (nid == NID::TIMER)
				return "Timer ID";
			else if constexpr (nid == NID::DYNAMIC_COUNTER_TIMER)
				return "THIS SHOULD NOT HAPPEN REPORT ASAP";

			throw "Invalid Named ID enum value";
		}

		static inline constexpr std::string_view getNamedIDIndentifier(NID nid)
		{
			if (nid == NID::GROUP)
				return getNamedIDIndentifier<NID::GROUP>();
			else if (nid == NID::COLLISION)
				return getNamedIDIndentifier<NID::COLLISION>();
			else if (nid == NID::COUNTER)
				return getNamedIDIndentifier<NID::COUNTER>();
			else if (nid == NID::TIMER)
				return getNamedIDIndentifier<NID::TIMER>();
			else if (nid == NID::DYNAMIC_COUNTER_TIMER)
				return getNamedIDIndentifier<NID::DYNAMIC_COUNTER_TIMER>();

			throw "Invalid Named ID enum value";
		}


		template <typename C, typename T>
		inline std::ptrdiff_t getIndexOf(const C& container, const T& toFind)
		{
			const auto it = std::find_if(container.cbegin(), container.cend(), [&](const T& value) {
				return value == toFind;
			});

			return it != container.cend() ? (it - container.cbegin()) : -1;
		}

		template <typename C, typename T>
		inline std::ptrdiff_t getIndexOf(const C& container, std::function<bool(const T&)>&& predicate)
		{
			const auto it = std::find_if(container.cbegin(), container.cend(), predicate);

			return it != container.cend() ? (it - container.cbegin()) : -1;
		}

		template <typename T>
		inline std::ptrdiff_t getIndexOf(const cocos2d::CCArray* array, std::function<bool(const T)>&& predicate)
		{
			for (std::size_t idx = 0; auto elem : geode::cocos::CCArrayExt<T>(array))
			{
				if (predicate(elem))
					return idx;

				idx++;
			}

			return -1;
		}


		geode::Result<> sanitizeName(const std::string_view);

		namespace cocos
		{
			void createNotificationToast(cocos2d::CCLayer*, const std::string&, float, float);

			cocos2d::CCNode* getChildByPredicate(cocos2d::CCNode*, std::function<bool(cocos2d::CCNode*)>&&);

			// allows nested/multiple CCMenus to receive touches
			void fixTouchPriority(cocos2d::CCTouchDelegate*);
		}

		namespace editor
		{
			void refreshObjectLabels(bool);
		}
	}
}

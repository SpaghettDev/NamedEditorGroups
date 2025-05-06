#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <vector>
#include <set>
#include <algorithm>
#include <type_traits>

#include <Geode/cocos/base_nodes/CCNode.h>

#include <NIDEnum.hpp>

namespace ng::utils
{
	namespace impl
	{
		template <typename T>
		struct PtrRefComparator
		{
			bool operator()(const T* lhs, const T& rhs) const
			{
				return *lhs < rhs;
			}

			bool operator()(const T& lhs, const T* rhs) const
			{
				return lhs < *rhs;
			}

			bool operator()(const T* lhs, const T* rhs) const
			{
				return *lhs < *rhs;
			}
		};
	}

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
		else if constexpr (nid == NID::EFFECT)
			return "Effect ID";
		else if constexpr (nid == NID::COLOR)
			return "Color ID";

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
		else if (nid == NID::EFFECT)
			return getNamedIDIndentifier<NID::EFFECT>();
		else if (nid == NID::COLOR)
			return getNamedIDIndentifier<NID::COLOR>();

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

	template<typename T> requires(!std::is_pointer_v<T>)
	std::set<T> multiSetIntersection(std::vector<std::set<T>>&& sets)
	{
		if (sets.empty()) return {};

		std::set<T> result = sets[0];

		for (std::size_t i = 1; i < sets.size() && !result.empty(); ++i)
		{
			std::set<T> temp;
			std::set_intersection(
				result.begin(), result.end(),
				sets[i].begin(), sets[i].end(),
				std::inserter(temp, temp.begin())
			);
			result = std::move(temp);
		}

		return result;
	}

	template<typename T> requires(std::is_pointer_v<T>)
	std::set<T, impl::PtrRefComparator<std::remove_pointer_t<T>>> multiSetIntersection(
		std::vector<std::span<std::remove_pointer_t<T>>>&& sets
	) {
		if (sets.empty()) return {};

		std::set<T, impl::PtrRefComparator<std::remove_pointer_t<T>>> result;
		std::ranges::transform(
			sets[0],
			std::inserter(result, result.end()),
			[](auto& obj) { return &obj; }
		);

		for (std::size_t i = 1; i < sets.size() && !result.empty(); ++i)
		{
			std::set<T, impl::PtrRefComparator<std::remove_pointer_t<T>>> temp;
			std::set_intersection(
				result.begin(), result.end(),
				sets[i].begin(), sets[i].end(),
				std::inserter(temp, temp.begin()),
				impl::PtrRefComparator<std::remove_pointer_t<T>>{}
			);
			result = std::move(temp);
		}

		return result;
	}

	geode::Result<> sanitizeName(const std::string_view);
	bool startsWithNumbers(const std::string_view);
	bool endsWithNumbers(const std::string_view);
	geode::Result<int> numberFromStart(const std::string_view);
	geode::Result<int> numberFromEnd(const std::string_view);
}

namespace ng::utils::cocos
{
	void createNotificationToast(cocos2d::CCLayer*, const std::string&, float, float);
	cocos2d::CCNode* getChildByPredicate(cocos2d::CCNode*, std::function<bool(cocos2d::CCNode*)>&&);

	// allows nested/multiple CCMenus to receive touches
	void fixTouchPriority(cocos2d::CCTouchDelegate*);
}

namespace ng::utils::editor
{
	void refreshObjectLabels();
}

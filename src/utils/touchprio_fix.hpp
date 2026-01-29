#pragma once

#include <type_traits>

#include "vmthooker.hpp"

namespace ng::utils::impl
{
	template<typename T>
	struct GetModifyBase;

	template<typename T>
	struct GetModifyBase
	{
	private:
		template<typename U>
		static auto test(const geode::Modify<U, void>*) -> geode::Modify<U, void>;
		
		template<typename U, typename V>
		static auto test(const geode::Modify<U, V>*) -> geode::Modify<U, V>;
		
		static auto test(...) -> void;
		
	public:
		using type = decltype(test(static_cast<const T*>(nullptr)));
	};


	template<typename ModifyBase>
	struct ModifyTraits;

	template<typename D, typename B>
	struct ModifyTraits<geode::Modify<D, B>>
	{
		using Derived = D;
		using Base = B;
	};


	template<typename T>
	concept ModifyDerived = !std::is_void_v<typename GetModifyBase<T>::type>;


	template<typename T>
	struct ModifyBaseExtractor;

	template<typename T>
		requires ModifyDerived<T>
	struct ModifyBaseExtractor<T>
	{
	private:
		using ModifyBase = typename GetModifyBase<T>::type;
		using Traits = ModifyTraits<ModifyBase>;

	public:
		using Derived = typename Traits::Derived;
		using Base = typename Traits::Base;
	};
}

namespace ng::utils::cocos
{
	template <typename T>
		requires ng::utils::impl::ModifyDerived<T>
	void fixTouchPriority(T* delegate)
	{
		using Derived = typename ng::utils::impl::ModifyBaseExtractor<T>::Derived;
		using Base = typename ng::utils::impl::ModifyBaseExtractor<T>::Base;

		cocos2d::CCTouchDispatcher::get()->addTargetedDelegate(
			delegate, cocos2d::kCCMenuHandlerPriority, true
		);

		if constexpr (std::is_constructible_v<Base, geode::CutoffConstructorType, std::size_t>)
			ng::utils::VMTHooker<&cocos2d::CCLayer::onExit, Base>::get(static_cast<Base*>(delegate))
				.toggleHook(Derived::onExitHook, true);
		else
			ng::utils::VMTHooker<&cocos2d::CCLayer::onExit, Base>::get()
				.toggleHook(Derived::onExitHook, true);
	}
}

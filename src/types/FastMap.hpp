#pragma once

// https://github.com/mapbox/eternal

#include <utility>
#include <functional>

namespace ng::types
{
	namespace fm
	{
		template <typename Compare, typename Iterator, typename Key>
		constexpr auto bound(Iterator left, Iterator right, const Key& key) noexcept
		{
			std::size_t count = right - left;
			while (count > 0)
			{
				const std::size_t step = count / 2;
				right = left + step;
				if (Compare()(*right, key))
				{
					left = ++right;
					count -= step + 1;
				}
				else
					count = step;
			}

			return left;
		}

		struct greater_equal
		{
			template <typename A, typename B>
			constexpr bool operator()(const A& a, const B& b) const noexcept { return !(b < a); }
		};

		template <typename Element>
		class iterator
		{
		public:
			constexpr iterator(const Element* pos) noexcept : pos(pos) {}

			constexpr bool operator==(const iterator& rhs) const noexcept { return pos == rhs.pos; }
			constexpr bool operator!=(const iterator& rhs) const noexcept { return pos != rhs.pos; }

			constexpr iterator& operator++() noexcept { ++pos; return *this; }
			constexpr iterator& operator+=(std::size_t i) noexcept { pos += i; return *this; }
			constexpr iterator operator+(std::size_t i) const noexcept { return pos + i; }

			constexpr iterator& operator--() noexcept { --pos; return *this; }
			constexpr iterator& operator-=(std::size_t i) noexcept { pos -= i; return *this; }
			constexpr std::size_t operator-(const iterator& rhs) const noexcept { return pos - rhs.pos; }

			constexpr const auto& operator*() const noexcept { return **pos; }
			constexpr const auto* operator->() const noexcept { return &**pos; }

		private:
			const Element* pos;
		};

		template <typename Key>
		class compare_key
		{
		public:
			constexpr compare_key(const Key& key) noexcept : key(key) {}

			template <typename Element>
			constexpr bool operator<(const Element& rhs) const noexcept { return key < rhs->first; }

			const Key key;
		};

		template <typename Key, typename Value>
		class element
		{
		public:
			using key_type = Key;
			using mapped_type = Value;
			using value_type = std::pair<key_type, mapped_type>;
			using compare_key_type = compare_key<key_type>;

			constexpr element(const value_type& pair) noexcept : pair(pair) {}
			constexpr element(const key_type& key, const mapped_type& value) noexcept : pair(key, value) {}

			constexpr bool operator<(const element& rhs) const noexcept { return pair.first < rhs.pair.first; }
			constexpr bool operator<(const compare_key_type& rhs) const noexcept { return pair.first < rhs.key; }
			constexpr const auto& operator*() const noexcept { return pair; }
			constexpr const auto* operator->() const noexcept { return &pair; }

			constexpr void swap(element& rhs) noexcept
			{
				std::swap(pair.first, rhs.pair.first);
				std::swap(pair.second, rhs.pair.second);
			}

		private:
			value_type pair;
		};

		template <typename Key, typename Hasher = std::hash<Key>>
		class compare_key_hash : public compare_key<Key>
		{
			using base_type = compare_key<Key>;

		public:
			const std::size_t hash;

			constexpr compare_key_hash(const Key& key_) noexcept : base_type(key_), hash(Hasher()(key_)) {}

			template <typename Element>
			constexpr bool operator<(const Element& rhs) const noexcept
			{
				return hash < rhs.hash || (!(rhs.hash < hash) && base_type::operator<(rhs));
			}
		};
	}

	/**
	 * @brief constexpr std
	 */
	template <typename E, std::size_t N>
	class fast_map
	{
	private:
		static_assert(N > 0, "map is empty");

		E m_data[N];

		template <typename T, std::size_t... I>
		constexpr fast_map(const T (&data)[N], std::index_sequence<I...>) noexcept
			: m_data{ { data[I].first, data[I].second }... }
		{
			static_assert(sizeof...(I) == N, "index_sequence has identical length");

			for (auto left = m_data, right = m_data + N - 1; m_data < right; right = left, left = m_data) {
				for (auto it = m_data; it < right; ++it) {
					if (it[1] < it[0]) {
						it[0].swap(it[1]);
						left = it;
					}
				}
			}
		}

		using compare_key_type = typename E::compare_key_type;

	public:
		template <typename T>
		constexpr fast_map(const T (&data)[N]) noexcept : fast_map(data, std::make_index_sequence<N>()) {}

		using key_type = typename E::key_type;
		using mapped_type = typename E::mapped_type;
		using value_type = typename E::value_type;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using const_reference = const value_type&;
		using const_pointer = const value_type*;
		using const_iterator = fm::iterator<E>;

		constexpr bool unique() const noexcept
		{
			for (auto right = m_data + N - 1, it = m_data; it < right; ++it)
				if (!(it[0] < it[1]))
					return false;

			return true;
		}

		constexpr const mapped_type& at(const key_type& key) const noexcept { return find(key)->second; }

		constexpr const mapped_type& operator[](const key_type& key) const noexcept { return at(key); }

		constexpr std::size_t size() const noexcept { return N; }

		constexpr const_iterator begin() const noexcept { return m_data; }
		constexpr const_iterator cbegin() const noexcept { return begin(); }

		constexpr const_iterator end() const noexcept { return m_data + N; }
		constexpr const_iterator cend() const noexcept { return end(); }

		constexpr const_iterator lower_bound(const key_type& key) const noexcept
		{
			return bound<std::less<void>>(m_data, m_data + N, compare_key_type{ key });
		}

		constexpr const_iterator upper_bound(const key_type& key) const noexcept
		{
			return bound<std::greater_equal<void>>(m_data, m_data + N, compare_key_type{ key });
		}

		constexpr std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const noexcept
		{
			const compare_key_type compare_key{ key };
			auto first = bound<std::less<void>>(m_data, m_data + N, compare_key);
			return { first, bound<std::greater_equal<void>>(first, m_data + N, compare_key) };
		}

		constexpr std::size_t count(const key_type& key) const noexcept
		{
			const auto range = equal_range(key);
			return range.second - range.first;
		}

		constexpr const_iterator find(const key_type& key) const noexcept
		{
			const compare_key_type compare_key{ key };
			auto it = bound<std::less<void>>(m_data, m_data + N, compare_key);
			if (it != m_data + N && fm::greater_equal{}(*it, compare_key))
				return it;
			else
				return end();
		}

		constexpr bool contains(const key_type& key) const noexcept { return find(key) != end(); }
	};

	template <typename Key, typename Value, std::size_t N>
	static constexpr auto fmap(const std::pair<const Key, const Value> (&items)[N]) noexcept
	{
		return fast_map<fm::element<Key, Value>, N>(items);
	}
}

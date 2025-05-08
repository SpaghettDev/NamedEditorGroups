#pragma once

#include <span>

namespace ng::utils::ranges
{
	template <template <typename ...> typename C, typename... Args> requires
		requires(C<Args...> c) { 
			typename C<Args...>::value_type;
			typename C<Args...>::iterator;
			typename C<Args...>::const_iterator;
			std::convertible_to<typename C<Args...>::value_type, std::span<typename C<Args...>::value_type::value_type>>;
			{ c.begin() } -> std::same_as<typename C<Args...>::iterator>;
			{ c.end() } -> std::same_as<typename C<Args...>::iterator>;
			{ c.size() } -> std::convertible_to<std::size_t>;
		}
	class JoinedSpans : public std::ranges::view_interface<JoinedSpans<C, Args...>>
	{
	public:
		using container_t = C<Args...>;
		using contained_t = typename container_t::value_type;
		using spans_contained_t = typename container_t::value_type::value_type;

	public:
		class Iterator
		{
		public:
			using span_iterator_t = typename C<Args...>::const_iterator;

		public:
			using iterator_concept = std::input_iterator_tag;
			using value_type = spans_contained_t;
			using difference_type = std::ptrdiff_t;
			using pointer = spans_contained_t*;
			using reference = spans_contained_t&;

			Iterator() = default;

			Iterator(span_iterator_t begin, span_iterator_t end)
				: m_span_begin(begin), m_span_end(end)
			{
				if (m_span_begin != m_span_end)
				{
					m_elem_it = m_span_begin->begin();
					advance();
				}
			}

			spans_contained_t& operator*() const { return *m_elem_it; }

			Iterator& operator++()
			{
				++m_elem_it;
				advance();
				return *this;
			}

			void operator++(int)
			{
				++*this;
			}

			bool operator!=(const Iterator& other) const
			{
				return m_span_begin != other.m_span_begin;
			}

			bool operator==(const Iterator& other) const
			{
				return m_span_begin == other.m_span_begin;
			}

		private:
			span_iterator_t m_span_begin;
			span_iterator_t m_span_end;
			contained_t::iterator m_elem_it;

			void advance()
			{
				while (m_span_begin != m_span_end && m_elem_it == m_span_begin->end())
				{
					++m_span_begin;
					if (m_span_begin != m_span_end)
						m_elem_it = m_span_begin->begin();
				}
			}
		};

		explicit JoinedSpans(const C<Args...>& spans)
			: m_begin(spans.cbegin()), m_end(spans.cend())
		{}

		auto begin() const { return Iterator(m_begin, m_end); }
		auto end() const { return Iterator(m_end, m_end); }

	private:
		Iterator::span_iterator_t m_begin, m_end;
	};

	template <template <typename ...> typename C, typename... Args>
	inline JoinedSpans<C, Args...> join_spans(const C<Args...>& spans)
	{
		return JoinedSpans<C, Args...>(spans);
	}
}

namespace std::ranges
{
	template <template <typename ...> typename C, typename... Args>
	inline constexpr bool enable_view<JoinedSpans<C, Args...>> = true;
}
